////////////////////////////////////////////////////////////////////////////////
// Filename: Direct3DHandler.h
////////////////////////////////////////////////////////////////////////////////

//WHEN THE 2D RENDERING STUFF CAME IN IN 0.10. CHANGES WERE MADE TO THIS 


#ifndef _Direct3DHandler_H_
#define _Direct3DHandler_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

// this specifies library links. a handier way than having to explicitly set up projects. a simple include of this will do the job

//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>

//same for the headers

////////////////////////////////////////////////////////////////////////////////
// Class name: Direct3DHandler
////////////////////////////////////////////////////////////////////////////////
class Direct3DHandler
{
public:
	Direct3DHandler();
	Direct3DHandler(const Direct3DHandler&);
	~Direct3DHandler();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DXMATRIX&);
	void GetWorldMatrix(D3DXMATRIX&);
	void GetOrthoMatrix(D3DXMATRIX&);

	void GetVideoCardInfo(char*, int&);
	void TurnZBufferOn(); //introduced for handling 2d rendering stuff in 0.10
	void TurnZBufferOff(); //^

	void TurnOnAlphaBlending(); //added for 0.12
	void TurnOffAlphaBlending(); // ^ for font stuff

	void TurnOnCulling(); //NEEDED FOR TURNING ON AND OFF CULLING. NEW IN TERRAIN 0.8
	void TurnOffCulling();

	void TurnOnWireFrame();
	void TurnOffWireFrame();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11RasterizerState* m_rasterStateWireFrame;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;

	ID3D11DepthStencilState* m_depthDisabledStencilState; //new stencil state for 2d drawing. in 0.10

	ID3D11BlendState* m_alphaEnableBlendingState; //added for 0.12.  for turning on alpha blending
	ID3D11BlendState* m_alphaDisableBlendingState; // for turning off alpha blending
};

#endif