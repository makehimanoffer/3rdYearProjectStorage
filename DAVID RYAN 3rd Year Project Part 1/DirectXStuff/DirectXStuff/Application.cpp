
#include "Application.h"


Application::Application()
{
	_Input = 0;
	_Direct3D = 0;
	_Timer = 0;
	_Position = 0;
	_Camera = 0;
	_Light = 0;
	_Terrain = 0;
	_TerrainShader = 0;
	_SkyBox = 0;
	_SkyBoxShader = 0;
	_SkyPlane = 0;
	_SkyPlaneShader = 0;
	_Fps = 0;
	_Cpu = 0;
	_FontShader = 0;
	_Text = 0;
	_RefractTexture = 0;
	_ReflTexture = 0;
	_ReflectionShader = 0;
	_Water = 0;
	_WaterShader = 0;
}





Application::~Application()
{
}


bool Application::Init(HINSTANCE hinstance, HWND hwnd, int scrWidth, int scrHeight)
{
	bool result;
	D3DXMATRIX baseViewMx;
	char videoCard[128];
	int videoMemory;

	
	// input object will be used to handle reading keyboard and mouse input from user.
	_Input = new Input;
	
	// Init input object.
	result = _Input->Init(hinstance, hwnd, scrWidth, scrHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init input object.", L"Error", MB_OK);
		return false;
	}

	_Direct3D = new Direct3DHandler;
	

	// Init Direct3D object.
	result = _Direct3D->Init(scrWidth, scrHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init DirectX 11.", L"Error", MB_OK);
		return false;
	}

	 //get video card information.
	_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	_Timer = new Timer;
	

	// Init timer object.
	result = _Timer->Init();
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init timer object.", L"Error", MB_OK);
		return false;
	}

	_Position = new Position;
	
	// Set initial position and rotation of viewer.
	_Position->SetPosition(280.379f, 24.5225f, 367.018f);
	_Position->SetRotation(19.6834f, 222.013f, 0.0f);
	
	_Camera = new Camera;
	

	// Initi  base view matrix with camera for 2D user interface rendering.
	_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	_Camera->GenerateBaseViewMx();
	_Camera->GetBaseViewMx(baseViewMx);

	_Light = new Light;
	

	// Init light object.
	_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_Light->SetDirection(0.5f, -0.75f, 0.25f);
	
	_Terrain = new Terrain;
	

	// Init terrain object.
	result = _Terrain->Init(_Direct3D->GetDevice(), "../heightmapWater.bmp", "../cm.bmp", 20.0f, L"../matrix.dds", 
				       L"../normal.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init terrain object.", L"Error", MB_OK);
		return false;
	}

	_TerrainShader = new TerrainShader;
	

	// Init terrain shader object.
	result = _TerrainShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init terrain shader object.", L"Error", MB_OK);
		return false;
	}

	_SkyBox = new SkyBox;
	

	// Init sky box object.
	result = _SkyBox->Init(_Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init sky box object.", L"Error", MB_OK);
		return false;
	}

	_SkyBoxShader = new SkyBoxShader;
	

	// Init sky box shader object.
	result = _SkyBoxShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init sky box shader object.", L"Error", MB_OK);
		return false;
	}


	_SkyPlane = new Skyplane;

	// Init sky plane object.
	result = _SkyPlane->Init(_Direct3D->GetDevice(), L"../pinkcloudMatrix.dds", L"../invertcloudMatrix.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init sky plane object.", L"Error", MB_OK);
		return false;
	}

	_SkyPlaneShader = new SkyplaneShader;
	

	// Init sky plane shader object.
	result = _SkyPlaneShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init sky plane shader object.", L"Error", MB_OK);
		return false;
	}
	
	
	_Fps = new FramesPS;
	

	// Init fps object.
	_Fps->Init();

	_Cpu = new CPUQuery;
	
	// Init cpu object.
	_Cpu->Init();

	_FontShader = new FontShader;
	

	// Init font shader object.
	result = _FontShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init font shader object.", L"Error", MB_OK);
		return false;
	}

	_Text = new TextHandler;
	
	// Init text object.
	result = _Text->Init(_Direct3D->GetDevice(), _Direct3D->GetDeviceContext(), hwnd, scrWidth, scrHeight, baseViewMx);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init text object.", L"Error", MB_OK);
		return false;
	}

	// Set video card information in text object.
	result = _Text->SetVideoCardInfo(videoCard, videoMemory, _Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in text object.", L"Error", MB_OK);
		return false;
	}

	_RefractTexture = new RenderTexture;
	
	// Init refraction render to texture object.
	result = _RefractTexture->Init(_Direct3D->GetDevice(), scrWidth, scrHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	_ReflTexture = new RenderTexture;

	// Init reflection render to texture object.
	result = _ReflTexture->Init(_Direct3D->GetDevice(), scrWidth, scrHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	_ReflectionShader = new ReflectionShader;
	

	// Init reflection shader object.
	result = _ReflectionShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init reflection shader object.", L"Error", MB_OK);
		return false;
	}

	_Water = new Water;

	// Init water object.
	result = _Water->Init(_Direct3D->GetDevice(), L"../waternormal.dds", 3.75f, 110.0f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init water object.", L"Error", MB_OK);
		return false;
	}

	_WaterShader = new WaterShader;
	if(!_WaterShader)
	{
		return false;
	}

	// Init water shader object.
	result = _WaterShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init water shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Application::KillSafely()
{
	if(_WaterShader)
	{
		_WaterShader->KillSafely();
		delete _WaterShader;
		_WaterShader = 0;
	}

	if(_Water)
	{
		_Water->KillSafely();
		delete _Water;
		_Water = 0;
	}

	if(_ReflectionShader)
	{
		_ReflectionShader->KillSafely();
		delete _ReflectionShader;
		_ReflectionShader = 0;
	}

	if(_ReflTexture)
	{
		_ReflTexture->KillSafely();
		delete _ReflTexture;
		_ReflTexture = 0;
	}

	if(_RefractTexture)
	{
		_RefractTexture->KillSafely();
		delete _RefractTexture;
		_RefractTexture = 0;
	}

	if(_Text)
	{
		_Text->KillSafely();
		delete _Text;
		_Text = 0;
	}

	if(_FontShader)
	{
		_FontShader->KillSafely();
		delete _FontShader;
		_FontShader = 0;
	}

	if(_Cpu)
	{
		_Cpu->KillSafely();
		delete _Cpu;
		_Cpu = 0;
	}

	if(_Fps)
	{
		delete _Fps;
		_Fps = 0;
	}

	if(_SkyPlaneShader)
	{
		_SkyPlaneShader->KillSafely();
		delete _SkyPlaneShader;
		_SkyPlaneShader = 0;
	}

	if(_SkyPlane)
	{
		_SkyPlane->KillSafely();
		delete _SkyPlane;
		_SkyPlane = 0;
	}
	if(_SkyBoxShader)
	{
		_SkyBoxShader->KillSafely();
		delete _SkyBoxShader;
		_SkyBoxShader = 0;
	}

	if(_SkyBox)
	{
		_SkyBox->KillSafely();
		delete _SkyBox;
		_SkyBox = 0;
	}

	if(_TerrainShader)
	{
		_TerrainShader->KillSafely();
		delete _TerrainShader;
		_TerrainShader = 0;
	}

	if(_Terrain)
	{
		_Terrain->KillSafely();
		delete _Terrain;
		_Terrain = 0;
	}

	if(_Light)
	{
		delete _Light;
		_Light = 0;
	}

	if(_Camera)
	{
		delete _Camera;
		_Camera = 0;
	}

	if(_Position)
	{
		delete _Position;
		_Position = 0;
	}

	if(_Timer)
	{
		delete _Timer;
		_Timer = 0;
	}

	if(_Direct3D)
	{
		_Direct3D->KillSafely();
		delete _Direct3D;
		_Direct3D = 0;
	}

	if(_Input)
	{
		_Input->KillSafely();
		delete _Input;
		_Input = 0;
	}

	return;
}


bool Application::Update()
{
	bool result;


	// Update system stats.
	_Timer->Update();
	_Fps->Update();
	_Cpu->Update();

	// Read user input.
	result = _Input->Update();
	if(!result)
	{
		return false;
	}
	
	// Check if user pressed escape and wants to exit application.
	if(_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Do frame input processing.
	result = HandleControls(_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Update FPS value in text object.
	result = _Text->SetFps(_Fps->GetFps(), _Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update CPU usage value in text object.
	result = _Text->SetCpu(_Cpu->GetCpuPercentage(), _Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do water frame processing.
	_Water->Update();

	// Do sky plane frame processing.
	_SkyPlane->Update();

	// Render refraction of scene to a texture.
	RenderRefractionToTexture();

	// Render reflection of scene to a texture.
	RenderReflectionToTexture();

	// Render graphics.
	result = Render();
	if(!result)
	{
		return false;
	}

	return result;
}


bool Application::HandleControls(float frameTime)
{
	bool keyDown, result;
	float posX, posY, posZ, rotX, rotY, rotZ;


	// Set frame time for calculating updated position.
	_Position->SetFrameTime(frameTime);

	// Handle input.
	keyDown = _Input->IsAPressed();
	_Position->GoLeft(keyDown);

	keyDown = _Input->IsDPressed();
	_Position->GoRight(keyDown);

	keyDown = _Input->IsWPressed();
	_Position->MoveForward(keyDown);

	keyDown = _Input->IsSPressed();
	_Position->MoveBackward(keyDown);

	keyDown = _Input->IsPgUpPressed();
	_Position->MoveUpward(keyDown);

	keyDown = _Input->IsPgUpPressed();
	_Position->MoveDownward(keyDown);

	keyDown=_Input->Is1Pressed();
	_Position->MoveDownward(keyDown);	
	
	keyDown=_Input->Is2Pressed();
	_Position->MoveUpward(keyDown);
		

	if(keyDown=_Input->Is3Pressed()){
		_Water->_normalMapTiling.x=0.1f;
		_Water->_normalMapTiling.y=0.2f;
		_Water->_reflectRefractScale = 0.3f;
	}
	if(keyDown=_Input->Is4Pressed()){
		_Water->_normalMapTiling.x=0.2f;
		_Water->_normalMapTiling.y=0.4f;
		_Water->_reflectRefractScale = 0.6f;
	}

	if(keyDown=_Input->Is5Pressed()){
		_Water->_normalMapTiling.x=0.01f;
		_Water->_normalMapTiling.y=0.02f;
		_Water->_reflectRefractScale = 0.03f;
	}
	if(keyDown=_Input->Is6Pressed()){
		_Water->_normalMapTiling.x=0.02f;
		_Water->_normalMapTiling.y=0.04f;
		_Water->_reflectRefractScale = 0.06f;
	}

	

	if(keyDown=_Input->IsPPressed()){
		_Position->SetPosition(262,5,348);
	}


	// Get view point position/rotation.
	_Position->GetPosition(posX, posY, posZ);
	_Position->GetRotation(rotX, rotY, rotZ);

	// Set position of camera.
	_Camera->SetPosition(posX, posY, posZ);
	

	
	_Camera->SetRotation(_Input->_mouseX, _Input->_mouseY, rotZ);

	// Update position values in text object.
	result = _Text->SetCameraPosition(posX, posY, posZ, _Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update rotation values in text object.
	result = _Text->SetCameraRotation(rotX, rotY, rotZ, _Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	return true;
}

void Application::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMx, viewMx, projectionMx;


	// Setup a clipping plane based on height of water to clip everything above it to create a refraction.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, _Water->GetWaterHeight() + 0.1f);

	// Set render target to be refraction render to texture.
	_RefractTexture->SetRenderTarget(_Direct3D->GetDeviceContext());

	// Clear refraction render to texture.
	_RefractTexture->ClearRenderTarget(_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate view matrix based on camera's position.
	_Camera->Render();

	// Get matrices from camera and d3d objects.
	_Direct3D->GetWorldMx(worldMx);
	_Camera->GetViewMx(viewMx);
	_Direct3D->GetProjectionMx(projectionMx);

	// Render terrain using reflection shader and refraction clip plane to produce refraction effect.
	_Terrain->Render(_Direct3D->GetDeviceContext());
	_ReflectionShader->Render(_Direct3D->GetDeviceContext(), _Terrain->GetIndexCount(), worldMx, viewMx, projectionMx, 
				   _Terrain->GetColorTexture(), _Terrain->GetNormalTexture(), _Light->GetDiffuseColor(), _Light->GetDirection(), 2.0f, 
				   clipPlane);
	
	// Reset render target back to original back buffer and not render to texture anymore.
	_Direct3D->SetBackBufferRenderTarget();

	// Reset viewport back to original.
	_Direct3D->ResetViewport();

	return;
}

void Application::RenderReflectionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX reflectionViewMx, worldMx, projectionMx;
	D3DXVECTOR3 cameraPosition;


	// Setup a clipping plane based on height of water to clip everything below it.
	clipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -_Water->GetWaterHeight());
	
	// Set render target to be reflection render to texture.
	_ReflTexture->SetRenderTarget(_Direct3D->GetDeviceContext());

	// Clear reflection render to texture.
	_ReflTexture->ClearRenderTarget(_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use camera to render reflection and create a reflection view matrix.
	_Camera->RenderReflection(_Water->GetWaterHeight());

	// Get camera reflection view matrix instead of normal view matrix.
	_Camera->GetReflectionViewMx(reflectionViewMx);

	// Get world and projection matrices from d3d object.
	_Direct3D->GetWorldMx(worldMx);
	_Direct3D->GetProjectionMx(projectionMx);

	// Get position of camera.
	cameraPosition = _Camera->GetPosition();

	// Invert Y coordinate of camera around water plane height for reflected camera position.
	cameraPosition.y = -cameraPosition.y + (_Water->GetWaterHeight() * 2.0f);

	// Translate sky box and sky plane to be centered around reflected camera position.
	D3DXMatrixTranslation(&worldMx, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and Z buffer.
	_Direct3D->TurnOffCulling();
	_Direct3D->TurnZBufferOff();

	// Render sky box using reflection view matrix.
	_SkyBox->Render(_Direct3D->GetDeviceContext());
	_SkyBoxShader->Render(_Direct3D->GetDeviceContext(), _SkyBox->GetIndexCount(), worldMx, reflectionViewMx, projectionMx, 
				_SkyBox->GetApexColor(), _SkyBox->GetCenterColor());

	// Enable back face culling.
	_Direct3D->TurnOnCulling();

	// Enable additive blending so clouds blend with sky box color.
	_Direct3D->EnableSecondBlendState();

	// Render sky plane using sky plane shader.
	_SkyPlane->Render(_Direct3D->GetDeviceContext());
	_SkyPlaneShader->Render(_Direct3D->GetDeviceContext(), _SkyPlane->GetIndexCount(), worldMx, reflectionViewMx, projectionMx, 
				 _SkyPlane->GetCloudTexture(), _SkyPlane->GetPerturbTexture(), _SkyPlane->GetTranslation(), _SkyPlane->GetScale(), 
				 _SkyPlane->GetBrightness());

	// Turn off blending and enable Z buffer again.
	_Direct3D->TurnOffAlphaBlending();
	_Direct3D->TurnZBufferOn();

	// Reset world matrix.
	_Direct3D->GetWorldMx(worldMx);

	// Render terrain using reflection view matrix and reflection clip plane.
	_Terrain->Render(_Direct3D->GetDeviceContext());
	_ReflectionShader->Render(_Direct3D->GetDeviceContext(), _Terrain->GetIndexCount(), worldMx, reflectionViewMx, projectionMx, 
				   _Terrain->GetColorTexture(), _Terrain->GetNormalTexture(), _Light->GetDiffuseColor(), _Light->GetDirection(), 2.0f, 
				   clipPlane);

	// Reset render target back to original back buffer and not render to texture anymore.
	_Direct3D->SetBackBufferRenderTarget();

	// Reset viewport back to original.
	_Direct3D->ResetViewport();

	return;
}

bool Application::Render()
{
	D3DXMATRIX worldMx, viewMx, projectionMx, orthoMx, baseViewMx, reflectionViewMx;
	bool result;
	D3DXVECTOR3 cameraPosition;


	// Clear scene.
	_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate view matrix based on camera's position.
	_Camera->Render();

	// Generate reflection matrix based on camera's position and height of water.
	_Camera->RenderReflection(_Water->GetWaterHeight());

	// Get world, view, projection, ortho, and base view matrices from camera and Direct3D objects.
	_Direct3D->GetWorldMx(worldMx);
	_Camera->GetViewMx(viewMx);
	_Direct3D->GetProjectionMx(projectionMx);
	_Direct3D->GetOrthoMx(orthoMx);
	_Camera->GetBaseViewMx(baseViewMx);
	_Camera->GetReflectionViewMx(reflectionViewMx);

	// Get position of camera.
	cameraPosition = _Camera->GetPosition();

	// Translate sky box to be centered around camera position.
	D3DXMatrixTranslation(&worldMx, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and Z buffer.
	_Direct3D->TurnOffCulling();
	_Direct3D->TurnZBufferOff();

	// Render sky box using sky box shader.
	_SkyBox->Render(_Direct3D->GetDeviceContext());
	_SkyBoxShader->Render(_Direct3D->GetDeviceContext(), _SkyBox->GetIndexCount(), worldMx, viewMx, projectionMx, 
				_SkyBox->GetApexColor(), _SkyBox->GetCenterColor());

	// Turn back face culling back on.
	_Direct3D->TurnOnCulling();

	// Enable additive blending so clouds blend with sky box color.
	_Direct3D->EnableSecondBlendState();

	// Render sky plane using sky plane shader.
	_SkyPlane->Render(_Direct3D->GetDeviceContext());
	_SkyPlaneShader->Render(_Direct3D->GetDeviceContext(), _SkyPlane->GetIndexCount(), worldMx, viewMx, projectionMx, 
				 _SkyPlane->GetCloudTexture(), _SkyPlane->GetPerturbTexture(), _SkyPlane->GetTranslation(), _SkyPlane->GetScale(), 
				 _SkyPlane->GetBrightness());

	// Turn off blending.
	_Direct3D->TurnOffAlphaBlending();

	// Turn Z buffer back on.
	_Direct3D->TurnZBufferOn();

	// Reset world matrix.
	_Direct3D->GetWorldMx(worldMx);

	// Render terrain using terrain shader.
	_Terrain->Render(_Direct3D->GetDeviceContext());
	result = _TerrainShader->Render(_Direct3D->GetDeviceContext(), _Terrain->GetIndexCount(), worldMx, viewMx, projectionMx, 
					 _Terrain->GetColorTexture(), _Terrain->GetNormalTexture(), _Light->GetDiffuseColor(), _Light->GetDirection(), 
					 2.0f);
	if(!result)
	{
		return false;
	}

	// Translate to location of water and render it.
	D3DXMatrixTranslation(&worldMx, 240.0f, _Water->GetWaterHeight(), 250.0f);
	_Water->Render(_Direct3D->GetDeviceContext());
	_WaterShader->Render(_Direct3D->GetDeviceContext(), _Water->GetIndexCount(), worldMx, viewMx, projectionMx, reflectionViewMx, 
			      _RefractTexture->GetShaderResourceView(), _ReflTexture->GetShaderResourceView(), _Water->GetTexture(), 
			      _Camera->GetPosition(), _Water->GetNormalMapTiling(), _Water->GetWaterTranslation(), _Water->GetReflectRefractScale(),
			      _Water->GetRefractionTint(), _Light->GetDirection(), _Water->GetspecShine());

	// Reset world matrix.
	_Direct3D->GetWorldMx(worldMx);

	// Turn off Z buffer to begin all 2D rendering.
	_Direct3D->TurnZBufferOff();

	// Turn on alpha blending before rendering text.
	_Direct3D->TurnOnAlphaBlending();

	// Render text user interface elements.
	result = _Text->Render(_Direct3D->GetDeviceContext(), _FontShader, worldMx, orthoMx);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering text.
	_Direct3D->TurnOffAlphaBlending();

	// Turn Z buffer back on now that all 2D rendering has completed.
	_Direct3D->TurnZBufferOn();

	// Present rendered scene to screen.
	_Direct3D->EndScene();

	return true;
}