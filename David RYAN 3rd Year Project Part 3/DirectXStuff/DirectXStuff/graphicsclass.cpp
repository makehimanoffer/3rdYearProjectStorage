////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	//m_TextureShader = 0;

	m_MultiTextureShader = 0;// NEW IN 0.17. replaces m_TextureShader as seen above in comments.
	m_Bitmap = 0;// new for 0.10

	m_Text = 0; // new for 0.12

	m_ModelList = 0; // NEW FOR 0.16
	m_Frustum = 0; // NEW FOR 0.16
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new Direct3DHandler;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	
	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_Model = new Model;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "../sphere.txt", L"../basetexture.dds", 
				     L"../secondtexture.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	//m_TextureShader = new TextureShader;
	//if(!m_TextureShader)
	//{
	//	return false;
	//}

	// Initialize the texture shader object.
	//result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	//if(!result)
	//{
		//MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		//return false;
	//}

	// Create the multitexture shader object. NEW IN 0.17. replaces texture stuff above in comments
	m_MultiTextureShader = new MultiTextureShader;
	if(!m_MultiTextureShader)
	{
		return false;
	}

	// Initialize the multitexture shader object.
	result = m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShader;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new Light;
	if(!m_Light)
	{
		return false;
	}

	// Create the model list object.
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = m_ModelList->Initialize(25);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}


	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../seafloor.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);

	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model list object.
	if(m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}


	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	//brought back for 2d rendering
	//if(m_TextureShader)
	//{
		//m_TextureShader->Shutdown();
		//delete m_TextureShader;
		//m_TextureShader = 0;
	//}

	// Release the multitexture shader object. NEW IN 0.17. replaces commented out above.
	if(m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY,int fps, int cpu, float frameTime, float rotationY)
{
	bool result;
	static float rotation = 0.0f;

	// Set the position of the camera.
	//m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Set the position of the camera. CHANGED IN 0.17. for closer look at multitexturing. what it used to be is above
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Set the rotation of the camera.
	m_Camera->SetRotation(0.0f, rotationY, 0.0f);

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Set the location of the mouse.
	//result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.005f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation)
{
	
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix; 
	int modelCount, renderCount, index; // NEW in 0.16
	float positionX, positionY, positionZ, radius; // NEW IN 0.16
	D3DXVECTOR4 color;// NEW IN 0.16
	bool renderModel; // NEW IN 0.16
	//orthoMatrix added in 0.10

	bool result;
	


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, projection, and ortho matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index<modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if(renderModel)
		{
			// Move the model to the location it should be rendered at.
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ); 

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			m_Model->Render(m_D3D->GetDeviceContext());

			// Render the model using the light shader.
			//m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
					    //  m_Model->GetTexture(), m_Light->GetDirection(), color);

			// Render the model using the multitexture shader. NEW IN 0.17. replaces the lightshader above as the way to render the models.
	m_MultiTextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				     m_Model->GetTextureArray());

			// Reset to the original world matrix.
			m_D3D->GetWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}

	// Set the number of models that was actually rendered this frame.
	//result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	//if(!result)
	//{
		//return false;
	//}

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text string of the render count.
	//m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	//if(!result)
	//{
		//return false;
	//}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


