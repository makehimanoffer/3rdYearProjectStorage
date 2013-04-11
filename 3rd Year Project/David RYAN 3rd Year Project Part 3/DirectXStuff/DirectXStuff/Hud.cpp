
 

#include "Hud.h"

Hud::Hud()
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	_Texture = 0;
}



Hud::~Hud()
{
}


bool Hud::Init(ID3D11Device* device, int scrWidth, int scrHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;

	// Store screen size.
	_scrWidth = scrWidth;
	_scrHeight = scrHeight;

	// Store size in pixels that this bitmap should be rendered at.
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;

	// Initialize previous rendering position to negative one.
	_previousPosX = -1;
	_previousPosY = -1;

	// Initialize vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void Hud::KillSafely()
{
	ReleaseTexture();

	// Shutdown vertex and index buffers.
	ShutdownBuffers();

	return;
}

bool Hud::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY,int positionZ)
{
	bool result;


	// Re-build dynamic vertex buffer for rendering to possibly a different location on screen.
	result = UpdateBuffers(deviceContext, positionX, positionY,positionZ);
	if(!result)
	{
		return false;
	}

	// Put vertex and index buffers on graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}

//get index of shape. usually 6. since making a square out of two triangles
int Hud::GetIndexCount()
{
	return _indexCount;
}

//returns a pointer to 2d image. shader will call this so has access when drawing buffers
ID3D11ShaderResourceView* Hud::GetTexture()
{
	return _Texture->GetTexture();
}

//used to build vertex and index buffers that will be used to draw 2d image.
bool Hud::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;
	// Set number of vertices in vertex array.
	_vertexCount = 6;

	// Set number of indices in index array.
	_indexCount = _vertexCount;

	// Create vertex array.
	vertices = new VertexType[_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create index array.
	indices = new unsigned long[_indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * _vertexCount));

	// Load index array with data.
	for(i=0; i<_indexCount; i++)
	{
		indices[i] = i;
	}

	//since before this was basically like model class. big changes are below. 
	//first. now creating a DYNAMIC vertex buffer.modifying data each frame if needs be
	//to make it dynamic. set usage to D3D11_Usage_DYNAMIC.
	// Set up description of static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	//don't need to make index buffer since always six points
	//even thought coords of vert might change
	// Set up description of static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release arrays now that vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Hud::ShutdownBuffers()
{
	if(_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	if(_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	return;
}

//used to update contents of dynamic vert buffers if needs
bool Hud::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY,int positionZ)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;
	// If position are rendering this bitmap to has not changed then don't update vertex buffer since it
	// currently has correct parameters.
	if((positionX == _previousPosX) && (positionY == _previousPosY))
	{
		return true;
	}
	// If it has changed then update position it is being rendered to.
	_previousPosX = positionX;
	_previousPosY = positionY;

	// Calculate screen coordinates of left side of bitmap.
	left = (float)((_scrWidth / 2) * -1) + (float)positionX;

	// Calculate screen coordinates of right side of bitmap.
	right = left + (float)_bitmapWidth;

	// Calculate screen coordinates of top of bitmap.
	top = (float)(_scrHeight / 2) - (float)positionY;

	// Calculate screen coordinates of bottom of bitmap.
	bottom = top - (float)_bitmapHeight;

	// Create vertex array.
	vertices = new VertexType[_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Load vertex array with data.
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, positionZ);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, positionZ);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, positionZ);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, positionZ);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, positionZ);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, positionZ);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// Lock vertex buffer so it can be written to.
	result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to data in vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy data into vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * _vertexCount));

	// Unlock vertex buffer.
	deviceContext->Unmap(_vertexBuffer, 0);

	// Release vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

//render buffers sets up buffers on GPU
void Hud::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set vertex buffer to active in input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set index buffer to active in input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

//now its loading of texture function
bool Hud::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create texture object.
	_Texture = new Texture;
	
	// Initialize texture object.
	result = _Texture->Init(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

void Hud::ReleaseTexture()
{
	if(_Texture)
	{
		_Texture->KillSafely();
		delete _Texture;
		_Texture = 0;
	}

	return;
}

bool Hud::SetCameraPosition(float posX, float posY, float posZ, ID3D11DeviceContext* deviceContext)
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

	

	return true;
}

bool Hud::SetCameraRotation(float rotX, float rotY, float rotZ, ID3D11DeviceContext* deviceContext)
{
	int rotationX, rotationY, rotationZ;
	char tempString[16];
	char dataString[16];
	bool result;


	// Convert rotation from floating point to integer.
	rotationX = (int)rotX;
	rotationY = (int)rotY;
	rotationZ = (int)rotZ;

	

	return true;
}