
// FontHandler.h

#ifndef _FontHandler_H_
#define _FontHandler_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;



// MY CLASS INCLUDES //

#include "Texture.h"



// Class name: FontHandler

class FontHandler
{
private:
	struct Font
	{
		float left, right;
		int size;
	};

	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	FontHandler();
	~FontHandler();

	bool Init(ID3D11Device*, char*, WCHAR*);
	void KillSafely();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	Font* _Font;
	Texture* _Texture;
};

#endif