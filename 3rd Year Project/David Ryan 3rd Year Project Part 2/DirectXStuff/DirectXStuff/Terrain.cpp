
// Terrain.cpp

#include "Terrain.h"


Terrain::Terrain()
{
	_heightMap = 0;
	_TerrainModel = 0;
	_vertexBuff = 0;
	_indexBuff = 0;
	_ColorTexture = 0;
	_NormalMapTexture = 0;
}


Terrain::~Terrain()
{
}


bool Terrain::Init(ID3D11Device* device, char* heightMapFilename, WCHAR* colorTextureFilename, WCHAR* normalMapFilename)
{
	bool result;

	/*flow of Initialize function has changed a bit. load height map as usual and then reduce its height. 
	Also still calculate shared normals.
	But after that construct a model out of terrain height map and normal data. 
	This is done so calculating tangent and binormal for each triangle in terrain becomes easy. 
	Once model is built and have calculated bump map normals then build vertex and index buffer for rendering. 
	And finally load in color detail texture and normal map texture.
	*/
	// Load in height map for terrain.
	result = LoadHeightMap(heightMapFilename);
	if(!result)
	{
		return false;
	}

	// Reduce height of height map.
	ReduceHeightMap();

	// Calculate normals for terrain data.
	result = CalculateNormals();
	if(!result)
	{
		return false;
	}

	// Construct a 3D model from height map and normal data.
	result = BuildTerrainModel();
	if(!result)
	{
		return false;
	}

	// Calculate normal, tangent, and binormal vectors for terrain model.
	CalculateTerrainVectors();

	// Initialize vertex and index buffer for terrain.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load textures.
	result = LoadTextures(device, colorTextureFilename, normalMapFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Terrain::KillSafely()
{
	ReleaseTextures();

	// Release vertex and index buffer.
	ShutdownBuffers();

	ReleaseTerrainModel();

	ShutdownHeightMap();

	return;
}


void Terrain::Render(ID3D11DeviceContext* deviceContext)
{
	// Put vertex and index buffers on graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int Terrain::GetIndexCount()
{
	return _indexCount;
}


ID3D11ShaderResourceView* Terrain::GetColorTexture()
{
	return _ColorTexture->GetTexture();
}


ID3D11ShaderResourceView* Terrain::GetNormalMapTexture()
{
	return _NormalMapTexture->GetTexture();
}


bool Terrain::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Read in bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Save dimensions of terrain.
	_terrainWidth = bitmapInfoHeader.biWidth;
	_terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate size of bitmap image data.
	imageSize = _terrainWidth * _terrainHeight * 3;

	// Allocate memory for bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage)
	{
		return false;
	}

	// Move to beginning of bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}

	// Create structure to hold height map data.
	_heightMap = new HeightMapType[_terrainWidth * _terrainHeight];
	
	// Initialize position in image data buffer.
	k=0;

	// Read image data into height map.
	for(j=0; j<_terrainHeight; j++)
	{
		for(i=0; i<_terrainWidth; i++)
		{
			height = bitmapImage[k];
			
			index = (_terrainWidth * j) + i;

			_heightMap[index].x = (float)i;
			_heightMap[index].y = (float)height;
			_heightMap[index].z = (float)j;

			k+=3;
		}
	}

	// Release bitmap image data.
	delete [] bitmapImage;
	bitmapImage = 0;

	return true;
}


void Terrain::ShutdownHeightMap()
{
	if(_heightMap)
	{
		delete [] _heightMap;
		_heightMap = 0;
	}

	return;
}


void Terrain::ReduceHeightMap()
{
	int i, j;


	for(j=0; j<_terrainHeight; j++)
	{
		for(i=0; i<_terrainWidth; i++)
		{
			_heightMap[(_terrainWidth * j) + i].y /= 15.0f;
		}
	}

	return;
}


bool Terrain::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold un-normalized normal vectors.
	normals = new VectorType[(_terrainHeight-1) * (_terrainWidth-1)];
	if(!normals)
	{
		return false;
	}

	// Go through all faces in mesh and calculate their normals.
	for(j=0; j<(_terrainHeight-1); j++)
	{
		for(i=0; i<(_terrainWidth-1); i++)
		{
			index1 = (j * _terrainWidth) + i;
			index2 = (j * _terrainWidth) + (i+1);
			index3 = ((j+1) * _terrainWidth) + i;

			// Get three vertices from face.
			vertex1[0] = _heightMap[index1].x;
			vertex1[1] = _heightMap[index1].y;
			vertex1[2] = _heightMap[index1].z;
		
			vertex2[0] = _heightMap[index2].x;
			vertex2[1] = _heightMap[index2].y;
			vertex2[2] = _heightMap[index2].z;
		
			vertex3[0] = _heightMap[index3].x;
			vertex3[1] = _heightMap[index3].y;
			vertex3[2] = _heightMap[index3].z;

			// Calculate two vectors for face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (_terrainWidth-1)) + i;

			// Calculate cross product of those two vectors to get un-normalized value for face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all vertices and take an average of each face normal that vertex touches to get averaged normal for that vertex.
	for(j=0; j<_terrainHeight; j++)
	{
		for(i=0; i<_terrainWidth; i++)
		{
			// Initialize sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize count.
			count = 0;

			// Bottom left face.
			if(((i-1) >= 0) && ((j-1) >= 0))
			{
				index = ((j-1) * (_terrainWidth-1)) + (i-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if((i < (_terrainWidth-1)) && ((j-1) >= 0))
			{
				index = ((j-1) * (_terrainHeight-1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if(((i-1) >= 0) && (j < (_terrainHeight-1)))
			{
				index = (j * (_terrainHeight-1)) + (i-1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if((i < (_terrainWidth-1)) && (j < (_terrainHeight-1)))
			{
				index = (j * (_terrainHeight-1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}
			
			// Take average of faces touching vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate length of normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));
			
			// Get an index to vertex location in height map array.
			index = (j * _terrainHeight) + i;

			// Normalize final shared normal for vertex and store it in height map array.
			_heightMap[index].nx = (sum[0] / length);
			_heightMap[index].ny = (sum[1] / length);
			_heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release temporary normals.
	delete [] normals;
	normals = 0;

	return true;
}

//BuildTerrainModel function builds terrain triangles same way InitializeBuffers function used to. But instead of creating a vertex and index buffer load terrain data into a ModelType structure array.

bool Terrain::BuildTerrainModel()
{
	int i, j, index, index1, index2, index3, index4;


	// Set number of vertices in model.
	_vertexCount = (_terrainWidth - 1) * (_terrainHeight - 1) * 6;

	_TerrainModel = new ModelType[_vertexCount];
	
	// Load terrain model with height map terrain data.
	index = 0;

	for(j=0; j<(_terrainHeight-1); j++)
	{
		for(i=0; i<(_terrainWidth-1); i++)
		{
			index1 = (_terrainWidth * j) + i;          // Bottom left.
			index2 = (_terrainWidth * j) + (i+1);      // Bottom right.
			index3 = (_terrainWidth * (j+1)) + i;      // Upper left.
			index4 = (_terrainWidth * (j+1)) + (i+1);  // Upper right.

			// Upper left.
			_TerrainModel[index].x  = _heightMap[index3].x;
			_TerrainModel[index].y  = _heightMap[index3].y;
			_TerrainModel[index].z  = _heightMap[index3].z;
			_TerrainModel[index].nx = _heightMap[index3].nx;
			_TerrainModel[index].ny = _heightMap[index3].ny;
			_TerrainModel[index].nz = _heightMap[index3].nz;
			_TerrainModel[index].tu = 0.0f;
			_TerrainModel[index].tv = 0.0f;
			index++;

			// Upper right.
			_TerrainModel[index].x  = _heightMap[index4].x;
			_TerrainModel[index].y  = _heightMap[index4].y;
			_TerrainModel[index].z  = _heightMap[index4].z;
			_TerrainModel[index].nx = _heightMap[index4].nx;
			_TerrainModel[index].ny = _heightMap[index4].ny;
			_TerrainModel[index].nz = _heightMap[index4].nz;
			_TerrainModel[index].tu = 1.0f;
			_TerrainModel[index].tv = 0.0f;
			index++;

			// Bottom left.
			_TerrainModel[index].x  = _heightMap[index1].x;
			_TerrainModel[index].y  = _heightMap[index1].y;
			_TerrainModel[index].z  = _heightMap[index1].z;
			_TerrainModel[index].nx = _heightMap[index1].nx;
			_TerrainModel[index].ny = _heightMap[index1].ny;
			_TerrainModel[index].nz = _heightMap[index1].nz;
			_TerrainModel[index].tu = 0.0f;
			_TerrainModel[index].tv = 1.0f;
			index++;

			// Bottom left.
			_TerrainModel[index].x  = _heightMap[index1].x;
			_TerrainModel[index].y  = _heightMap[index1].y;
			_TerrainModel[index].z  = _heightMap[index1].z;
			_TerrainModel[index].nx = _heightMap[index1].nx;
			_TerrainModel[index].ny = _heightMap[index1].ny;
			_TerrainModel[index].nz = _heightMap[index1].nz;
			_TerrainModel[index].tu = 0.0f;
			_TerrainModel[index].tv = 1.0f;
			index++;

			// Upper right.
			_TerrainModel[index].x  = _heightMap[index4].x;
			_TerrainModel[index].y  = _heightMap[index4].y;
			_TerrainModel[index].z  = _heightMap[index4].z;
			_TerrainModel[index].nx = _heightMap[index4].nx;
			_TerrainModel[index].ny = _heightMap[index4].ny;
			_TerrainModel[index].nz = _heightMap[index4].nz;
			_TerrainModel[index].tu = 1.0f;
			_TerrainModel[index].tv = 0.0f;
			index++;

			// Bottom right.
			_TerrainModel[index].x  = _heightMap[index2].x;
			_TerrainModel[index].y  = _heightMap[index2].y;
			_TerrainModel[index].z  = _heightMap[index2].z;
			_TerrainModel[index].nx = _heightMap[index2].nx;
			_TerrainModel[index].ny = _heightMap[index2].ny;
			_TerrainModel[index].nz = _heightMap[index2].nz;
			_TerrainModel[index].tu = 1.0f;
			_TerrainModel[index].tv = 1.0f;
			index++;
		}
	}

	return true;
}


void Terrain::ReleaseTerrainModel()
{
	if(_TerrainModel)
	{
		delete [] _TerrainModel;
		_TerrainModel = 0;
	}

	return;
}

//CalculateTerrainVectors is function that goes through each triangle in terrain model and calculates and stores tangent and binormal vectors.

void Terrain::CalculateTerrainVectors()
{
	int faceCount, i, index;
	TempVertex vertex1, vertex2, vertex3;
	VectorType tangent, binormal;


	// Calculate number of faces in terrain model.
	faceCount = _vertexCount / 3;

	// Initialize index to model data.
	index = 0;

	// Go through all faces and calculate tangent, binormal, and normal vectors.
	for(i=0; i<faceCount; i++)
	{
		// Get three vertices for face from terrain model.
		vertex1.x  = _TerrainModel[index].x;
		vertex1.y  = _TerrainModel[index].y;
		vertex1.z  = _TerrainModel[index].z;
		vertex1.tu = _TerrainModel[index].tu;
		vertex1.tv = _TerrainModel[index].tv;
		vertex1.nx = _TerrainModel[index].nx;
		vertex1.ny = _TerrainModel[index].ny;
		vertex1.nz = _TerrainModel[index].nz;
		index++;

		vertex2.x  = _TerrainModel[index].x;
		vertex2.y  = _TerrainModel[index].y;
		vertex2.z  = _TerrainModel[index].z;
		vertex2.tu = _TerrainModel[index].tu;
		vertex2.tv = _TerrainModel[index].tv;
		vertex2.nx = _TerrainModel[index].nx;
		vertex2.ny = _TerrainModel[index].ny;
		vertex2.nz = _TerrainModel[index].nz;
		index++;

		vertex3.x  = _TerrainModel[index].x;
		vertex3.y  = _TerrainModel[index].y;
		vertex3.z  = _TerrainModel[index].z;
		vertex3.tu = _TerrainModel[index].tu;
		vertex3.tv = _TerrainModel[index].tv;
		vertex3.nx = _TerrainModel[index].nx;
		vertex3.ny = _TerrainModel[index].ny;
		vertex3.nz = _TerrainModel[index].nz;
		index++;

		// Calculate tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Store tangent and binormal for face back in model structure.
		_TerrainModel[index-1].tx = tangent.x;
		_TerrainModel[index-1].ty = tangent.y;
		_TerrainModel[index-1].tz = tangent.z;
		_TerrainModel[index-1].bx = binormal.x;
		_TerrainModel[index-1].by = binormal.y;
		_TerrainModel[index-1].bz = binormal.z;

		_TerrainModel[index-2].tx = tangent.x;
		_TerrainModel[index-2].ty = tangent.y;
		_TerrainModel[index-2].tz = tangent.z;
		_TerrainModel[index-2].bx = binormal.x;
		_TerrainModel[index-2].by = binormal.y;
		_TerrainModel[index-2].bz = binormal.z;

		_TerrainModel[index-3].tx = tangent.x;
		_TerrainModel[index-3].ty = tangent.y;
		_TerrainModel[index-3].tz = tangent.z;
		_TerrainModel[index-3].bx = binormal.x;
		_TerrainModel[index-3].by = binormal.y;
		_TerrainModel[index-3].bz = binormal.z;
	}

	return;
}


void Terrain::CalculateTangentBinormal(TempVertex vertex1, TempVertex vertex2, TempVertex vertex3, VectorType& tangent, 
					    VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate two vectors for face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate denominator of tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate cross products and multiply by coefficient to get tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate length of normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));
			
	// Normalize normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate length of normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));
			
	// Normalize normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

//InitializeBuffers function has been modified to build vertex and index buffer from terrain model instead of height map data.

bool Terrain::InitializeBuffers(ID3D11Device* device)
{
	Vertex* vertices;
	unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBuffDesc, indexBuffDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


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

	// Load vertex and index array with data from terrain model.
	for(i=0; i<_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(_TerrainModel[i].x, _TerrainModel[i].y, _TerrainModel[i].z);
		vertices[i].texture = D3DXVECTOR2(_TerrainModel[i].tu, _TerrainModel[i].tv);
		vertices[i].normal = D3DXVECTOR3(_TerrainModel[i].nx, _TerrainModel[i].ny, _TerrainModel[i].nz);
		vertices[i].tangent = D3DXVECTOR3(_TerrainModel[i].tx, _TerrainModel[i].ty, _TerrainModel[i].tz);
		vertices[i].binormal = D3DXVECTOR3(_TerrainModel[i].bx, _TerrainModel[i].by, _TerrainModel[i].bz);

		indices[i] = i;
	}

	// Set up description of static vertex buffer.
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

	// Now create vertex buffer.
	result = device->CreateBuffer(&vertexBuffDesc, &vertexData, &_vertexBuff);
	if(FAILED(result))
	{
		return false;
	}

	// Set up description of static index buffer.
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

	// Release arrays now that buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void Terrain::ShutdownBuffers()
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


void Terrain::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

	// Set type of primitive that should be rendered from vertex buffer, in case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
//LoadTextures now loads a color detail map texture and a normal map texture.

bool Terrain::LoadTextures(ID3D11Device* device, WCHAR* colorTexturefilename, WCHAR* normalMapFilename)
{
	bool result;



	_ColorTexture = new Texture;
	
	result = _ColorTexture->Init(device, colorTexturefilename);
	if(!result)
	{
		return false;
	}

	_NormalMapTexture = new Texture;

	result = _NormalMapTexture->Init(device, normalMapFilename);
	if(!result)
	{
		return false;
	}

	return true;
}


void Terrain::ReleaseTextures()
{
	if(_NormalMapTexture)
	{
		_NormalMapTexture->KillSafely();
		delete _NormalMapTexture;
		_NormalMapTexture = 0;
	}

	if(_ColorTexture)
	{
		_ColorTexture->KillSafely();
		delete _ColorTexture;
		_ColorTexture = 0;
	}

	return;
}

