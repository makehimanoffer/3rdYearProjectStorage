
// Skyplane.h

#ifndef _Skyplane_H_
#define _Skyplane_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx10math.h>


//////////
// MY CLASS INCLUDES //
//////////
#include "Texture.h"



// Class name: Skyplane

class Skyplane
{
private:
	struct SkyPlaneType
	{
		float x, y, z;
		float tu, tv;
	};

	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Skyplane();
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
	bool InitializeSkyPlane(int, float, float, float, int);
	void ShutdownSkyPlane();

	bool InitializeBuffers(ID3D11Device*, int);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

private:
	SkyPlaneType* _skyPlane;
	int _vertexCount, _indexCount;
	ID3D11Buffer *_vertexBuff, *_indexBuff;
	Texture *_CloudTexture, *_PerturbTexture;
	float _scale, _brightness, _translation;
};

#endif