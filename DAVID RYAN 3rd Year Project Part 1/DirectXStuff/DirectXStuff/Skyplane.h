
#ifndef _Skyplane_H_
#define _Skyplane_H_



#include <d3d11.h>
#include <d3dx10math.h>


#include "Texture.h"

class Skyplane
{
private:
	struct SkyPlaneType
	{
		float x, y, z;
		float tu, tv;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Skyplane();
	Skyplane(const Skyplane&);
	~Skyplane();

	bool Init(ID3D11Device*, WCHAR*, WCHAR*);
	void KillSafely();
	void Render(ID3D11DeviceContext*);
	void Update();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetCloudTexture();
	ID3D11ShaderResourceView* GetPerturbTexture();
	
	float GetScale();
	float GetBrightness();
	float GetTranslation();

private:
	bool InitSkyPlane(int, float, float, float, int);
	void ShutdownSkyPlane();

	bool InitBuffers(ID3D11Device*, int);
	void KillSafelyBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

private:
	SkyPlaneType* m_skyPlane;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	Texture *m_CloudTexture, *m_PerturbTexture;
	float m_scale, m_brightness, m_translation;
};

#endif