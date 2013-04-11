
 

#include "Model.h"

//init vert and ind buffers to null
Model::Model()
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	//_Texture = 0; //added for 0.4



	_model = 0;//new in 0.6
}


Model::~Model()
{
}

//calls buffer initialisation
bool Model::Init(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1) //UPDATED ARG IN 0.17
{
	bool result;

	// Load in model data, new for 0.6
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize vertex and index buffer that hold geometry for triangle.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load texture for this model.
	result = LoadTexture(device, textureFilename1);
	if(!result)
	{
		return false;
	}


	
	return true;
}

void Model::KillSafely()
{
	ReleaseTexture();

	

	// Release vertex and index buffers.
	ShutdownBuffers();

	// Release model data. new to 0.6
	ReleaseModel();
	return;
}





void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put vertex and index buffers on graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Model::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return _Texture->GetTexture();
}


bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i; //new to 0.6 for counter
	
	// Create vertex array.
	vertices = new VertexType[_vertexCount]; // longer set to 3. new to 0.6
	if(!vertices)
	{
		return false;
	}

	// Create index array.
	indices = new unsigned long[_indexCount]; // longer set to 3. new to 0.6
	if(!indices)
	{
		return false;
	}

	// Load vertex array and index array with data. new to 0.6
	for(i=0; i<_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(_model[i].x+290, _model[i].y-1, _model[i].z+280);
		vertices[i].texture = D3DXVECTOR2(_model[i].tu, _model[i].tv);
		vertices[i].normal = D3DXVECTOR3(_model[i].nx, _model[i].ny, _model[i].nz);

		indices[i] = i;
	}

	// Set up description of static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
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

	//once ind and vert buffers been created arrays no longer needed as data in buffers
	// Release arrays now that vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Model::ShutdownBuffers()
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


void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename)
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
//replaces function above. NEW IN 0.17

void Model::ReleaseTexture()
{
	if(_Texture)
	{
		_Texture->KillSafely();
		delete _Texture;
		_Texture = 0;
	}

	return;
}

//NEW IN 0.17. Replaces ReleaseTexture as seen above in comments




//opens text file and reads in vertex count first. then creates modeltype array and then reads each line into array. vertex and index count set
bool Model::LoadModel(char* filename)
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

void Model::ReleaseModel()
{
	if(_model)
	{
		delete [] _model;
		_model = 0;
	}

	return;
}





