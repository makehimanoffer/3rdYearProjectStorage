
 

#ifndef _Texture_H_
#define _Texture_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx11tex.h>





class Texture
{
public:
	Texture();
	~Texture();

	bool Init(ID3D11Device*, WCHAR*);
	void KillSafely();

	ID3D11ShaderResourceView* GetTexture();
	private:
		ID3D11ShaderResourceView* _texture;
};

#endif

