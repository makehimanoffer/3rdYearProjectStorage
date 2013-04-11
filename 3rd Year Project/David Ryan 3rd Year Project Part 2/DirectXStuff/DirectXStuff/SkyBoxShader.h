
// SkyBoxShader.h

#ifndef _SkyBoxShader_H_
#define _SkyBoxShader_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;



// Class name: SkyBoxShader

class SkyBoxShader
{
private:
	struct MxBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct GradientBufferType
	{
		D3DXVECTOR4 apexColor;
		D3DXVECTOR4 centerColor;
	};

public:
	SkyBoxShader();
	~SkyBoxShader();

	bool Init(ID3D11Device*, HWND);
	void KillSafely();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, D3DXVECTOR4, D3DXVECTOR4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11Buffer* _matrixBuffer;
	ID3D11Buffer* _gradientBuffer;
};

#endif