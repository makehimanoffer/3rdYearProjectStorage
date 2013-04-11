
#ifndef _TerrainShader_H_
#define _TerrainShader_H_



#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;



class TerrainShader
{
private:
	struct MxBuff
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct LightBuff
	{
		D3DXVECTOR4 lightDiffuseColor;
		D3DXVECTOR3 lightDirection;
		float colorTextureBrightness;
	};

public:
	TerrainShader();
	
	~TerrainShader();

	bool Init(ID3D11Device*, HWND);
	void KillSafely();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR4, 
				D3DXVECTOR3, float);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, D3DXVECTOR4, 
							 D3DXVECTOR3, float);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;
	ID3D11Buffer* _matrixBuff;
	ID3D11Buffer* _lightBuff;
};

#endif