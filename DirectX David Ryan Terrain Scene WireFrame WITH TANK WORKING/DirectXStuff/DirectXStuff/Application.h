////////////////////////////////////////////////////////////////////////////////
// Filename: Application.h
////////////////////////////////////////////////////////////////////////////////

//main wrapper class for terrain application. handles all graphics, input and processing. 
// this may make way for a sort of scene management if I find the time
#ifndef _Application_H_
#define _Application_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Input.h"
#include "Direct3DHandler.h"
#include "Camera.h"
#include "Terrain.h"
#include "ColorShader.h"
#include "Timer.h"
#include "Position.h"
#include "FramesPS.h"
#include "CPUQuery.h"
#include "FontShader.h"
#include "TextHandler.h"

#include "TerrainShader.h" //NEW IN TERRAIN 0.3
#include "Light.h" //NEW IN TERRAIN 0.3

#include "Frustum.h" //NEW IN TERRAIN 0.7
#include "QuadTree.h" // NEW IN TERRAIN 0.7

#include "Model.h"
#include "LightShader.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: Application
////////////////////////////////////////////////////////////////////////////////
class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();

private:
	Input* m_Input;
	Direct3DHandler* m_Direct3D;
	Camera* m_Camera;
	Terrain* m_Terrain;
	ColorShader* m_ColorShader;
	Timer* m_Timer;
	Position* m_Position;
	FramesPS* m_Fps;
	CPUQuery* m_Cpu;
	FontShader* m_FontShader;
	TextHandler* m_Text;

	TerrainShader* m_TerrainShader; //NEW IN TERRAIN 0.3
	Light* m_Light; //NEW IN TERRAIN 0.3

	Frustum* m_Frustum; // NEW IN TERRAIN 0.7
	QuadTree* m_QuadTree; // NEW IN TERRAIN 0.7
	Model* m_Model;
	LightShader* m_LightShader;
};

#endif
