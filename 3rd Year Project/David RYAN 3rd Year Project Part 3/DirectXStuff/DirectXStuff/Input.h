
 

#ifndef _Input_H_
#define _Input_H_

///
// PRE-PROCESSING DIRECTIVES //
///
#define DIRECTINPUT_VERSION 0x0800


// LINKING //

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


// INCLUDES //

#include <dinput.h>





class Input
{
public:
	Input();
	~Input();

	bool Init(HINSTANCE, HWND, int, int);
	void KillSafely();
	bool Update();

	bool IsEscapePressed();
	
	bool IsAPressed();
	bool IsDPressed();

	bool IsWPressed();
    bool IsSPressed();
	
	bool Is1Pressed();
	bool Is2Pressed();

	bool IsPgUpPressed();
	bool IsPgDownPressed();
	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	//used for recording current state keyboard and mouse devices
	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;
	DIMOUSESTATE _mouseLastState;

	int _scrWidth, _scrHeight;

public:
	int _mouseX, _mouseY;
};

#endif
