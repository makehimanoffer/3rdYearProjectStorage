////////////////////////////////////////////////////////////////////////////////
// Filename: Terrain.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Terrain_H_
#define _Terrain_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: Terrain
////////////////////////////////////////////////////////////////////////////////
class Terrain
{
private:
	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
	};
//We have added and modified some of the structures to accommodate bump mapping.

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

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};
	
public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetColorTexture();
	ID3D11ShaderResourceView* GetNormalMapTexture();

private:
	bool LoadHeightMap(char*);
	void ShutdownHeightMap();
	void ReduceHeightMap();
	bool CalculateNormals();
//We have a new function to build a model structure from the terrain height data so that tangent and binormal calculations can be performed on the model data.

	bool BuildTerrainModel();
	void ReleaseTerrainModel();

	//These are the two new functions for calculating the tangent and binormal for each triangle in the terrain model.

	void CalculateTerrainVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

private:
	int m_terrainWidth, m_terrainHeight;
	HeightMapType* m_heightMap;

	//This is the new structure for holding the terrain model data.

	ModelType* m_TerrainModel;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	//Here are the two textures for the color detail and normal map.

	Texture *m_ColorTexture, *m_NormalMapTexture;
};

#endif
