
#include "WaterShader.h"


WaterShader::WaterShader()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_sampleState = 0;
	_matrixBuff = 0;
	_camNormBuff = 0;
	_waterBuff= 0;
}



WaterShader::~WaterShader()
{
}


bool WaterShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Init vertex and pixel shaders.
	result = InitShader(device, hwnd, L"../water.vs", L"../water.ps");
	if(!result)
	{
		return false;
	}

	return true;
}


void WaterShader::KillSafely()
{
	// KillSafely vertex and pixel shaders as well as related objects.
	ShutdownShader();

	return;
}


bool WaterShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMx, D3DXMATRIX viewMx, 
			      D3DXMATRIX projectionMx, D3DXMATRIX reflectionMx, ID3D11ShaderResourceView* refractionTexture, 
			      ID3D11ShaderResourceView* reflectionTexture, ID3D11ShaderResourceView* normalTexture, D3DXVECTOR3 cameraPosition, 
			      D3DXVECTOR2 normalMapTiling, float waterTranslation, float reflectRefractScale, D3DXVECTOR4 refractionTint, 
			      D3DXVECTOR3 lightDirection, float specShine)
{
	bool result;


	// Set shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMx, viewMx, projectionMx, reflectionMx, refractionTexture, reflectionTexture, 
				     normalTexture, cameraPosition, normalMapTiling, waterTranslation, reflectRefractScale, refractionTint, lightDirection, 
				     specShine);
	if(!result)
	{
		return false;
	}

	// Now render prepared buffers with shader.
	RenderShader(deviceContext, indexCount);

	return true;
}


bool WaterShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBuffDesc;
	D3D11_BUFFER_DESC CamNormBuffDesc;
	D3D11_BUFFER_DESC WaterBuffDesc;


	// Init pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "WaterVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If shader failed to compile it should have writen something to error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was  nothing in error message then it simply could not find shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "WaterPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If shader failed to compile it should have writen something to error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was  nothing in error message then it simply could not find file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create vertex shader from buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create vertex shader from buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create vertex input layout description.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of elements in layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup description of matrix dynamic constant buffer that is in vertex shader.
	matrixBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBuffDesc.ByteWidth = sizeof(MxBuff);
	matrixBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBuffDesc.MiscFlags = 0;
	matrixBuffDesc.StructureByteStride = 0;

	// Create matrix constant buffer pointer so can access vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBuffDesc, NULL, &_matrixBuff);
	if(FAILED(result))
	{
		return false;
	}

	// Setup description of camera and normal tiling dynamic constant buffer that is in vertex shader.
	CamNormBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	CamNormBuffDesc.ByteWidth = sizeof(CamNormBuff);
	CamNormBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CamNormBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CamNormBuffDesc.MiscFlags = 0;
	CamNormBuffDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&CamNormBuffDesc, NULL, &_camNormBuff);
	if(FAILED(result))
	{
		return false;
	}

	// Setup description of water dynamic constant buffer that is in pixel shader.
	WaterBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	WaterBuffDesc.ByteWidth = sizeof(WaterBuff);
	WaterBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	WaterBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	WaterBuffDesc.MiscFlags = 0;
	WaterBuffDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&WaterBuffDesc, NULL, &_waterBuff);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void WaterShader::ShutdownShader()
{

	if(_waterBuff)
	{
		_waterBuff->Release();
		_waterBuff = 0;
	}

	// Release camera and normal tiling constant buffer.
	if(_camNormBuff)
	{
		_camNormBuff->Release();
		_camNormBuff = 0;
	}

	if(_matrixBuff)
	{
		_matrixBuff->Release();
		_matrixBuff = 0;
	}

	if(_sampleState)
	{
		_sampleState->Release();
		_sampleState = 0;
	}

	if(_layout)
	{
		_layout->Release();
		_layout = 0;
	}

	if(_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = 0;
	}

	if(_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = 0;
	}

	return;
}


void WaterShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get length of message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write error message to.
	fout.open("shader-error.txt");

	// Write out error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close file.
	fout.close();

	// Release error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on screen to notify user to check text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


bool WaterShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMx, D3DXMATRIX viewMx, D3DXMATRIX projectionMx,
					   D3DXMATRIX reflectionMx, ID3D11ShaderResourceView* refractionTexture, 
					   ID3D11ShaderResourceView* reflectionTexture, ID3D11ShaderResourceView* normalTexture, 
					   D3DXVECTOR3 cameraPosition, D3DXVECTOR2 normalMapTiling, float waterTranslation, float reflectRefractScale, 
					   D3DXVECTOR4 refractionTint, D3DXVECTOR3 lightDirection, float specShine)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MxBuff* pointerToData;
	CamNormBuff* pointerToData2;
	WaterBuff* pointerToData3;


	// Transpose all input matrices to prepare them for shader.
	D3DXMatrixTranspose(&worldMx, &worldMx);
	D3DXMatrixTranspose(&viewMx, &viewMx);
	D3DXMatrixTranspose(&projectionMx, &projectionMx);
	D3DXMatrixTranspose(&reflectionMx, &reflectionMx);

	// Lock matrix constant buffer so it can be written to.
	result = deviceContext->Map(_matrixBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in constant buffer.
	pointerToData = (MxBuff*)mappedResource.pData;

	// Copy matrices into constant buffer.
	pointerToData->world = worldMx;
	pointerToData->view = viewMx;
	pointerToData->projection = projectionMx;
	pointerToData->reflection = reflectionMx;

	// Unlock matrix constant buffer.
	deviceContext->Unmap(_matrixBuff, 0);

	// Set position of matrix constant buffer in vertex shader.
	bufferNumber = 0;

	// Now set matrix constant buffer in vertex shader with updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuff);

	// Lock camera and normal tiling constant buffer so it can be written to.
	result = deviceContext->Map(_camNormBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in constant buffer.
	pointerToData2 = (CamNormBuff*)mappedResource.pData;

	// Copy data into constant buffer.
	pointerToData2->cameraPosition = cameraPosition;
	pointerToData2->padding1 = 0.0f;
	pointerToData2->normalMapTiling = normalMapTiling;
	pointerToData2->padding2 = D3DXVECTOR2(0.0f, 0.0f);

	// Unlock constant buffer.
	deviceContext->Unmap(_camNormBuff, 0);

	// Set position of constant buffer in vertex shader.
	bufferNumber = 1;

	// Set constant buffer in vertex shader with updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_camNormBuff);

	// Set texture resources in pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &refractionTexture);
	deviceContext->PSSetShaderResources(1, 1, &reflectionTexture);
	deviceContext->PSSetShaderResources(2, 1, &normalTexture);

	// Lock water constant buffer so it can be written to.
	result = deviceContext->Map(_waterBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in constant buffer.
	pointerToData3 = (WaterBuff*)mappedResource.pData;

	// Copy water data into constant buffer.
	pointerToData3->waterTranslation = waterTranslation;
	pointerToData3->reflectRefractScale = reflectRefractScale;
	pointerToData3->refractionTint = refractionTint;
	pointerToData3->lightDirection = lightDirection;
	pointerToData3->specShine = specShine;
	pointerToData3->padding = D3DXVECTOR2(0.0f, 0.0f);

	// Unlock constant buffer.
	deviceContext->Unmap(_waterBuff, 0);

	// Set position of water constant buffer in pixel shader.
	bufferNumber = 0;

	// Finally set water constant buffer in pixel shader with updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_waterBuff);

	return true;
}


void WaterShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set vertex input layout.
	deviceContext->IASetInputLayout(_layout);

	// Set vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	// Set sampler state in pixel shader.
	deviceContext->PSSetSamplers(0, 1, &_sampleState);

	// Render triangles.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}