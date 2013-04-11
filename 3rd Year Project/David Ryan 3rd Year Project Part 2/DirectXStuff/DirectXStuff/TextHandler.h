
// TextHandler.h

#ifndef _TextHandler_H_
#define _TextHandler_H_


//////////
// MY CLASS INCLUDES //
//////////
#include "FontHandler.h"
#include "FontShader.h"

// MODIFIED IN TERRAIN 0.7 for new sentence

// Class name: TextHandler

class TextHandler
{
private:
	struct Sentence
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextHandler();
	~TextHandler();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void KillSafely();
	bool Render(ID3D11DeviceContext*, FontShader*, D3DXMATRIX, D3DXMATRIX);

	bool SetVideoCardInfo(char*, int, ID3D11DeviceContext*);
	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetCameraPosition(float, float, float, ID3D11DeviceContext*);
	bool SetCameraRotation(float, float, float, ID3D11DeviceContext*);
	bool SetRenderCount(int, ID3D11DeviceContext*);

private:
	bool InitializeSentence(Sentence**, int, ID3D11Device*);
	bool UpdateSentence(Sentence*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(Sentence**);
	bool RenderSentence(Sentence*, ID3D11DeviceContext*, FontShader*, D3DXMATRIX, D3DXMATRIX);

private:
	int _screenWidth, _screenHeight;
	D3DXMATRIX _baseViewMx;
	FontHandler* _Font;
	Sentence *_sentence1, *_sentence2, *_sentence3, *_sentence4, *_sentence5;
	Sentence *_sentence6, *_sentence7, *_sentence8, *_sentence9, *_sentence10, *_sentence11;
};

#endif