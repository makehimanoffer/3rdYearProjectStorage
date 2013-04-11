
// Terrain.h

#ifndef _Terrain_H_
#define _Terrain_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>


//////////
// MY CLASS INCLUDES //
//////////
#include "Texture.h"



// Class name: Terrain

class Terrain
{
private:
	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
	};
//have added and modified some of structures to accommodate bump mapping.

	struct VectorType 
	{ 
		float x, y, z;
	};

	struct ModelType 
	{ 
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};
	
public:
	Terrain();
	~Terrain();

	bool Init(ID3D11Device*, char*, WCHAR*, WCHAR*);
	void KillSafely();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetColorTexture();
	ID3D11ShaderResourceView* GetNormalMapTexture();

private:
	bool LoadHeightMap(char*);
	void ShutdownHeightMap();
	void ReduceHeightMap();
	bool CalculateNormals();
//have a new function to build a model structure from terrain height data so that tangent and binormal calculations can be performed on model data.

	bool BuildTerrainModel();
	void ReleaseTerrainModel();

	//These are two new functions for calculating tangent and binormal for each triangle in terrain model.

	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertex, TempVertex, TempVertex, VectorType&, VectorType&);

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

private:
	int _terrainWidth, _terrainHeight;
	HeightMapType* _heightMap;

	//This is new structure for holding terrain model data.

	ModelType* _TerrainModel;
	int _vertexCount, _indexCount;
	ID3D11Buffer *_vertexBuff, *_indexBuff;

	//Here are two textures for color detail and normal map.

	Texture *_ColorTexture, *_NormalMapTexture;
};

#endif
