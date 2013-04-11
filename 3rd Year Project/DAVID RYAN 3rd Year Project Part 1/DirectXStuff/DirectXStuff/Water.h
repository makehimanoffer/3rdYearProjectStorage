
#ifndef _Water_H_
#define _Water_H_


#include <d3d11.h>
#include <d3dx10math.h>


#include "Texture.h"



class Water
{
private:
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Water();
	~Water();

	bool Init(ID3D11Device*, WCHAR*, float, float);
	void KillSafely();
	void Update();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	
	float GetWaterHeight();
	D3DXVECTOR2 GetNormalMapTiling();
	float GetWaterTranslation();
	float GetReflectRefractScale();
	D3DXVECTOR4 GetRefractionTint();
	float GetspecShine();

private:
	bool InitBuffers(ID3D11Device*, float);
	void KillSafelyBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	float _waterHeight;
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;
	Texture* _Texture;
	
	float _waterTranslation;
	
	D3DXVECTOR4 _refractionTint;
	float _specShine;

public:
	D3DXVECTOR2 _normalMapTiling;
	float _reflectRefractScale;
};

#endif