
#include "SkyBoxShader.h"


SkyBoxShader::SkyBoxShader()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_matrixBuff = 0;
	_gradientBuffer = 0;
}


SkyBoxShader::~SkyBoxShader()
{
}


bool SkyBoxShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Init vertex and pixel shaders.
	result = InitShader(device, hwnd, L"../SkyBox.vs", L"../SkyBox.ps");
	if(!result)
	{
		return false;
	}

	return true;
}


void SkyBoxShader::KillSafely()
{
	// Shutdown vertex and pixel shaders as well as related objects.
	ShutdownShader();

	return;
}


bool SkyBoxShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMx, D3DXMATRIX viewMx, 
								D3DXMATRIX projectionMx, D3DXVECTOR4 apexColor, D3DXVECTOR4 centerColor)
{
	bool result;


	// Set shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMx, viewMx, projectionMx, apexColor, centerColor);
	if(!result)
	{
		return false;
	}

	// Now render prepared buffers with shader.
	RenderShader(deviceContext, indexCount);

	return true;
}


bool SkyBoxShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBuffDesc;
	D3D11_BUFFER_DESC gradientBufferDesc;


	// Init pointers  function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    // Compile vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "SkyBoxVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
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
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "SkyBoxPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
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

    // Setup description of dynamic matrix constant buffer that is in vertex shader.
    matrixBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBuffDesc.ByteWidth = sizeof(MxBuff);
    matrixBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBuffDesc.MiscFlags = 0;
	matrixBuffDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access vertex shader constant buffer from within class.
	result = device->CreateBuffer(&matrixBuffDesc, NULL, &_matrixBuff);
	if(FAILED(result))
	{
		return false;
	}

	// Setup description of gradient constant buffer that is in pixel shader.
	gradientBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gradientBufferDesc.ByteWidth = sizeof(GradientBufferType);
	gradientBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gradientBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gradientBufferDesc.MiscFlags = 0;
	gradientBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access pixel shader constant buffer from within  class.
	result = device->CreateBuffer(&gradientBufferDesc, NULL, &_gradientBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void SkyBoxShader::ShutdownShader()
{

	if(_gradientBuffer)
	{
		_gradientBuffer->Release();
		_gradientBuffer = 0;
	}

	if(_matrixBuff)
	{
		_matrixBuff->Release();
		_matrixBuff = 0;
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


void SkyBoxShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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


bool SkyBoxShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMx, D3DXMATRIX viewMx,
											 D3DXMATRIX projectionMx, D3DXVECTOR4 apexColor, D3DXVECTOR4 centerColor)
{
	HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
	MxBuff* pointerToData;
	GradientBufferType* pointerToData2;
	unsigned int bufferNumber;


	// Transpose matrices to prepare them for shader.
	D3DXMatrixTranspose(&worldMx, &worldMx);
	D3DXMatrixTranspose(&viewMx, &viewMx);
	D3DXMatrixTranspose(&projectionMx, &projectionMx);

	// Lock constant buffer so it can be written to.
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

	// Unlock constant buffer.
    deviceContext->Unmap(_matrixBuff, 0);

	// Set position of constant buffer in vertex shader.
	bufferNumber = 0;

	// Finally set constant buffer in vertex shader with updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuff);
	
	// Lock gradient constant buffer so it can be written to.
	result = deviceContext->Map(_gradientBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in constant buffer.
	pointerToData2 = (GradientBufferType*)mappedResource.pData;

	// Copy gradient color variables into constant buffer.
	pointerToData2->apexColor = apexColor;
	pointerToData2->centerColor = centerColor;

	// Unlock constant buffer.
	deviceContext->Unmap(_gradientBuffer, 0);

	// Set position of gradient constant buffer in pixel shader.
	bufferNumber = 0;

	// Finally set gradient constant buffer in pixel shader with updated values.
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &_gradientBuffer);

	return true;
}


void SkyBoxShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set vertex input layout.
	deviceContext->IASetInputLayout(_layout);

    // Set vertex and pixel shaders that will be used to render triangles.
    deviceContext->VSSetShader(_vertexShader, NULL, 0);
    deviceContext->PSSetShader(_pixelShader, NULL, 0);

	// Render triangles.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}