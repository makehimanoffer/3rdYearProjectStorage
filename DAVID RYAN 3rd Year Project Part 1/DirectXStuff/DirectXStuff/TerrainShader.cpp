
#include "TerrainShader.h"


TerrainShader::TerrainShader()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_sampleState = 0;
	_matrixBuff = 0;
	_lightBuff = 0;
}



TerrainShader::~TerrainShader()
{
}


bool TerrainShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Init  vertex and pixel shaders.
	result = InitShader(device, hwnd, L"../terrain.vs", L"../terrain.ps");
	if(!result)
	{
		return false;
	}

	return true;
}


void TerrainShader::KillSafely()
{
	
	ShutdownShader();

	return;
}


bool TerrainShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMx, D3DXMATRIX viewMx, 
								D3DXMATRIX projectionMx, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, 
								D3DXVECTOR4 lightDiffuseColor, D3DXVECTOR3 lightDirection, float colorTextureBrightness)
{
	bool result;


	
	result = SetShaderParameters(deviceContext, worldMx, viewMx, projectionMx, colorTexture, normalTexture, lightDiffuseColor, 
								 lightDirection, colorTextureBrightness);
	if(!result)
	{
		return false;
	}

	// Now render  prepared buffers with  shader.
	RenderShader(deviceContext, indexCount);

	return true;
}


bool TerrainShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuff;
	ID3D10Blob* pixelShaderBuff;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[6];
	unsigned int numElements;
    D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBuffDesc;
	D3D11_BUFFER_DESC lightBuffDesc;


	// Init  pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuff = 0;
	pixelShaderBuff = 0;

    // Compile  vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TerrainVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
								   &vertexShaderBuff, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If  shader failed to compile it should have writen something to  error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If nothing in  error message it simply could not find  shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Compile  pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TerrainPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
								   &pixelShaderBuff, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If  shader failed to compile it should have writen something to  error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// If re was nothing in  error message n it simply could not find  file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Create vertex shader from buffer.
    result = device->CreateVertexShader(vertexShaderBuff->GetBufferPointer(), vertexShaderBuff->GetBufferSize(), NULL, &_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

    // Create pixel shader from buffer.
    result = device->CreatePixelShader(pixelShaderBuff->GetBufferPointer(), pixelShaderBuff->GetBufferSize(), NULL, &_pixelShader);
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

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].SemanticIndex = 0;
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlot = 0;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].InstanceDataStepRate = 0;

	polygonLayout[5].SemanticName = "COLOR";
	polygonLayout[5].SemanticIndex = 0;
	polygonLayout[5].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[5].InputSlot = 0;
	polygonLayout[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[5].InstanceDataStepRate = 0;

	// Get count of elements in  layout.
    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuff->GetBufferPointer(), vertexShaderBuff->GetBufferSize(), &_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release  vertex shader buffer and pixel shader buffer since no longer needed.
	vertexShaderBuff->Release();
	vertexShaderBuff = 0;

	pixelShaderBuff->Release();
	pixelShaderBuff = 0;

	// Create a texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create  texture sampler state.
    result = device->CreateSamplerState(&samplerDesc, &_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup  description of  dynamic matrix constant buffer in vertex shader.
    matrixBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBuffDesc.ByteWidth = sizeof(MxBuff);
    matrixBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBuffDesc.MiscFlags = 0;
	matrixBuffDesc.StructureByteStride = 0;

	// Create  constant buffer pointer so can access  vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBuffDesc, NULL, &_matrixBuff);
	if(FAILED(result))
	{
		return false;
	}

	// Setup  description of  light dynamic constant buffer in pixel shader.
	lightBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBuffDesc.ByteWidth = sizeof(LightBuff);
	lightBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBuffDesc.MiscFlags = 0;
	lightBuffDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access pixel shader constant buffer 
	result = device->CreateBuffer(&lightBuffDesc, NULL, &_lightBuff);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void TerrainShader::ShutdownShader()
{

	if(_lightBuff)
	{
		_lightBuff->Release();
		_lightBuff = 0;
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


void TerrainShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long BuffSize, i;
	ofstream fout;


	// Get a pointer to  error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get  length of  message.
	BuffSize = errorMessage->GetBufferSize();

	// Open a file to write  error message to.
	fout.open("shader-error.txt");

	// Write out  error message.
	for(i=0; i<BuffSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close  file.
	fout.close();

	// Release  error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on  screen to notify  user to check  text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


bool TerrainShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMx, D3DXMATRIX viewMx, 
											 D3DXMATRIX projectionMx, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture, 
											 D3DXVECTOR4 lightDiffuseColor, D3DXVECTOR3 lightDirection, float colorTextureBrightness)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int BuffNumber;
	MxBuff* pointerToData;
	LightBuff* pointerToData2;


	// Transpose  matrices to prepare  for shader.
	D3DXMatrixTranspose(&worldMx, &worldMx);
	D3DXMatrixTranspose(&viewMx, &viewMx);
	D3DXMatrixTranspose(&projectionMx, &projectionMx);

	// Lock  constant buffer so it can be written to.
	result = deviceContext->Map(_matrixBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to  data in  constant buffer.
	pointerToData = (MxBuff*)mappedResource.pData;

	// Copy  matrices into  constant buffer.
	pointerToData->world = worldMx;
	pointerToData->view = viewMx;
	pointerToData->projection = projectionMx;

	// Unlock  constant buffer.
    deviceContext->Unmap(_matrixBuff, 0);

	// Set  position of  constant buffer in  vertex shader.
	BuffNumber = 0;

	// Now set  constant buffer in  vertex shader with  updated values.
    deviceContext->VSSetConstantBuffers(BuffNumber, 1, &_matrixBuff);

	// Lock  light constant buffer so it can be written to.
	result = deviceContext->Map(_lightBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to  data in  constant buffer.
	pointerToData2 = (LightBuff*)mappedResource.pData;

	// Copy  lighting variables into  constant buffer.
	pointerToData2->lightDiffuseColor = lightDiffuseColor;
	pointerToData2->lightDirection = lightDirection;
	pointerToData2->colorTextureBrightness = colorTextureBrightness;

	// Unlock  constant buffer.
	deviceContext->Unmap(_lightBuff, 0);

	// Set  position of  light constant buffer in  pixel shader.
	BuffNumber = 0;

	
	deviceContext->PSSetConstantBuffers(BuffNumber, 1, &_lightBuff);

	// Set texture resources in pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &colorTexture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);

	return true;
}


void TerrainShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	
	deviceContext->IASetInputLayout(_layout);

    
    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

	// Set sampler state in pixel shader.
	deviceContext->PSSetSamplers(0, 1, &_sampleState);

	// Render  triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}