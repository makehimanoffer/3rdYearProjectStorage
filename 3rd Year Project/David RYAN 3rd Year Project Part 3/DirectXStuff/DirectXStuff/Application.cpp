
 

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
	
	
	_Fps = 0;
	_Cpu = 0;
	_FontShader = 0;
	_Text = 0;
	

	_Model=0;

	_Hud=0;
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

	
	// Create input object.  input object will be used to handle reading keyboard and mouse input from user.
	_Input = new Input;
	
	// Initialize input object.
	result = _Input->Init(hinstance, hwnd, scrWidth, scrHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize input object.", L"Error", MB_OK);
		return false;
	}

	// Create Direct3D object.
	_Direct3D = new Direct3DHandler;
	
	// Initialize Direct3D object.
	result = _Direct3D->Init(scrWidth, scrHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Retrieve video card information.
	_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	_Timer = new Timer;
	
	result = _Timer->Init();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize timer object.", L"Error", MB_OK);
		return false;
	}

	_Position = new Position;
	
	_Model = new Model;
	
	// NEW AND UPDATED IN TERR 0.6
	result = _Model->Init(_Direct3D->GetDevice(), "../MetalGearRex.txt", L"../tankTexture.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize model object.", L"Error", MB_OK);
		return false;
	}

	_Sound = new Sound;

	// NEW IN 0.14
	result = _Sound->Init(hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}
	_LightShader = new LightShader;

	result = _LightShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize light shader object.", L"Error", MB_OK);
		return false;
	}
	_TextureShader = new TextureShader;

	result = _TextureShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize texture shader object.", L"Error", MB_OK);
		return false;
	}

	_Hud = new Hud;

	result = _Hud->Init(_Direct3D->GetDevice(), scrWidth, scrHeight, L"../hud1.dds", 800, 600);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Hud object.", L"Error", MB_OK);
		return false;
	}

	// Set initial position and rotation of viewer.
	_Position->SetPosition(394, 27, 208);
	_Position->SetRotation(19.6834f, 222.013f, 0.0f);

	_Camera = new Camera;
	
	// Initialize a base view matrix with camera for 2D user interface rendering.
	_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	_Camera->GenerateBaseViewMatrix();
	_Camera->GetBaseViewMatrix(baseViewMx);

	_Light = new Light;

	_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	_Light->SetDirection(0.5f, -0.75f, 0.25f);
	_Light->SetDiffuseColor(1.0,1.0,1.0,1.0);

	_Terrain = new Terrain;

	result = _Terrain->Init(_Direct3D->GetDevice(), "../test1.bmp", L"../battlefield.dds", L"../bump.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize terrain object.", L"Error", MB_OK);
		return false;
	}
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize terrain object.", L"Error", MB_OK);
		return false;
	}

	_TerrainShader = new TerrainShader;

	result = _TerrainShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize terrain shader object.", L"Error", MB_OK);
		return false;
	}


	_Fps = new FramesPS;

	_Fps->Init();

	_Cpu = new CPUQuery;
	
	_Cpu->Init();

	_FontShader = new FontShader;

	result = _FontShader->Init(_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize font shader object.", L"Error", MB_OK);
		return false;
	}

	_Text = new TextHandler;

	result = _Text->Init(_Direct3D->GetDevice(), _Direct3D->GetDeviceContext(), hwnd, scrWidth, scrHeight, baseViewMx);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize text object.", L"Error", MB_OK);
		return false;
	}

	// Set video card information in text object.
	result = _Text->SetVideoCardInfo(videoCard, videoMemory, _Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in text object.", L"Error", MB_OK);
		return false;
	}

	

	

	return true;
}

void Application::KillSafely()
{
	if(_Sound)
	{
		_Sound->KillSafely();
		delete _Sound;
		_Sound = 0;
	}

	//brought back for 2d rendering
	if(_TextureShader)
	{
		_TextureShader->KillSafely();
		delete _TextureShader;
		_TextureShader = 0;
	}

	if(_Model)
	{
		_Model->KillSafely();
		delete _Model;
		_Model = 0;
	}
	if(_LightShader)
	{
		_LightShader->KillSafely();
		delete _LightShader;
		_LightShader = 0;
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

	//_Hud->UpdateBuffers(_Direct3D->GetDeviceContext(),_Camera->_posX,_Camera->_posY);
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
	result = HandleMovementInput(_Timer->GetTime());
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

	

	// Render graphics.
	result = Render();
	if(!result)
	{
		return false;
	}

	return result;
}


bool Application::HandleMovementInput(float frameTime)
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

	keyDown = _Input->IsSPressed();
	_Position->MoveForward(keyDown);

	keyDown = _Input->IsWPressed();
	_Position->MoveBackward(keyDown);

	keyDown = _Input->Is1Pressed();
	_Position->MoveUpward(keyDown);

	keyDown = _Input->Is2Pressed();
	_Position->MoveDownward(keyDown);

	if(keyDown=_Input->Is1Pressed()){
		_Direct3D->TurnOnWireFrame();

	}
	if(keyDown=_Input->Is2Pressed()){
		_Direct3D->TurnOffWireFrame();

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

	// Update position values in text object.
	result = _Hud->SetCameraPosition(posX, posY, posZ, _Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update rotation values in text object.
	result = _Hud->SetCameraRotation(rotX, rotY, rotZ, _Direct3D->GetDeviceContext());
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





bool Application::Render()
{
	D3DXMATRIX worldMx, viewMx, projectionMx, orthoMx, baseViewMx, reflectionViewMx;
	bool result;
	D3DXVECTOR3 cameraPosition;


	// Clear scene.
	_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate view matrix based on camera's position.
	_Camera->Render();

	
	// Get world, view, projection, ortho, and base view matrices from camera and Direct3D objects.
	_Direct3D->GetWorldMatrix(worldMx);
	_Camera->GetViewMatrix(viewMx);
	_Direct3D->GetProjectionMatrix(projectionMx);
	_Direct3D->GetOrthoMatrix(orthoMx);
	_Camera->GetBaseViewMatrix(baseViewMx);
	

	// Get position of camera.
	cameraPosition = _Camera->GetPosition();

	// Translate sky dome to be centered around camera position.
	D3DXMatrixTranslation(&worldMx, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and Z buffer.
	_Direct3D->TurnOffCulling();
	_Direct3D->TurnZBufferOff();

	

	// Turn back face culling back on.
	_Direct3D->TurnOnCulling();

	// Enable additive blending so clouds blend with sky dome color.
	_Direct3D->EnableSecondBlendState();

	

	// Turn off blending.
	_Direct3D->TurnOffAlphaBlending();

	// Turn Z buffer back on.
	_Direct3D->TurnZBufferOn();

	// Reset world matrix.
	_Direct3D->GetWorldMatrix(worldMx);

	// Render terrain using terrain shader.
	_Terrain->Render(_Direct3D->GetDeviceContext());
	result = _TerrainShader->Render(_Direct3D->GetDeviceContext(), _Terrain->GetIndexCount(), worldMx, viewMx, projectionMx, 
					 _Light->GetDiffuseColor(), _Light->GetDirection(), _Terrain->GetColorTexture(), _Terrain->GetNormalMapTexture());
	if(!result)
	{
		return false;
	}

	
	

	// Render terrain using quad tree and terrain shader. RENDER VISIBLE NODES
	_Model->Render( _Direct3D->GetDeviceContext());

	// Render model using light shader.
	result = _LightShader->Render(_Direct3D->GetDeviceContext(), _Model->GetIndexCount(), worldMx, viewMx, projectionMx, 
				       _Model->GetTexture(), _Light->GetDirection(), _Light->GetAmbientColor(), _Light->GetDiffuseColor());


	// Reset world matrix.
	_Direct3D->GetWorldMatrix(worldMx);

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