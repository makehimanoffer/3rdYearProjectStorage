
// SkyplaneShader.cpp

#include "SkyplaneShader.h"


SkyplaneShader::SkyplaneShader()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_sampleState = 0;
	_matrixBuffer = 0;
	_skyBuffer = 0;
}


SkyplaneShader::~SkyplaneShader()
{
}


bool SkyplaneShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"../skyplane.vs", L"../skyplane.ps");
	if(!result)
	{
		return false;
	}

	return true;
}


void SkyplaneShader::KillSafely()
{
	// Shutdown vertex and pixel shaders as well as related objects.
	ShutdownShader();

	return;
}


bool SkyplaneShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMx, D3DXMATRIX viewMx, 
								 D3DXMATRIX projectionMx, ID3D11ShaderResourceView* cloudTexture, ID3D11ShaderResourceView* perturbTexture, 
								 float translation, float scale, float brightness)
{
	bool result;


	// Set shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMx, viewMx, projectionMx, cloudTexture, perturbTexture, translation, scale, brightness);
	if(!result)
	{
		return false;
	}

	// Now render prepared buffers with shader.
	RenderShader(deviceContext, indexCount);

	return true;
}


bool SkyplaneShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
    D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC skyBufferDesc;


	// Initialize pointers function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    // Compile vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "SkyPlaneVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
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
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "SkyPlanePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
								   &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If shader failed to compile it should have writen something to error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If there was nothing in error message then it simply could not find file itself.
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

    // Create pixel shader from buffer.
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
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
									   &_layout);
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

    // Setup description of dynamic matrix constant buffer that is in vertex shader.
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MxBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access vertex shader constant buffer from within class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

    // Setup description of dynamic sky constant buffer that is in pixel shader.
    skyBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	skyBufferDesc.ByteWidth = sizeof(SkyBufferType);
    skyBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    skyBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    skyBufferDesc.MiscFlags = 0;
	skyBufferDesc.StructureByteStride = 0;

	// Create sky buffer pointer so can access pixel shader constant buffer from within class.
	result = device->CreateBuffer(&skyBufferDesc, NULL, &_skyBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void SkyplaneShader::ShutdownShader()
{
	if(_skyBuffer)
	{
		_skyBuffer->Release();
		_skyBuffer = 0;
	}

	if(_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = 0;
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


void SkyplaneShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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


bool SkyplaneShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMx, D3DXMATRIX viewMx, 
											  D3DXMATRIX projectionMx, ID3D11ShaderResourceView* cloudTexture, 
											  ID3D11ShaderResourceView* perturbTexture, float translation, float scale, float brightness)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	MxBufferType* pointerToData;
	SkyBufferType* pointerToData2;
	unsigned int bufferNumber;


	// Transpose matrices to prepare them for shader.
	D3DXMatrixTranspose(&worldMx, &worldMx);
	D3DXMatrixTranspose(&viewMx, &viewMx);
	D3DXMatrixTranspose(&projectionMx, &projectionMx);

	// Lock constant buffer so it can be written to.
	result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in constant buffer.
	pointerToData = (MxBufferType*)mappedResource.pData;

	// Copy matrices into constant buffer.
	pointerToData->world = worldMx;
	pointerToData->view = viewMx;
	pointerToData->projection = projectionMx;

	// Unlock constant buffer.
    deviceContext->Unmap(_matrixBuffer, 0);

	// Set position of constant buffer in vertex shader.
	bufferNumber = 0;

	// Finally set constant buffer in vertex shader with updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);
	
	// Lock sky constant buffer so it can be written to.
	result = deviceContext->Map(_skyBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in sky constant buffer.
	pointerToData2 = (SkyBufferType*)mappedResource.pData;

	// Copy data into sky constant buffer.
	pointerToData2->translation = translation;
	pointerToData2->scale = scale;
	pointerToData2->brightness = brightness;
	pointerToData2->padding = 0.0f;

	// Unlock sky constant buffer.
    deviceContext->Unmap(_skyBuffer, 0);

	// Set position of sky constant buffer in pixel shader.
	bufferNumber = 0;

	// Now set sky constant buffer in pixel shader with updated values.
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_skyBuffer);
	
	// Set shader texture resources in pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &cloudTexture);
	deviceContext->PSSetShaderResources(1, 1, &perturbTexture);

	return true;
}


void SkyplaneShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set vertex input layout.
	deviceContext->IASetInputLayout(_layout);

    // Set vertex and pixel shaders that will be used to render triangles.
    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

	// Set sampler state in pixel shader.
	deviceContext->PSSetSamplers(0, 1, &_sampleState);

	// Render triangles.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}