
#ifndef _SystemBackend_H_
#define _SystemBackend_H_


#define WIN32_LEAN_AND_MEAN


#include <windows.h>


#include "Application.h"  //NEW in TERRAIN 0.1
// This application class will perform function of what system class did before being simplified. 
//this allows for a degree of encapsulation


class SystemBackend
{
public:
	SystemBackend();
	~SystemBackend();

	bool Init();
	void KillSafely();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Update();
	void InitWindows(int&, int&);
	void KillSafelyWindows();

private:
	LPCWSTR _applicationName;
	HINSTANCE _hinstance;
	HWND _hwnd;
	Application* _Application; // NEW IN TERRAIN 0.1
};


// FUNCTION PROTOTYPES //

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



// GLOBALS //

static SystemBackend* ApplicationHandle = 0;


#endif