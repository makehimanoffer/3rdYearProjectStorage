
#include "Application.h"


Application::Application()
{
	m_Input = 0;
	m_Direct3D = 0;
	m_Timer = 0;
	m_Position = 0;
	m_Camera = 0;
	m_Light = 0;
	m_Terrain = 0;
	m_TerrainShader = 0;
	m_SkyDome = 0;
	m_SkyDomeShader = 0;
	m_SkyPlane = 0;
	m_SkyPlaneShader = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	m_RefractionTexture = 0;
	m_ReflectionTexture = 0;
	m_ReflectionShader = 0;
	m_Water = 0;
	m_WaterShader = 0;
}


Application::Application(const Application& other)
{
}


Application::~Application()
{
}


bool Application::Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;
	D3DXMATRIX baseViewMatrix;
	char videoCard[128];
	int videoMemory;

	
	// Create the input object.  The input object will be used to handle reading the keyboard and mouse input from the user.
	m_Input = new Input;
	if(!m_Input)
	{
		return false;
	}

	// Init input object.
	result = m_Input->Init(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the input object.", L"Error", MB_OK);
		return false;
	}

	// Create Direct3D object.
	m_Direct3D = new Direct3DHandler;
	if(!m_Direct3D)
	{
		return false;
	}

	// Init Direct3D object.
	result = m_Direct3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init DirectX 11.", L"Error", MB_OK);
		return false;
	}

	 //get video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Create  timer object.
	m_Timer = new Timer;
	if(!m_Timer)
	{
		return false;
	}

	// Init timer object.
	result = m_Timer->Init();
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new Position;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position and rotation of the viewer.
	m_Position->SetPosition(280.379f, 24.5225f, 367.018f);
	m_Position->SetRotation(19.6834f, 222.013f, 0.0f);
	
	// Create the camera object.
	m_Camera = new Camera;
	if(!m_Camera)
	{
		return false;
	}

	// Initi  base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->GenerateBaseViewMatrix();
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Create the light object.
	m_Light = new Light;
	if(!m_Light)
	{
		return false;
	}

	// Init the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.75f, 0.25f);
	
	// Create the terrain object.
	m_Terrain = new Terrain;
	if(!m_Terrain)
	{
		return false;
	}

	// Init the terrain object.
	result = m_Terrain->Init(m_Direct3D->GetDevice(), "../heightmapWater.bmp", "../cm.bmp", 20.0f, L"../matrix.dds", 
				       L"../normal.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShader;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Init the terrain shader object.
	result = m_TerrainShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome object.
	m_SkyDome = new SkyDome;
	if(!m_SkyDome)
	{
		return false;
	}

	// Init the sky dome object.
	result = m_SkyDome->Init(m_Direct3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the sky dome object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky dome shader object.
	m_SkyDomeShader = new SkyDomeShader;
	if(!m_SkyDomeShader)
	{
		return false;
	}

	// Init the sky dome shader object.
	result = m_SkyDomeShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the sky dome shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane object.
	m_SkyPlane = new Skyplane;
	if(!m_SkyPlane)
	{
		return false;
	}

	// Init the sky plane object.
	result = m_SkyPlane->Init(m_Direct3D->GetDevice(), L"../pinkcloudMatrix.dds", L"../invertcloudMatrix.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the sky plane object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky plane shader object.
	m_SkyPlaneShader = new SkyplaneShader;
	if(!m_SkyPlaneShader)
	{
		return false;
	}

	// Init the sky plane shader object.
	result = m_SkyPlaneShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the sky plane shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the fps object.
	m_Fps = new FramesPS;
	if(!m_Fps)
	{
		return false;
	}

	// Init the fps object.
	m_Fps->Init();

	// Create the cpu object.
	m_Cpu = new CPUQuery;
	if(!m_Cpu)
	{
		return false;
	}

	// Init the cpu object.
	m_Cpu->Init();

	// Create the font shader object.
	m_FontShader = new FontShader;
	if(!m_FontShader)
	{
		return false;
	}

	// Init the font shader object.
	result = m_FontShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextHandler;
	if(!m_Text)
	{
		return false;
	}

	// Init the text object.
	result = m_Text->Init(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the text object.", L"Error", MB_OK);
		return false;
	}

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTexture;
	if(!m_RefractionTexture)
	{
		return false;
	}

	// Init the refraction render to texture object.
	result = m_RefractionTexture->Init(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTexture;
	if(!m_ReflectionTexture)
	{
		return false;
	}

	// Init the reflection render to texture object.
	result = m_ReflectionTexture->Init(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection shader object.
	m_ReflectionShader = new ReflectionShader;
	if(!m_ReflectionShader)
	{
		return false;
	}

	// Init the reflection shader object.
	result = m_ReflectionShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the reflection shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water object.
	m_Water = new Water;
	if(!m_Water)
	{
		return false;
	}

	// Init the water object.
	result = m_Water->Init(m_Direct3D->GetDevice(), L"../waternormal.dds", 3.75f, 110.0f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the water object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShader;
	if(!m_WaterShader)
	{
		return false;
	}

	// Init the water shader object.
	result = m_WaterShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not Init the water shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Application::KillSafely()
{
	// Release the water shader object.
	if(m_WaterShader)
	{
		m_WaterShader->KillSafely();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the water object.
	if(m_Water)
	{
		m_Water->KillSafely();
		delete m_Water;
		m_Water = 0;
	}

	// Release the reflection shader object.
	if(m_ReflectionShader)
	{
		m_ReflectionShader->KillSafely();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the reflection render to texture object.
	if(m_ReflectionTexture)
	{
		m_ReflectionTexture->KillSafely();
		delete m_ReflectionTexture;
		m_ReflectionTexture = 0;
	}

	// Release the refraction render to texture object.
	if(m_RefractionTexture)
	{
		m_RefractionTexture->KillSafely();
		delete m_RefractionTexture;
		m_RefractionTexture = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->KillSafely();
		delete m_Text;
		m_Text = 0;
	}

	// Release the font shader object.
	if(m_FontShader)
	{
		m_FontShader->KillSafely();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->KillSafely();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}
		
	// Release the sky plane shader object.
	if(m_SkyPlaneShader)
	{
		m_SkyPlaneShader->KillSafely();
		delete m_SkyPlaneShader;
		m_SkyPlaneShader = 0;
	}

	// Release the sky plane object.
	if(m_SkyPlane)
	{
		m_SkyPlane->KillSafely();
		delete m_SkyPlane;
		m_SkyPlane = 0;
	}

	// Release the sky dome shader object.
	if(m_SkyDomeShader)
	{
		m_SkyDomeShader->KillSafely();
		delete m_SkyDomeShader;
		m_SkyDomeShader = 0;
	}

	// Release the sky dome object.
	if(m_SkyDome)
	{
		m_SkyDome->KillSafely();
		delete m_SkyDome;
		m_SkyDome = 0;
	}

	// Release the terrain shader object.
	if(m_TerrainShader)
	{
		m_TerrainShader->KillSafely();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}
	
	// Release the terrain object.
	if(m_Terrain)
	{
		m_Terrain->KillSafely();
		delete m_Terrain;
		m_Terrain = 0;
	}
	
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	
	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}

	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->KillSafely();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		m_Input->KillSafely();
		delete m_Input;
		m_Input = 0;
	}

	return;
}


bool Application::Update()
{
	bool result;


	// Update the system stats.
	m_Timer->Update();
	m_Fps->Update();
	m_Cpu->Update();

	// Read the user input.
	result = m_Input->Update();
	if(!result)
	{
		return false;
	}
	
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsEscapePressed() == true)
	{
		return false;
	}

	// Do the frame input processing.
	result = HandleMovementInput(m_Timer->GetTime());
	if(!result)
	{
		return false;
	}

	// Update the FPS value in the text object.
	result = m_Text->SetFps(m_Fps->GetFps(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}
	
	// Update the CPU usage value in the text object.
	result = m_Text->SetCpu(m_Cpu->GetCpuPercentage(), m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Do the water frame processing.
	m_Water->Update();

	// Do the sky plane frame processing.
	m_SkyPlane->Update();

	// Render the refraction of the scene to a texture.
	RenderRefractionToTexture();

	// Render the reflection of the scene to a texture.
	RenderReflectionToTexture();

	// Render the graphics.
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


	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(frameTime);

	// Handle the input.
	keyDown = m_Input->IsAPressed();
	m_Position->GoLeft(keyDown);

	keyDown = m_Input->IsDPressed();
	m_Position->GoRight(keyDown);

	keyDown = m_Input->IsWPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsSPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->IsPgUpPressed();
	m_Position->MoveDownward(keyDown);

	keyDown=m_Input->Is1Pressed();
	m_Position->MoveDownward(keyDown);	
	
	keyDown=m_Input->Is2Pressed();
	m_Position->MoveUpward(keyDown);
		

	if(keyDown=m_Input->Is3Pressed()){
		m_Water->m_normalMapTiling.x=0.1f;
		m_Water->m_normalMapTiling.y=0.2f;
		m_Water->m_reflectRefractScale = 0.3f;
	}
	if(keyDown=m_Input->Is4Pressed()){
		m_Water->m_normalMapTiling.x=0.2f;
		m_Water->m_normalMapTiling.y=0.4f;
		m_Water->m_reflectRefractScale = 0.6f;
	}

	if(keyDown=m_Input->Is5Pressed()){
		m_Water->m_normalMapTiling.x=0.01f;
		m_Water->m_normalMapTiling.y=0.02f;
		m_Water->m_reflectRefractScale = 0.03f;
	}
	if(keyDown=m_Input->Is6Pressed()){
		m_Water->m_normalMapTiling.x=0.02f;
		m_Water->m_normalMapTiling.y=0.04f;
		m_Water->m_reflectRefractScale = 0.06f;
	}

	

	if(keyDown=m_Input->IsPPressed()){
		m_Position->SetPosition(262,5,348);
	}


	// Get the view point position/rotation.
	m_Position->GetPosition(posX, posY, posZ);
	m_Position->GetRotation(rotX, rotY, rotZ);

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	

	
	m_Camera->SetRotation(m_Input->m_mouseX, m_Input->m_mouseY, rotZ);

	// Update the position values in the text object.
	result = m_Text->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Text->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	return true;
}

void Application::RenderRefractionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;


	// Setup a clipping plane based on the height of the water to clip everything above it to create a refraction.
	clipPlane = D3DXVECTOR4(0.0f, -1.0f, 0.0f, m_Water->GetWaterHeight() + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Render the terrain using the reflection shader and the refraction clip plane to produce the refraction effect.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());
	m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				   m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 2.0f, 
				   clipPlane);
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return;
}

void Application::RenderReflectionToTexture()
{
	D3DXVECTOR4 clipPlane;
	D3DXMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	D3DXVECTOR3 cameraPosition;


	// Setup a clipping plane based on the height of the water to clip everything below it.
	clipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -m_Water->GetWaterHeight());
	
	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_Direct3D->GetDeviceContext());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_Water->GetWaterHeight());

	// Get the camera reflection view matrix instead of the normal view matrix.
	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	// Get the world and projection matrices from the d3d object.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Invert the Y coordinate of the camera around the water plane height for the reflected camera position.
	cameraPosition.y = -cameraPosition.y + (m_Water->GetWaterHeight() * 2.0f);

	// Translate the sky dome and sky plane to be centered around the reflected camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and the Z buffer.
	m_Direct3D->TurnOffCulling();
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the reflection view matrix.
	m_SkyDome->Render(m_Direct3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, 
				m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Enable back face culling.
	m_Direct3D->TurnOnCulling();

	// Enable additive blending so the clouds blend with the sky dome color.
	m_Direct3D->EnableSecondBlendState();

	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_Direct3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, 
				 m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(), 
				 m_SkyPlane->GetBrightness());

	// Turn off blending and enable the Z buffer again.
	m_Direct3D->TurnOffAlphaBlending();
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain using the reflection view matrix and reflection clip plane.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());
	m_ReflectionShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix, 
				   m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 2.0f, 
				   clipPlane);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_Direct3D->ResetViewport();

	return;
}

bool Application::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix, reflectionViewMatrix;
	bool result;
	D3DXVECTOR3 cameraPosition;


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Generate the reflection matrix based on the camera's position and the height of the water.
	m_Camera->RenderReflection(m_Water->GetWaterHeight());

	// Get the world, view, projection, ortho, and base view matrices from the camera and Direct3D objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);
	m_Camera->GetBaseViewMatrix(baseViewMatrix);
	m_Camera->GetReflectionViewMatrix(reflectionViewMatrix);

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Translate the sky dome to be centered around the camera position.
	D3DXMatrixTranslation(&worldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and the Z buffer.
	m_Direct3D->TurnOffCulling();
	m_Direct3D->TurnZBufferOff();

	// Render the sky dome using the sky dome shader.
	m_SkyDome->Render(m_Direct3D->GetDeviceContext());
	m_SkyDomeShader->Render(m_Direct3D->GetDeviceContext(), m_SkyDome->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				m_SkyDome->GetApexColor(), m_SkyDome->GetCenterColor());

	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();

	// Enable additive blending so the clouds blend with the sky dome color.
	m_Direct3D->EnableSecondBlendState();

	// Render the sky plane using the sky plane shader.
	m_SkyPlane->Render(m_Direct3D->GetDeviceContext());
	m_SkyPlaneShader->Render(m_Direct3D->GetDeviceContext(), m_SkyPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				 m_SkyPlane->GetCloudTexture(), m_SkyPlane->GetPerturbTexture(), m_SkyPlane->GetTranslation(), m_SkyPlane->GetScale(), 
				 m_SkyPlane->GetBrightness());

	// Turn off blending.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain using the terrain shader.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
					 m_Terrain->GetColorTexture(), m_Terrain->GetNormalTexture(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), 
					 2.0f);
	if(!result)
	{
		return false;
	}

	// Translate to the location of the water and render it.
	D3DXMatrixTranslation(&worldMatrix, 240.0f, m_Water->GetWaterHeight(), 250.0f);
	m_Water->Render(m_Direct3D->GetDeviceContext());
	m_WaterShader->Render(m_Direct3D->GetDeviceContext(), m_Water->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix, 
			      m_RefractionTexture->GetShaderResourceView(), m_ReflectionTexture->GetShaderResourceView(), m_Water->GetTexture(), 
			      m_Camera->GetPosition(), m_Water->GetNormalMapTiling(), m_Water->GetWaterTranslation(), m_Water->GetReflectRefractScale(),
			      m_Water->GetRefractionTint(), m_Light->GetDirection(), m_Water->GetSpecularShininess());

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	// Render the text user interface elements.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), m_FontShader, worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}