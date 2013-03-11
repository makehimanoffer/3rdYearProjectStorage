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

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
//NEW IN TERR 0.6
#include "textureclass.h"

/////////////
// GLOBALS //
/////////////
const int TEXTURE_REPEAT = 8;
//global here so can easily control how often texture will repeat over the terrain grid. will be repeated over width and length
////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class TerrainClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		//NEW IN TERR 0.6. replaces lighting specific version. and handles texture too
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float tu, tv; // NEW IN TERR 0.6 . texture tu and tv
		float nx, ny, nz; //now has normals
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


	bool Initialize(ID3D11Device*, char*, WCHAR*); // UPDATED IN TERR 0.6 for texture filename
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture(); // NEW IN TERR 0.6 for texturing

private:

	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	bool CalculateNormals(); //A NEW FUNCTION FOR CALCULATING SHARED NORMALS AFTER A HEIGHT MAP HAS BEEN LOADED.
	//^NEW IN TERRAIN 0.3
	void ShutdownHeightMap();


	void CalculateTextureCoordinates(); // NEW IN TERR 0.6
	bool LoadTexture(ID3D11Device*, WCHAR*); //NEW IN TERR 0.6
	void ReleaseTexture(); // NEW IN TERR 0.6
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;

	HeightMapType* m_heightMap;

	TextureClass* m_Texture; // NEW IN TERR 0.6
};

#endif