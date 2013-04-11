
 

#include "Texture.h"

Texture::Texture()
{
	_texture = 0;
}


Texture::~Texture()
{
}

bool Texture::Init(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;


	// Load texture in.
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &_texture, NULL);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void Texture::KillSafely()
{
	// Release texture resource.
	if(_texture)
	{
		_texture->Release();
		_texture = 0;
	}

	return;
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return _texture;
}