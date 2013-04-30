
 

#ifndef _Hud_H_
#define _Hud_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx10math.h>



// MY CLASS INCLUDES //

#include "Texture.h"





class Hud
{
private:

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Hud();
	~Hud();

	bool Init(ID3D11Device*, int, int, WCHAR*, int, int);
	void KillSafely();
	bool Render(ID3D11DeviceContext*, int, int,int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int, int);
	bool SetCameraPosition(float, float, float ,ID3D11DeviceContext*);
	bool SetCameraRotation(float, float, float, ID3D11DeviceContext*);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;
	Texture* _Texture;

	int _scrWidth, _scrHeight;
	int _bitmapWidth, _bitmapHeight;
	int _previousPosX, _previousPosY;
};

#endif