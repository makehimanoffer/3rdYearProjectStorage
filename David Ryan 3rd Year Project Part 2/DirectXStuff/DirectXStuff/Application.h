
// Application.h

#ifndef _Application_H_
#define _Application_H_



// GLOBALS //

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

#include "Input.h"
#include "Direct3DHandler.h"
#include "Timer.h"
#include "Position.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "TerrainShader.h"
#include "SkyDome.h"
#include "SkyDomeShader.h"
#include "Skyplane.h"
#include "SkyplaneShader.h"
#include "FramesPS.h"
#include "CPUQuery.h"
#include "FontShader.h"
#include "TextHandler.h"




// Class name: Application

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Init(HINSTANCE, HWND, int, int);
	void KillSafely();
	bool Update();

private:
	bool HandleControls(float);
	
	bool Render();

private:
	Input* m_Input;
	Direct3DHandler* m_Direct3D;
	Timer* m_Timer;
	Position* m_Position;
	Camera* m_Camera;
	Light* m_Light;
	Terrain* m_Terrain;
	TerrainShader* m_TerrainShader;
	SkyDome* m_SkyDome;
	SkyDomeShader* m_SkyDomeShader;
	Skyplane *m_SkyPlane;
	SkyplaneShader* m_SkyPlaneShader;
	FramesPS* m_Fps;
	CPUQuery* m_Cpu;
	FontShader* m_FontShader;
	TextHandler* m_Text;
	
};

#endif