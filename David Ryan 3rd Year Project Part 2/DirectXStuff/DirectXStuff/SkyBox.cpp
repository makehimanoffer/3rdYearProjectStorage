
// SkyBox.cpp

#include "SkyBox.h"


SkyBox::SkyBox()
{
	_model = 0;
	_vertexBuff = 0;
	_indexBuff = 0;
}


SkyBox::~SkyBox()
{
}


bool SkyBox::Init(ID3D11Device* device)
{
	bool result;


	// Load in sky box model.
	result = LoadSkyBoxModel("../SkyBox.txt");
	if(!result)
	{
		return false;
	}

	// Load sky box into a vertex and index buffer for rendering.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Set color at top of sky box.
	_apexColor = D3DXVECTOR4(0.0f, 0.15f, 0.66f, 1.0f);
	_apexColor = D3DXVECTOR4(0.0f, 0.145f, 0.667f, 1.0f);

	// Set color at center of sky box.
	_centerColor =  D3DXVECTOR4(0.81f, 0.38f, 0.66f, 1.0f);
	_centerColor =  D3DXVECTOR4(0.02f, 0.365f, 0.886f, 1.0f);

	return true;
}


void SkyBox::KillSafely()
{
	// Release vertex and index buffer that were used for rendering sky box.
	ReleaseBuffers();
	ReleaseSkyBoxModel();

	return;
}


void SkyBox::Render(ID3D11DeviceContext* deviceContext)
{
	// Render sky box.
	RenderBuffers(deviceContext);

	return;
}


int SkyBox::GetIndexCount()
{
	return _indexCount;
}


D3DXVECTOR4 SkyBox::GetApexColor()
{
	return _apexColor;
}


D3DXVECTOR4 SkyBox::GetCenterColor()
{
	return _centerColor;
}


bool SkyBox::LoadSkyBoxModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open model file.
	fin.open(filename);
	
	// If it could not open file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in vertex count.
	fin >> _vertexCount;

	// Set number of indices to be same as vertex count.
	_indexCount = _vertexCount;

	// Create model using vertex count that was read in.
	_model = new ModelType[_vertexCount];
	
	// Read up to beginning of data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in vertex data.
	for(i=0; i<_vertexCount; i++)
	{
		fin >> _model[i].x >> _model[i].y >> _model[i].z;
		fin >> _model[i].tu >> _model[i].tv;
		fin >> _model[i].nx >> _model[i].ny >> _model[i].nz;
	}

	// Close model file.
	fin.close();

	return true;
}


void SkyBox::ReleaseSkyBoxModel()
{
	if(_model)
	{
		delete [] _model;
		_model = 0;
	}

	return;
}


bool SkyBox::InitializeBuffers(ID3D11Device* device)
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBuffDesc, indexBuffDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	
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

	// Load vertex array and index array with data.
	for(i=0; i<_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(_model[i].x, _model[i].y, _model[i].z);
		indices[i] = i;
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


void SkyBox::ReleaseBuffers()
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


void SkyBox::RenderBuffers(ID3D11DeviceContext* deviceContext)
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