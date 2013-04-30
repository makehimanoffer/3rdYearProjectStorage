//THIS CLASS IS SUPPOSED TO REPLACE THE TEXTURE class, but for posterity sake I'm keeping them as part of the 
//project and codepieces.
//instead of having one texture this can handle multiple textures. for this it handles two. but should be easily expandable.
//it also gives calling object access to the textures.

////////////////////////////////////////////////////////////////////////////////
// Filename: texturearrayclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureArrayClass
////////////////////////////////////////////////////////////////////////////////
class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
//This is the two element texture array private variable.

	ID3D11ShaderResourceView* m_textures[2];
};

#endif

