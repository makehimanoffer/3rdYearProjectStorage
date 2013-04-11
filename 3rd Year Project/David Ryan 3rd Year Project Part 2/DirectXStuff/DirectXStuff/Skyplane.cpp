
// Skyplane.cpp

#include "Skyplane.h"


Skyplane::Skyplane()
{
	_skyPlane = 0;
	_vertexBuff = 0;
	_indexBuff = 0;
	_CloudTexture = 0;
	_PerturbTexture = 0;
}


Skyplane::~Skyplane()
{
}


bool Skyplane::Init(ID3D11Device* device, WCHAR* cloudTextureFilename, WCHAR* perturbTextureFilename)
{
	int skyPlaneResolution, textureRepeat;
	float skyPlaneWidth, skyPlaneTop, skyPlaneBottom;
	bool result;


	// Set sky plane parameters.
	skyPlaneResolution = 50;
	skyPlaneWidth = 10.0f;
	skyPlaneTop = 0.5f;
	skyPlaneBottom = 0.0f;
	textureRepeat = 2;

	// Set sky plane shader related parameters.
	_scale = 0.3f;
	_brightness = 0.5f;

	// Initialize translation to zero.
	_translation = 0.0f;

	result = InitializeSkyPlane(skyPlaneResolution, skyPlaneWidth, skyPlaneTop, skyPlaneBottom, textureRepeat);
	if(!result)
	{
		return false;
	}

	// Create vertex and index buffer for sky plane.
	result = InitializeBuffers(device, skyPlaneResolution);
	if(!result)
	{
		return false;
	}

	// Load sky plane textures.
	result = LoadTextures(device, cloudTextureFilename, perturbTextureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Skyplane::KillSafely()
{
	ReleaseTextures();

	// Release vertex and index buffer that were used for rendering sky plane.
	ShutdownBuffers();

	ShutdownSkyPlane();

	return;
}


void Skyplane::Render(ID3D11DeviceContext* deviceContext)
{
	// Render sky plane.
	RenderBuffers(deviceContext);

	return;
}


void Skyplane::Update()
{
	// Increment texture translation value each frame.
	_translation += 0.0001f;
	if(_translation > 1.0f)
	{
		_translation -= 1.0f;
	}

	return;
}


int Skyplane::GetIndexCount()
{
	return _indexCount;
}


ID3D11ShaderResourceView* Skyplane::GetCloudTexture()
{
	return _CloudTexture->GetTexture();
}


ID3D11ShaderResourceView* Skyplane::GetPerturbTexture()
{
	return _PerturbTexture->GetTexture();
}


float Skyplane::GetScale()
{
	return _scale;
}


float Skyplane::GetBrightness()
{

	return _brightness;
}


float Skyplane::GetTranslation()
{

	return _translation;
}


bool Skyplane::InitializeSkyPlane(int skyPlaneResolution, float skyPlaneWidth, float skyPlaneTop, float skyPlaneBottom, int textureRepeat)
{
	float quadSize, radius, constant, textureDelta;
	int i, j, index;
	float positionX, positionY, positionZ, tu, tv;


	// Create array to hold sky plane coordinates.
	_skyPlane = new SkyPlaneType[(skyPlaneResolution + 1) * (skyPlaneResolution + 1)];
	
	// Determine size of each quad on sky plane.
	quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// Calculate radius of sky plane based on width.
	radius = skyPlaneWidth / 2.0f;

	// Calculate height constant to increment by.
	constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// Calculate texture coordinate increment value.
	textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// Loop through sky plane and build coordinates based on increment values given.
	for(j=0; j<=skyPlaneResolution; j++)
	{
		for(i=0; i<=skyPlaneResolution; i++)
		{
			// Calculate vertex coordinates.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// Calculate texture coordinates.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// Calculate index into sky plane array to add coordinate.
			index = j * (skyPlaneResolution + 1) + i;

			// Add coordinates to sky plane array.
			_skyPlane[index].x = positionX;
			_skyPlane[index].y = positionY;
			_skyPlane[index].z = positionZ;
			_skyPlane[index].tu = tu;
			_skyPlane[index].tv = tv;
		}
	}

	return true;
}


void Skyplane::ShutdownSkyPlane()
{
	if(_skyPlane)
	{
		delete [] _skyPlane;
		_skyPlane = 0;
	}

	return;
}


bool Skyplane::InitializeBuffers(ID3D11Device* device, int skyPlaneResolution)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBuffDesc, indexBuffDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, j, index, index1, index2, index3, index4;


	// Calculate number of vertices in sky plane mesh.
	_vertexCount = (skyPlaneResolution + 1) * (skyPlaneResolution + 1) * 6;

	// Set index count to same as vertex count.
	_indexCount = _vertexCount;
		
	// Create vertex array.
	vertices = new Vertex[_vertexCount];
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

	// Initialize index into vertex array.
	index = 0;

	// Load vertex and index array with sky plane array data.
	for(j=0; j<skyPlaneResolution; j++)
	{
		for(i=0; i<skyPlaneResolution; i++)
		{
			index1 = j * (skyPlaneResolution + 1) + i;
			index2 = j * (skyPlaneResolution + 1) + (i+1);
			index3 = (j+1) * (skyPlaneResolution + 1) + i;
			index4 = (j+1) * (skyPlaneResolution + 1) + (i+1);

			// Triangle 1 - Upper Left
			vertices[index].position = D3DXVECTOR3(_skyPlane[index1].x, _skyPlane[index1].y, _skyPlane[index1].z);
			vertices[index].texture = D3DXVECTOR2(_skyPlane[index1].tu, _skyPlane[index1].tv);
			indices[index] = index;
			index++;

			// Triangle 1 - Upper Right
			vertices[index].position = D3DXVECTOR3(_skyPlane[index2].x, _skyPlane[index2].y, _skyPlane[index2].z);
			vertices[index].texture = D3DXVECTOR2(_skyPlane[index2].tu, _skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// Triangle 1 - Bottom Left
			vertices[index].position = D3DXVECTOR3(_skyPlane[index3].x, _skyPlane[index3].y, _skyPlane[index3].z);
			vertices[index].texture = D3DXVECTOR2(_skyPlane[index3].tu, _skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// Triangle 2 - Bottom Left
			vertices[index].position = D3DXVECTOR3(_skyPlane[index3].x, _skyPlane[index3].y, _skyPlane[index3].z);
			vertices[index].texture = D3DXVECTOR2(_skyPlane[index3].tu, _skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// Triangle 2 - Upper Right
			vertices[index].position = D3DXVECTOR3(_skyPlane[index2].x, _skyPlane[index2].y, _skyPlane[index2].z);
			vertices[index].texture = D3DXVECTOR2(_skyPlane[index2].tu, _skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// Triangle 2 - Bottom Right
			vertices[index].position = D3DXVECTOR3(_skyPlane[index4].x, _skyPlane[index4].y, _skyPlane[index4].z);
			vertices[index].texture = D3DXVECTOR2(_skyPlane[index4].tu, _skyPlane[index4].tv);
			indices[index] = index;
			index++;
		}
	}

	// Set up description of vertex buffer.
    vertexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBuffDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
    vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBuffDesc.CPUAccessFlags = 0;
    vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create vertex buffer.
    result = device->CreateBuffer(&vertexBuffDesc, &vertexData, &_vertexBuff);
	if(FAILED(result))
	{
		return false;
	}

	// Set up description of index buffer.
    indexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBuffDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
    indexBuffDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBuffDesc.CPUAccessFlags = 0;
    indexBuffDesc.MiscFlags = 0;
	indexBuffDesc.StructureByteStride = 0;

	// Give subresource structure a pointer to index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer.
	result = device->CreateBuffer(&indexBuffDesc, &indexData, &_indexBuff);
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


void Skyplane::ShutdownBuffers()
{
	if(_indexBuff)
	{
		_indexBuff->Release();
		_indexBuff = 0;
	}

	if(_vertexBuff)
	{
		_vertexBuff->Release();
		_vertexBuff = 0;
	}

	return;
}


void Skyplane::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
    stride = sizeof(Vertex); 
	offset = 0;
    
	// Set vertex buffer to active in input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuff, &stride, &offset);

    // Set index buffer to active in input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuff, DXGI_FORMAT_R32_UINT, 0);

    // Set type of primitive that should be rendered from vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool Skyplane::LoadTextures(ID3D11Device* device, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	bool result;
	_CloudTexture = new Texture;
	
	result = _CloudTexture->Init(device, textureFilename1);
	if(!result)
	{
		return false;
	}

	_PerturbTexture = new Texture;

	result = _PerturbTexture->Init(device, textureFilename2);
	if(!result)
	{
		return false;
	}

	return true;
}


void Skyplane::ReleaseTextures()
{
	if(_PerturbTexture)
	{
		_PerturbTexture->KillSafely();
		delete _PerturbTexture;
		_PerturbTexture = 0;
	}

	if(_CloudTexture)
	{
		_CloudTexture->KillSafely();
		delete _CloudTexture;
		_CloudTexture = 0;
	}

	return;
}