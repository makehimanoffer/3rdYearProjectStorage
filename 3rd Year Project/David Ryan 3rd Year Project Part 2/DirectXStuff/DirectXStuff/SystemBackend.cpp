
// SystemBackend.cpp


//MASSIVE CHANGES MADE TO THIS CLASS in favour of an encapsulated format in application class
#include "SystemBackend.h"

SystemBackend::SystemBackend()
{
	_Application = 0;
}


SystemBackend::~SystemBackend()
{
}


bool SystemBackend::Init()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize width and height of screen to zero before sending variables into function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize windows api.
	InitializeWindows(screenWidth, screenHeight);

	//app class object is created and init'd below

	_Application = new Application;
	

	// Initialize application wrapper object.
	result = _Application->Init(_hinstance, _hwnd, screenWidth, screenHeight);
	if(!result)
	{
		return false;
	}

	return true;
}


void SystemBackend::KillSafely()
{

	if(_Application)
	{
		_Application->KillSafely();
		delete _Application;
		_Application = 0;
	}

	// Shutdown window.
	ShutdownWindows();
	
	return;
}


void SystemBackend::Run()
{
	MSG msg;
	bool done, result;


	// Initialize message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from window or user.
	done = false;
	while(!done)
	{
		// Handle windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do frame processing.
			result = Update();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemBackend::Update()
{
	bool result;

	// Do frame processing for application object.
	result = _Application->Update();
	if(!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemBackend::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemBackend::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	ApplicationHandle = this;

	_hinstance = GetModuleHandle(NULL);

	_applicationName = L"David Ryan DirectX Challenge";

	// Setup windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = _hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register window class.
	RegisterClassEx(&wc);

	// Determine resolution of clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set screen to maximum size of users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		// If windowed set to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place window in middle of screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create window screen settings and get handle to it.
	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName, 
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, screenWidth, screenHeight, NULL, NULL, _hinstance, NULL);

	// Bring window up on screen and set it as main focus.
	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	// show mouse cursor.
	ShowCursor(true);

	return;
}

void SystemBackend::ShutdownWindows()
{
	// Show mouse cursor.
	ShowCursor(true);

	// Fix display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove window.
	DestroyWindow(_hwnd);
	_hwnd = NULL;

	// Remove application instance.
	UnregisterClass(_applicationName, _hinstance);
	_hinstance = NULL;

	// Release pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to message handler in system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}


