
#include "Input.h"

Input::Input()
{
	_directInput = 0;
	_keyboard = 0;
	_mouse = 0;
}



Input::~Input()
{
}


bool Input::Init(HINSTANCE hinstance, HWND hwnd, int scrWidth, int scrHeight)
{
	HRESULT result;


	// Store screen size which will be used for positioning mouse cursor.
	_scrWidth = scrWidth;
	_scrHeight = scrHeight;

	// Init location of mouse on screen.
	_mouseX = 0;
	_mouseY = 0;

	// Init main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL);
	if(FAILED(result))
	{
		return false;
	}

	//initializing keyboard
	// Init direct input interface for keyboard.
	result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set data format.  In this case since it is a keyboard can use predefined data format.
	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}

	// Set cooperative level of keyboard to not share with other programs.

	//ok. if set to DISCL_EXCLUSIVE only this application has access to it.
	//if set to DISCL_NONEXCLUSIVE
	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	//then call acquire to get keyboard
	// Now acquire keyboard.
	result = _keyboard->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	//device setup of mouse
	// Init direct input interface for mouse.
	result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Set data format for mouse using pre-defined mouse data format.
	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}

	//for mouse. It has to be set non-exclusive.
	//has to be checked for going in and out of focus.
	//and re-acquire it each time. 
	// a similar process may be required for keyboard in and out of focus

	// Set cooperative level of mouse to share with other programs.
	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if(FAILED(result))
	{
		return false;
	}

	// Acquire mouse.
	result = _mouse->Acquire();
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void Input::KillSafely()
{
	if(_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = 0;
	}

	if(_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = 0;
	}

	// Release main interface to direct input.
	if(_directInput)
	{
		_directInput->Release();
		_directInput = 0;
	}

	return;
}

//frame will read current state of devices into device buffers setup
//after state of each device is read  process changes. 
bool Input::Update()
{
	bool result;


	// Read current state of keyboard.
	result = ReadKeyboard();
	if(!result)
	{
		return false;
	}

	// Read current state of mouse.
	result = ReadMouse();
	if(!result)
	{
		return false;
	}

	// Process changes in mouse and keyboard.
	ProcessInput();

	return true;
}

//read keyboard. read state into _keyboardState.
//state will show any keys currently pressed and not pressed.
//it can fail reading for following reasons.
//1. If focus is lost.
//2. If it becomes un-acquired. if this happens. call acquire each frame
//
bool Input::ReadKeyboard()
{
	HRESULT result;


	// Read keyboard device.
	result = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
	if(FAILED(result))
	{
		// If keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
		
	return true;
}


//read mouse monitors state(position) of mouse.

bool Input::ReadMouse()
{
	HRESULT result;


	// Read mouse device.
	result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if(FAILED(result))
	{
		// If mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

//process input deals with changes that would have occured since last frame. 
//simply add changes to mouse position.
//also check to make sure mouse doesn't move off screen

void Input::ProcessInput()
{
	if((_mouseState.lX != _mouseLastState.lX) || (_mouseState.lY != _mouseLastState.lY))
	{
		// Update location of mouse cursor based on change of mouse location during frame.
		_mouseY += _mouseState.lX*0.5f;
		_mouseX += _mouseState.lY*0.5f;

		_mouseLastState = _mouseState;
	}
	
		

		
	

	
	
	return;
}

//next comes is escape pressed. this can be used as a prototype for all types of button press
bool Input::IsEscapePressed()
{
	
	if(_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsAPressed()
{
	
	if(_keyboardState[DIK_A] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is1Pressed()
{
	
	if(_keyboardState[DIK_1] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is2Pressed()
{
	
	if(_keyboardState[DIK_2] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is3Pressed()
{
	
	if(_keyboardState[DIK_3] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is4Pressed()
{
	
	if(_keyboardState[DIK_4] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is5Pressed()
{
	
	if(_keyboardState[DIK_5] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is6Pressed()
{
	
	if(_keyboardState[DIK_6] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::Is7Pressed()
{
	
	if(_keyboardState[DIK_7] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsPPressed()
{
	
	if(_keyboardState[DIK_P] & 0x80)
	{
		return true;
	}

	return false;
}


bool Input::IsDPressed()
{
	
	if(_keyboardState[DIK_D] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsWPressed()
{
	
	if(_keyboardState[DIK_W] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsSPressed()
{
	
	if(_keyboardState[DIK_S] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsPgUpPressed()
{
	
	if(_keyboardState[DIK_PGUP] & 0x80)
	{
		return true;
	}

	return false;
}

bool Input::IsPgDownPressed()
{
	
	if(_keyboardState[DIK_PGDN] & 0x80)
	{
		return true;
	}

	return false;
}




//helper function. for use in graphics class
void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
	return;
}

