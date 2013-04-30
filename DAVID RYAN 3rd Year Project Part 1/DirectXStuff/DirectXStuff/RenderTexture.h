
#ifndef _RenderTexture_H_
#define _RenderTexture_H_



#include <d3d11.h>
#include <d3dx10math.h>



class RenderTexture
{
public:
	RenderTexture();
	
	~RenderTexture();

	bool Init(ID3D11Device*, int, int, float, float);
	void KillSafely();

	void SetRenderTarget(ID3D11DeviceContext*);
	void ClearRenderTarget(ID3D11DeviceContext*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();
	void GetProjectionMx(D3DXMATRIX&);
	void GetOrthoMx(D3DXMATRIX&);

private:
	ID3D11Texture2D* _renderTargetTexture;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11ShaderResourceView* _shaderResourceView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11_VIEWPORT _viewport;
	D3DXMATRIX _projectionMx;
	D3DXMATRIX _orthoMx;

};

#endif