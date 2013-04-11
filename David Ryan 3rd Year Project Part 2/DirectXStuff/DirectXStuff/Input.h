
// Input.h

#ifndef _Input_H_
#define _Input_H_

/////
// PRE-PROCESSING DIRECTIVES //
/////
#define DIRECTINPUT_VERSION 0x0800


// LINKING //

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


// INCLUDES //

#include <dinput.h>



// Class name: Input

class Input
{
public:
	Input();
	Input(const Input&);
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
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	//used for recording current state keyboard and mouse devices
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	DIMOUSESTATE m_mouseLastState;

	int m_screenWidth, m_screenHeight;

public:
	int m_mouseX, m_mouseY;
};

#endif
