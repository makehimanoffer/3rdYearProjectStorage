

#ifndef _Application_H_
#define _Application_H_



// GLOBALS //

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;



// MY CLASS INCLUDES //

#include "Input.h"
#include "Direct3DHandler.h"
#include "Timer.h"
#include "Position.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "TerrainShader.h"

#include "FramesPS.h"
#include "CPUQuery.h"
#include "FontShader.h"
#include "TextHandler.h"


#include "Model.h"
#include "LightShader.h"

#include "Hud.h"
#include "TextureShader.h"

#include "Sound.h"





class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Init(HINSTANCE, HWND, int, int);
	void KillSafely();
	bool Frame();

private:
	bool HandleMovementInput(float);
	void RenderRefractionToTexture();
	void RenderReflectionToTexture();
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
	
	FramesPS* m_Fps;
	CPUQuery* m_Cpu;
	FontShader* m_FontShader;
	TextHandler* m_Text;
	

	Model* m_Model;
	LightShader* m_LightShader;

	Hud* m_Hud;
	TextureShader* m_TextureShader;

	Sound* m_Sound;
};

#endif