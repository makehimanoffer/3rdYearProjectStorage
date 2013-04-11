
#ifndef _Application_H_
#define _Application_H_


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
#include "SkyBox.h"
#include "SkyBoxShader.h"
#include "Skyplane.h"
#include "SkyplaneShader.h"
#include "FramesPS.h"
#include "CPUQuery.h"
#include "FontShader.h"
#include "TextHandler.h"
#include "RenderTexture.h"
#include "ReflectionShader.h"
#include "Water.h"
#include "WaterShader.h"



class Application
{
public:
	Application();
	~Application();

	bool Init(HINSTANCE, HWND, int, int);
	void KillSafely();
	bool Update();

private:
	bool HandleControls(float);
	void RenderRefractionToTexture();
	void RenderReflectionToTexture();
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
	SkyBox* _SkyBox;
	SkyBoxShader* _SkyBoxShader;
	Skyplane *_SkyPlane;
	SkyplaneShader* _SkyPlaneShader;
	FramesPS* _Fps;
	CPUQuery* _Cpu;
	FontShader* _FontShader;
	TextHandler* _Text;
	RenderTexture *_RefractTexture, *_ReflTexture;
	ReflectionShader* _ReflectionShader;
	Water* _Water;
	WaterShader* _WaterShader;
};

#endif