////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
//EDITED IN 0.2
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_



//////////////
// INCLUDES //
//////////////
//#include <windows.h> being replaced by the new header below for 0.2
#include "d3dclass.h"

#include "cameraclass.h" //new in 0.3
#include "modelclass.h" //new in 0.3
#include "colorshaderclass.h" //new in 0.3

#include "textureshaderclass.h" //new in 0.4


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_D3D; // second change in 0.2
	CameraClass* m_Camera; //new in 0.3
	ModelClass* m_Model; //new in 0.3
	
	TextureShaderClass* m_TextureShader; //replaces the old ColorShaderClass with this in 0.4
};

#endif