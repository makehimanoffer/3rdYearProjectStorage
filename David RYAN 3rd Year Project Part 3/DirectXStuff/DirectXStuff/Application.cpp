
 

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
	
	
	m_Fps = 0;
	m_Cpu = 0;
	m_FontShader = 0;
	m_Text = 0;
	

	m_Model=0;

	m_Hud=0;
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

	// Initialize the input object.
	result = m_Input->Init(hinstance, hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the Direct3D object.
	m_Direct3D = new Direct3DHandler;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	// Retrieve the video card information.
	m_Direct3D->GetVideoCardInfo(videoCard, videoMemory);

	// Create the timer object.
	m_Timer = new Timer;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Init();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object.
	m_Position = new Position;
	if(!m_Position)
	{
		return false;
	}
	// Create the terrain object.
	m_Model = new Model;
	if(!m_Model)
	{
		return false;
	}
	// Initialize the terrain object. NEW AND UPDATED IN TERR 0.6
	result = m_Model->Init(m_Direct3D->GetDevice(), "../MetalGearRex.txt", L"../tankTexture.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the sound object.
	m_Sound = new Sound;
	if(!m_Sound)
	{
		return false;
	}
 
	// Initialize the sound object.
	// NEW IN 0.14
	result = m_Sound->Init(hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}
	// Create the light shader object.
	m_LightShader = new LightShader;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the color shader object.
	result = m_LightShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	// Create the texture shader object.
	m_TextureShader = new TextureShader;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the Hud object.
	m_Hud = new Hud;
	if(!m_Hud)
	{
		return false;
	}

	// Initialize the Hud object.
	result = m_Hud->Init(m_Direct3D->GetDevice(), screenWidth, screenHeight, L"../hud1.dds", 800, 600);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the Hud object.", L"Error", MB_OK);
		return false;
	}

	// Set the initial position and rotation of the viewer.
	m_Position->SetPosition(394, 27, 208);
	m_Position->SetRotation(19.6834f, 222.013f, 0.0f);
	
	// Create the camera object.
	m_Camera = new Camera;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->GenerateBaseViewMatrix();
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	// Create the light object.
	m_Light = new Light;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.5f, -0.75f, 0.25f);
	m_Light->SetDiffuseColor(1.0,1.0,1.0,1.0);
	
	// Create the terrain object.
	m_Terrain = new Terrain;
	if(!m_Terrain)
	{
		return false;
	}

	// Initialize the terrain object.
	result = m_Terrain->Init(m_Direct3D->GetDevice(), "../test1.bmp", L"../battlefield.dds", L"../bump.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain object.", L"Error", MB_OK);
		return false;
	}

	// Create the terrain shader object.
	m_TerrainShader = new TerrainShader;
	if(!m_TerrainShader)
	{
		return false;
	}

	// Initialize the terrain shader object.
	result = m_TerrainShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	

	// Create the fps object.
	m_Fps = new FramesPS;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Init();

	// Create the cpu object.
	m_Cpu = new CPUQuery;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Init();

	// Create the font shader object.
	m_FontShader = new FontShader;
	if(!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Init(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextHandler;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Init(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Set the video card information in the text object.
	result = m_Text->SetVideoCardInfo(videoCard, videoMemory, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		MessageBox(hwnd, L"Could not set video card info in the text object.", L"Error", MB_OK);
		return false;
	}

	

	

	return true;
}

void Application::KillSafely()
{
	if(m_Sound)
	{
		m_Sound->KillSafely();
		delete m_Sound;
		m_Sound = 0;
	}

	// Release the texture shader object.
	//brought back for 2d rendering
	if(m_TextureShader)
	{
		m_TextureShader->KillSafely();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model
	if(m_Model)
	{
		m_Model->KillSafely();
		delete m_Model;
		m_Model = 0;
	}
	// Release the model
	if(m_LightShader)
	{
		m_LightShader->KillSafely();
		delete m_LightShader;
		m_LightShader = 0;
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


bool Application::Frame()
{
	bool result;


	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	//m_Hud->UpdateBuffers(m_Direct3D->GetDeviceContext(),m_Camera->m_positionX,m_Camera->m_positionY);
	// Read the user input.
	result = m_Input->Frame();
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

	keyDown = m_Input->IsSPressed();
	m_Position->MoveForward(keyDown);

	keyDown = m_Input->IsWPressed();
	m_Position->MoveBackward(keyDown);

	keyDown = m_Input->Is1Pressed();
	m_Position->MoveUpward(keyDown);

	keyDown = m_Input->Is2Pressed();
	m_Position->MoveDownward(keyDown);

	if(keyDown=m_Input->Is1Pressed()){
		m_Direct3D->TurnOnWireFrame();

	}
	if(keyDown=m_Input->Is2Pressed()){
		m_Direct3D->TurnOffWireFrame();

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

	// Update the position values in the text object.
	result = m_Hud->SetCameraPosition(posX, posY, posZ, m_Direct3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation values in the text object.
	result = m_Hud->SetCameraRotation(rotX, rotY, rotZ, m_Direct3D->GetDeviceContext());
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





bool Application::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix, reflectionViewMatrix;
	bool result;
	D3DXVECTOR3 cameraPosition;


	// Clear the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	
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

	

	// Turn back face culling back on.
	m_Direct3D->TurnOnCulling();

	// Enable additive blending so the clouds blend with the sky dome color.
	m_Direct3D->EnableSecondBlendState();

	

	// Turn off blending.
	m_Direct3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on.
	m_Direct3D->TurnZBufferOn();

	// Reset the world matrix.
	m_Direct3D->GetWorldMatrix(worldMatrix);

	// Render the terrain using the terrain shader.
	m_Terrain->Render(m_Direct3D->GetDeviceContext());
	result = m_TerrainShader->Render(m_Direct3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
					 m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Terrain->GetColorTexture(), m_Terrain->GetNormalMapTexture());
	if(!result)
	{
		return false;
	}

	
	

	// Render the terrain using the quad tree and terrain shader. RENDER VISIBLE NODES
	m_Model->Render( m_Direct3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				       m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());


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