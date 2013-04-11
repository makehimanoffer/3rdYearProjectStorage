
// Direct3DHandler.cpp

#include "Direct3DHandler.h"


Direct3DHandler::Direct3DHandler()
{
	_swapChain = 0;
	_device = 0;
	_deviceContext = 0;
	_renderTargetView = 0;
	_depthStencilBuffer = 0;
	_depthStencilState = 0;
	_depthStencilView = 0;
	_rasterState = 0;
	_rasterStateWireFrame=0;
	_rasterStateNoCulling = 0;
	_depthDisabledStencilState = 0;
	_alphaEnableBlendingState = 0;
	_alphaDisableBlendingState = 0;
	_alphaBlendState2 = 0;
}


Direct3DHandler::~Direct3DHandler()
{
}


bool Direct3DHandler::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, 
						  float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator, stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;


	// Store vsync setting.
	_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	// Use factory to create an adapter for primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		return false;
	}

	// Enumerate primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		return false;
	}

	// Get number of modes that fit DXGI_FORMAT_R8G8B8A8_UNORM display format for adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Create a list to hold all possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}

	// Now fill display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	// Now go through all display modes and find one that matches screen width and height.
	// When a match is found store numerator and denominator of refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return false;
	}

	// Store dedicated video card memory in megabytes.
	_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert name of video card to a character array and store it.
	error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}

	// Release display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release adapter output.
	adapterOutput->Release();
	adapterOutput = 0;


	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	// Initialize swap chain description.
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set width and height of back buffer.
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set refresh rate of back buffer.
	if(_vsync_enabled)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set usage of back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set handle for window to render to.
    swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set advanced flags.
	swapChainDesc.Flags = 0;

	// Set feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
										   D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);
	if(FAILED(result))
	{
		return false;
	}

	// Get pointer to back buffer.
	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Create render target view with back buffer pointer.
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Release pointer to back buffer as no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize description of depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up description of depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create texture for depth buffer using filled out description.
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize description of stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up description of stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state.
	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Set depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// Initialize depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create depth stencil view.
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Bind render target view and depth stencil buffer to output render pipeline.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// Setup raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create rasterizer state from description just filled out.
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if(FAILED(result))
	{
		return false;
	}

	

	// Now set rasterizer state.
	_deviceContext->RSSetState(_rasterState);
	
	// Setup a raster description which turns off back face culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create no culling rasterizer state.
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterStateNoCulling);
	if(FAILED(result))
	{
		return false;
	}
	//NEW IN TERRAIN 0.9
	// Setup a raster description which turns off back face culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create no culling rasterizer state.
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterStateWireFrame);
	if(FAILED(result))
	{
		return false;
	}

	// Setup viewport for rendering.
    _viewport.Width = (float)screenWidth;
    _viewport.Height = (float)screenHeight;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;

	

	// Create viewport.
    _deviceContext->RSSetViewports(1, &_viewport);

	// Setup projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&_projectionMx, fieldOfView, screenAspect, screenNear, screenDepth);

    // Initialize world matrix to identity matrix.
    D3DXMatrixIdentity(&_worldMx);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&_orthoMx, (float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Clear second depth stencil state before setting parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off Z buffer for 2D rendering.  only difference is 
	// that DepthEnable is set to false, all other parameters are same as other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create state using device.
	result = _device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Clear blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create blend state using description.
	result = _device->CreateBlendState(&blendStateDescription, &_alphaEnableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	// Modify description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create second blend state using description.
	result = _device->CreateBlendState(&blendStateDescription, &_alphaDisableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	// Create a secondary alpha blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create blend state using description.
	result = _device->CreateBlendState(&blendStateDescription, &_alphaBlendState2);
	if(FAILED(result))
	{
		return false;
	}

    return true;
}


void Direct3DHandler::KillSafely()
{

	//NEW FOR TERRAIN 0.8
	if(_rasterStateWireFrame)
	{
		_rasterStateWireFrame->Release();
		_rasterStateWireFrame = 0;
	}


	// Before shutting down set to windowed mode or when you release swap chain it will throw an exception.
	if(_swapChain)
	{
		_swapChain->SetFullscreenState(false, NULL);
	}

	if(_alphaBlendState2)
	{
		_alphaBlendState2->Release();
		_alphaBlendState2 = 0;
	}

	if(_alphaEnableBlendingState)
	{
		_alphaEnableBlendingState->Release();
		_alphaEnableBlendingState = 0;
	}

	if(_alphaDisableBlendingState)
	{
		_alphaDisableBlendingState->Release();
		_alphaDisableBlendingState = 0;
	}

	if(_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = 0;
	}

	if(_rasterStateNoCulling)
	{
		_rasterStateNoCulling->Release();
		_rasterStateNoCulling = 0;
	}

	if(_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if(_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if(_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if(_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if(_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if(_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = 0;
	}

	if(_device)
	{
		_device->Release();
		_device = 0;
	}

	if(_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}

	return;
}


void Direct3DHandler::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup color to clear buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear back buffer.
	_deviceContext->ClearRenderTargetView(_renderTargetView, color);
    
	// Clear depth buffer.
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void Direct3DHandler::EndScene()
{
	// Present back buffer to screen since rendering is complete.
	if(_vsync_enabled)
	{
		// Lock to screen refresh rate.
		_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		_swapChain->Present(0, 0);
	}

	return;
}


ID3D11Device* Direct3DHandler::GetDevice()
{
	return _device;
}


ID3D11DeviceContext* Direct3DHandler::GetDeviceContext()
{
	return _deviceContext;
}


void Direct3DHandler::GetProjectionMx(D3DXMATRIX& projectionMx)
{
	projectionMx = _projectionMx;
	return;
}


void Direct3DHandler::GetWorldMx(D3DXMATRIX& worldMx)
{
	worldMx = _worldMx;
	return;
}


void Direct3DHandler::GetOrthoMx(D3DXMATRIX& orthoMx)
{
	orthoMx = _orthoMx;
	return;
}


void Direct3DHandler::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, _videoCardDescription);
	memory = _videoCardMemory;
	return;
}


void Direct3DHandler::TurnZBufferOn()
{
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
	return;
}


void Direct3DHandler::TurnZBufferOff()
{
	_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState, 1);
	return;
}


void Direct3DHandler::TurnOnAlphaBlending()
{
	float blendFactor[4];
	

	// Setup blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on alpha blending.
	_deviceContext->OMSetBlendState(_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}


void Direct3DHandler::TurnOffAlphaBlending()
{
	float blendFactor[4];
	

	// Setup blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn off alpha blending.
	_deviceContext->OMSetBlendState(_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}


void Direct3DHandler::TurnOnCulling()
{
	// Set culling rasterizer state.
	_deviceContext->RSSetState(_rasterState);

	return;
}


void Direct3DHandler::TurnOffCulling()
{
	// Set no back face culling rasterizer state.
	_deviceContext->RSSetState(_rasterStateNoCulling);

	return;
}


void Direct3DHandler::SetBackBufferRenderTarget()
{
	// Bind render target view and depth stencil buffer to output render pipeline.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	return;
}


void Direct3DHandler::ResetViewport()
{
	// Set viewport.
    _deviceContext->RSSetViewports(1, &_viewport);

	return;
}


void Direct3DHandler::EnableSecondBlendState()
{
	float blendFactor[4];
	

	// Setup blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on alpha blending.
	_deviceContext->OMSetBlendState(_alphaBlendState2, blendFactor, 0xffffffff);

	return;
}

void Direct3DHandler::TurnOnWireUpdate(){
	_deviceContext->RSSetState(_rasterStateWireFrame);
}

void Direct3DHandler::TurnOffWireUpdate(){
	_deviceContext->RSSetState(_rasterState);
}