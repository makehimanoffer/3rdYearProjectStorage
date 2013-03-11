////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal; //NEW IN TERRAIN 0.3 REPLACES THE COLOR VERSION
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz; // NEW IN TERRAIN 0.3 SO IT NOW HAS THE NORMALS!
	};

	//NEW STRUCTURE THAT'LL BE USED FOR CALCULATING SHARED LIGHTING
	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	TerrainClass();
	TerrainClass(const TerrainClass&);
	~TerrainClass();


	bool Initialize(ID3D11Device*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:

	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals(); //A NEW FUNCTION FOR CALCULATING SHARED NORMALS AFTER A HEIGHT MAP HAS BEEN LOADED.
	//^NEW IN TERRAIN 0.3
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	HeightMapType* m_heightMap;
};

#endif