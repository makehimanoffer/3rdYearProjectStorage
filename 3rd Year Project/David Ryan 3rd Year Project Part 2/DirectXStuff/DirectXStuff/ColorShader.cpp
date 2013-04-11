
// ColorShader.cpp

#include "ColorShader.h"

ColorShader::ColorShader()
{
	_vertexShader = 0;
	_pixelShader = 0;
	_layout = 0;
	_matrixBuffer = 0;
}


ColorShader::ColorShader(const ColorShader& other)
{
}


ColorShader::~ColorShader()
{
}
//calls init for shaders
bool ColorShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"../color.vs", L"../color.ps");
	if(!result)
	{
		return false;
	}

	return true;
}

void ColorShader::KillSafely()
{
	// Shutdown vertex and pixel shaders as well as related objects.
	ShutdownShader();

	return;
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, 
			      D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;


	// Set shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	// Now render prepared buffers with shader.
	RenderShader(deviceContext, indexCount);

	return true;
}

//VERY IMPORTANT
//what loads shader files and usable to DX and GPU
//shows how vert buffer data looks in graphics pipeline should match .h and .vs

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;


	// Initialize pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile vertex shader code.
	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If shader failed to compile it should have writen something to error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// If there was nothing in error message then it simply could not find shader file itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile pixel shader code.
	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
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

	// Create pixel shader from buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Now setup layout of data that goes into shader.
	// This setup needs to match Vertex stucture in Model and in shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of elements in layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
					   vertexShaderBuffer->GetBufferSize(), &_layout);
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
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create constant buffer pointer so can access vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShader::ShutdownShader()
{
	// Release matrix constant buffer.
	if(_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = 0;
	}

	// Release layout.
	if(_layout)
	{
		_layout->Release();
		_layout = 0;
	}

	// Release pixel shader.
	if(_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = 0;
	}

	// Release vertex shader.
	if(_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = 0;
	}

	return;
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
					   D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// Transpose matrices to prepare them for shader.
	//DX requirement
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock constant buffer so it can be written to.
	result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy matrices into constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock constant buffer.
	deviceContext->Unmap(_matrixBuffer, 0);

	// Set position of constant buffer in vertex shader.
	bufferNumber = 0;

	// Finanly set constant buffer in vertex shader with updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set vertex input layout.
	deviceContext->IASetInputLayout(_layout);

	// Set vertex and pixel shaders that will be used to render this triangle.
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	// Render triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}

