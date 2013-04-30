

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
	~Application();

	bool Init(HINSTANCE, HWND, int, int);
	void KillSafely();
	bool Update();

private:
	bool HandleMovementInput(float);
	
	bool Render();
	

private:
	Input* _Input;
	Direct3DHandler* _Direct3D;
	Timer* _Timer;
	Position* _Position;
	Camera* _Camera;
	Light* _Light;
	Terrain* _Terrain;
	TerrainShader* _TerrainShader;
	
	FramesPS* _Fps;
	CPUQuery* _Cpu;
	FontShader* _FontShader;
	TextHandler* _Text;
	

	Model* _Model;
	LightShader* _LightShader;

	Hud* _Hud;
	TextureShader* _TextureShader;

	Sound* _Sound;
};

#endif