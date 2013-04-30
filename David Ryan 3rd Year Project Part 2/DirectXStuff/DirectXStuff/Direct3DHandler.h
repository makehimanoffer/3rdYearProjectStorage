
// Direct3DHandler.h

#ifndef _Direct3DHandler_H_
#define _Direct3DHandler_H_



// LINKING //

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")



// INCLUDES //

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>



// Class name: Direct3DHandler

class Direct3DHandler
{
public:
	Direct3DHandler();
	~Direct3DHandler();

	bool Init(int, int, bool, HWND, bool, float, float);
	void KillSafely();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMx(D3DXMATRIX&);
	void GetWorldMx(D3DXMATRIX&);
	void GetOrthoMx(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	void TurnOnCulling();
	void TurnOffCulling();
	void TurnOnWireUpdate();
	void TurnOffWireUpdate();
	void SetBackBufferRenderTarget();
	void ResetViewport();

	void EnableSecondBlendState();

private:
	bool _vsync_enabled;
	int _videoCardMemory;
	char _videoCardDescription[128];
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11RasterizerState* _rasterState;
	ID3D11RasterizerState* _rasterStateNoCulling;
	ID3D11RasterizerState* _rasterStateWireFrame;
	D3DXMATRIX _projectionMx;
	D3DXMATRIX _worldMx;
	D3DXMATRIX _orthoMx;
	ID3D11DepthStencilState* _depthDisabledStencilState;
	ID3D11BlendState* _alphaEnableBlendingState;
	ID3D11BlendState* _alphaDisableBlendingState;
	D3D11_VIEWPORT _viewport;
	ID3D11BlendState* _alphaBlendState2;
};

#endif