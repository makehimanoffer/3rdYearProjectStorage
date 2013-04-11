
#include "RenderTexture.h"


RenderTexture::RenderTexture()
{
	_renderTargetTexture = 0;
	_renderTargetView = 0;
	_shaderResourceView = 0;
	_depthStencilBuffer = 0;
	_depthStencilView = 0;
}




RenderTexture::~RenderTexture()
{
}


bool RenderTexture::Init(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;


	// Init render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

	// Create render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, &_renderTargetTexture);
	if(FAILED(result))
	{
		return false;
	}

	// Setup description of render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create render target view.
	result = device->CreateRenderTargetView(_renderTargetTexture, &renderTargetViewDesc, &_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup description of shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create shader resource view.
	result = device->CreateShaderResourceView(_renderTargetTexture, &shaderResourceViewDesc, &_shaderResourceView);
	if(FAILED(result))
	{
		return false;
	}

	// Init description of depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up description of depth buffer.
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
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
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initailze depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create depth stencil view.
	result = device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Setup viewport for rendering.
    _viewport.Width = (float)textureWidth;
    _viewport.Height = (float)textureHeight;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;

	// Setup projection matrix.
	D3DXMatrixPerspectiveFovLH(&_projectionMx, ((float)D3DX_PI / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&_orthoMx, (float)textureWidth, (float)textureHeight, screenNear, screenDepth);

	return true;
}


void RenderTexture::KillSafely()
{
	if(_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if(_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if(_shaderResourceView)
	{
		_shaderResourceView->Release();
		_shaderResourceView = 0;
	}

	if(_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if(_renderTargetTexture)
	{
		_renderTargetTexture->Release();
		_renderTargetTexture = 0;
	}

	return;
}


void RenderTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	// Bind render target view and depth stencil buffer to output render pipeline.
	deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
	
	// Set viewport.
	deviceContext->RSSetViewports(1, &_viewport);
	
	return;
}


void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha)
{
	float color[4];


	// Setup color to clear buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear back buffer.
	deviceContext->ClearRenderTargetView(_renderTargetView, color);
    
	// Clear depth buffer.
	deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


ID3D11ShaderResourceView* RenderTexture::GetShaderResourceView()
{
	return _shaderResourceView;
}


void RenderTexture::GetProjectionMx(D3DXMATRIX& projectionMx)
{
	projectionMx = _projectionMx;
	return;
}


void RenderTexture::GetOrthoMx(D3DXMATRIX& orthoMx)
{
	orthoMx = _orthoMx;
	return;
}