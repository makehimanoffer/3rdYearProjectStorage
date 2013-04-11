
#ifndef _Water_H_
#define _Water_H_


#include <d3d11.h>
#include <d3dx10math.h>


#include "Texture.h"



class Water
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Water();
	Water(const Water&);
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
	float GetSpecularShininess();

private:
	bool InitBuffers(ID3D11Device*, float);
	void KillSafelyBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	float m_waterHeight;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
	
	float m_waterTranslation;
	
	D3DXVECTOR4 m_refractionTint;
	float m_specularShininess;

public:
	D3DXVECTOR2 m_normalMapTiling;
	float m_reflectRefractScale;
};

#endif