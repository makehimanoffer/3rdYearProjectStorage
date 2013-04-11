
#include "Water.h"


Water::Water()
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	_Texture = 0;
}


Water::~Water()
{
}

bool Water::Init(ID3D11Device* device, WCHAR* textureFilename, float waterHeight, float waterRadius)
{
	bool result;

	// Store water height.
	_waterHeight = waterHeight;

	// Init vertex and index buffer that hold geometry for triangle.
	result = InitBuffers(device, waterRadius);
	if(!result)
	{
		return false;
	}

	// Load texture for  model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	// Set tiling for water normal maps.
	_normalMapTiling.x = 0.01f;  // Tile ten times over quad. VALUECHANGE
	_normalMapTiling.y = 0.02f;  // Tile five times over quad. VALUECHANGE

	// Init water translation to zero.
	_waterTranslation = 0.0f;

	// Set scaling value for water normal map.
	_reflectRefractScale = 0.03f; //get rid of 0 VALUECHANGE

	//will do some crazy shit

	// Set tint of refraction.
	_refractionTint = D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);

	// Set specular shininess.
	_specShine = 200.0f;

	return true;
}


void Water::KillSafely()
{
	ReleaseTexture();

	KillSafelyBuffers();

	return;
}

void Water::Update()
{
	// Update position of water to simulate motion.
	_waterTranslation += 0.003f;
	if(_waterTranslation > 1.0f)
	{
		_waterTranslation -= 1.0f;
	}

	return;
}


void Water::Render(ID3D11DeviceContext* deviceContext)
{
	// Put vertex and index buffers on graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Water::GetIndexCount()
{
	return _indexCount;
}


ID3D11ShaderResourceView* Water::GetTexture()
{
	return _Texture->GetTexture();
}


float Water::GetWaterHeight()
{
	return _waterHeight;
}


D3DXVECTOR2 Water::GetNormalMapTiling()
{
	return _normalMapTiling;
}


float Water::GetWaterTranslation()
{
	return _waterTranslation;
}


float Water::GetReflectRefractScale()
{
	return _reflectRefractScale;
}


D3DXVECTOR4 Water::GetRefractionTint()
{
	return _refractionTint;
}


float Water::GetspecShine()
{
	return _specShine;
}


bool Water::InitBuffers(ID3D11Device* device, float waterRadius)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	
	// Set number of vertices in vertex array.
	_vertexCount = 6;

	// Set number of indices in index array.
	_indexCount = 6;

	vertices = new Vertex[_vertexCount];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned long[_indexCount];
	if(!indices)
	{
		return false;
	}

	//MANUALLY CREATE A QUAD

	// Load vertex array with data.
	vertices[0].position = D3DXVECTOR3(-waterRadius, 0.0f, waterRadius);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(waterRadius, 0.0f, waterRadius);  // Top right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 0.0f);
	
	vertices[2].position = D3DXVECTOR3(-waterRadius, 0.0f, -waterRadius);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(-waterRadius, 0.0f, -waterRadius);  // Bottom left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(waterRadius, 0.0f, waterRadius);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(waterRadius, 0.0f, -waterRadius);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	// Load index array with data.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

	// Set up description of vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up description of index buffer.
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


void Water::KillSafelyBuffers()
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


void Water::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(Vertex); 
	offset = 0;
    
	// Set vertex buffer to active in input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set index buffer to active in input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set type of primitive that should be rendered from  vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool Water::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	_Texture = new Texture;
	

	// Init texture object.
	result = _Texture->Init(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Water::ReleaseTexture()
{
	if(_Texture)
	{
		_Texture->KillSafely();
		delete _Texture;
		_Texture = 0;
	}

	return;
}

