
 

#include "TextHandler.h"


TextHandler::TextHandler()
{
	_Font = 0;
	_sentence1 = 0;
	_sentence2 = 0;
	_sentence3 = 0;
	_sentence4 = 0;
	_sentence5 = 0;
	_sentence6 = 0;
	_sentence7 = 0;
	_sentence8 = 0;
	_sentence9 = 0;
	_sentence10 = 0;
	_sentence11 = 0; //T 0.7
}

TextHandler::~TextHandler()
{
}


bool TextHandler::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int scrWidth, int scrHeight, 
						   D3DXMATRIX baseViewMx)
{
	bool result;


	// Store screen width and height for calculating pixel location during sentence updates.
	_scrWidth = scrWidth;
	_scrHeight = scrHeight;

	// Store base view matrix for 2D text rendering.
	_baseViewMx = baseViewMx;

	// Create font object.
	_Font = new FontHandler;
	
	// Initialize font object.
	result = _Font->Init(device, "../fontdata.txt", L"../font.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize font object.", L"Error", MB_OK);
		return false;
	}

	// Initialize first sentence.
	result = InitializeSentence(&_sentence1, 150, device);
	if(!result)
	{
		return false;
	}

	// Initialize second sentence.
	result = InitializeSentence(&_sentence2, 32, device);
	if(!result)
	{
		return false;
	}

	// Initialize third sentence.
	result = InitializeSentence(&_sentence3, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize fourth sentence.
	result = InitializeSentence(&_sentence4, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize fifth sentence.
	result = InitializeSentence(&_sentence5, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize sixth sentence.
	result = InitializeSentence(&_sentence6, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize seventh sentence.
	result = InitializeSentence(&_sentence7, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize eighth sentence.
	result = InitializeSentence(&_sentence8, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize ninth sentence.
	result = InitializeSentence(&_sentence9, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize tenth sentence.
	result = InitializeSentence(&_sentence10, 16, device);
	if(!result)
	{
		return false;
	}

	// Initialize eleventh sentence.
	result = InitializeSentence(&_sentence11, 32, device);
	if(!result)
	{
		return false;
	}

	return true;
}


void TextHandler::KillSafely()
{
	if(_Font)
	{
		_Font->KillSafely();
		delete _Font;
		_Font = 0;
	}

	ReleaseSentence(&_sentence1);
	ReleaseSentence(&_sentence2);
	ReleaseSentence(&_sentence3);
	ReleaseSentence(&_sentence4);
	ReleaseSentence(&_sentence5);
	ReleaseSentence(&_sentence6);
	ReleaseSentence(&_sentence7);
	ReleaseSentence(&_sentence8);
	ReleaseSentence(&_sentence9);
	ReleaseSentence(&_sentence10);
	ReleaseSentence(&_sentence11); //T 0.7

	return;
}


bool TextHandler::Render(ID3D11DeviceContext* deviceContext, FontShader* FontShader, D3DXMATRIX worldMx, D3DXMATRIX orthoMx)
{
	bool result;


	// Draw sentences.
	result = RenderSentence(_sentence1, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence2, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence3, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence4, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence5, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence6, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence7, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence8, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence9, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence10, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	result = RenderSentence(_sentence11, deviceContext, FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}
	return true;
}


bool TextHandler::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	// Initialize sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set maximum length of sentence.
	(*sentence)->maxLength = maxLength;

	// Set number of vertices in vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set number of indexes in index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up description of dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up description of static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}


bool TextHandler::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
							   ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store color of sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get number of letters in sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate X and Y pixel position on screen to start drawing to.
	drawX = (float)(((_scrWidth / 2) * -1) + positionX);
	drawY = (float)((_scrHeight / 2) - positionY);

	// Use font class to build vertex array from sentence text and sentence draw location.
	_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy data into vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}


void TextHandler::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool TextHandler::RenderSentence(SentenceType* sentence, ID3D11DeviceContext* deviceContext, FontShader* FontShader, D3DXMATRIX worldMx, 
							   D3DXMATRIX orthoMx)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set vertex buffer to active in input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set index buffer to active in input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with input sentence color.
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render text using font shader.
	result = FontShader->Render(deviceContext, sentence->indexCount, worldMx, _baseViewMx, orthoMx, _Font->GetTexture(), pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}


bool TextHandler::SetVideoCardInfo(char* videoCardName, int videoCardMemory, ID3D11DeviceContext* deviceContext)
{
	char dataString[150];
	bool result;
	char tempString[16];
	char memoryString[32];


	// Setup video card name string.
	strcpy_s(dataString, "Video Card: ");
	strcat_s(dataString, videoCardName);

	// Update sentence vertex buffer with new string information.
	result = UpdateSentence(_sentence1, dataString, 10, 10, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Truncate memory value to prevent buffer over flow.
	if(videoCardMemory > 9999999)
	{
		videoCardMemory = 9999999;
	}

	// Convert video memory integer value to a string format.
	_itoa_s(videoCardMemory, tempString, 10);

	// Setup video memory string.
	strcpy_s(memoryString, "Video Memory: ");
	strcat_s(memoryString, tempString);
	strcat_s(memoryString, " MB");

	// Update sentence vertex buffer with new string information.
	result = UpdateSentence(_sentence2, memoryString, 10, 30, 1.0f, 1.0f, 1.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}


bool TextHandler::SetFps(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	bool result;


	// Truncate fps to prevent a buffer over flow.
	if(fps > 9999)
	{
		fps = 9999;
	}

	// Convert fps integer to string format.
	_itoa_s(fps, tempString, 10);

	// Setup fps string.
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	// Update sentence vertex buffer with new string information.
	result = UpdateSentence(_sentence3, fpsString, 10, 70, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}


bool TextHandler::SetCpu(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;


	// Convert cpu integer to string format.
	_itoa_s(cpu, tempString, 10);

	// Setup cpu string.
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update sentence vertex buffer with new string information.
	result = UpdateSentence(_sentence4, cpuString, 10, 90, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}


bool TextHandler::SetCameraPosition(float posX, float posY, float posZ, ID3D11DeviceContext* deviceContext)
{
	int positionX, positionY, positionZ;
	char tempString[16];
	char dataString[16];
	bool result;


	// Convert position from floating point to integer.
	positionX = (int)posX;
	positionY = (int)posY;
	positionZ = (int)posZ;

	// Truncate position if it exceeds either 9999 or -9999.
	if(positionX > 9999) { positionX = 9999; }
	if(positionY > 9999) { positionY = 9999; }
	if(positionZ > 9999) { positionZ = 9999; }

	if(positionX < -9999) { positionX = -9999; }
	if(positionY < -9999) { positionY = -9999; }
	if(positionZ < -9999) { positionZ = -9999; }

	// Setup X position string.
	_itoa_s(positionX, tempString, 10);
	strcpy_s(dataString, "X: ");
	strcat_s(dataString, tempString);

	result = UpdateSentence(_sentence5, dataString, 10, 130, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}
	
	// Setup Y position string.
	_itoa_s(positionY, tempString, 10);
	strcpy_s(dataString, "Y: ");
	strcat_s(dataString, tempString);

	result = UpdateSentence(_sentence6, dataString, 10, 150, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Setup Z position string.
	_itoa_s(positionZ, tempString, 10);
	strcpy_s(dataString, "Z: ");
	strcat_s(dataString, tempString);

	result = UpdateSentence(_sentence7, dataString, 10, 170, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}


bool TextHandler::SetCameraRotation(float rotX, float rotY, float rotZ, ID3D11DeviceContext* deviceContext)
{
	int rotationX, rotationY, rotationZ;
	char tempString[16];
	char dataString[16];
	bool result;


	// Convert rotation from floating point to integer.
	rotationX = (int)rotX;
	rotationY = (int)rotY;
	rotationZ = (int)rotZ;

	// Setup X rotation string.
	_itoa_s(rotationX, tempString, 10);
	strcpy_s(dataString, "rX: ");
	strcat_s(dataString, tempString);

	result = UpdateSentence(_sentence8, dataString, 10, 210, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Setup Y rotation string.
	_itoa_s(rotationY, tempString, 10);
	strcpy_s(dataString, "rY: ");
	strcat_s(dataString, tempString);

	result = UpdateSentence(_sentence9, dataString, 10, 230, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	// Setup Z rotation string.
	_itoa_s(rotationZ, tempString, 10);
	strcpy_s(dataString, "rZ: ");
	strcat_s(dataString, tempString);

	result = UpdateSentence(_sentence10, dataString, 10, 250, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}

bool TextHandler::SetRenderCount(int count, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char renderString[32];
	bool result;


	// Truncate render count if it gets to large to prevent a buffer overflow.
	if(count > 999999999)
	{
		count = 999999999;
	}

	// Convert cpu integer to string format.
	_itoa_s(count, tempString, 10);

	// Setup cpu string.
	strcpy_s(renderString, "Render Count: ");
	strcat_s(renderString, tempString);

	// Update sentence vertex buffer with new string information.
	result = UpdateSentence(_sentence11, renderString, 10, 290, 0.0f, 1.0f, 0.0f, deviceContext);
	if(!result)
	{
		return false;
	}

	return true;
}