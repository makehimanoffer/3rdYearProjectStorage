
#ifndef _Terrain_H_
#define _Terrain_H_


#include <d3d11.h>
#include <d3dx10math.h>
#include <stdio.h>



#include "Texture.h"

class Terrain
{
private:
	struct HeightMapType 
	{ 
		float x, y, z;
		float nx, ny, nz;
		float r, g, b;
	};

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
		float r, g, b;
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
		D3DXVECTOR4 color;
	};
	
public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Init(ID3D11Device*, char*, char*, float, WCHAR*, WCHAR*);
	void KillSafely();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetColorTexture();
	ID3D11ShaderResourceView* GetNormalTexture();

private:
	bool LoadHeightMap(char*);
	bool LoadColorMap(char*);
	void ReduceHeightMap(float);
	bool CalculateNormals();
	bool BuildModel();
	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	bool InitBuffers(ID3D11Device*);
	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);

	void ReleaseHeightMap();
	void ReleaseModel();
	void ReleaseBuffers();
	void ReleaseTextures();

	void RenderBuffers(ID3D11DeviceContext*);

private:
	int m_terrainWidth, m_terrainHeight, m_vertexCount, m_indexCount;
	HeightMapType* m_heightMap;
	ModelType* m_model;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	Texture *m_ColorTexture, *m_NormalTexture;
};

#endif