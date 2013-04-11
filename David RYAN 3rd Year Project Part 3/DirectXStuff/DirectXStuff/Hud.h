
 

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
	Hud(const Hud&);
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
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

#endif