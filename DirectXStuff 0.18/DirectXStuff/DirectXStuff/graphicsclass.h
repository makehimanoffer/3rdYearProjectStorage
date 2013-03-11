////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"

//a new addition made in 0.10
// the bitmap class
#include "bitmapclass.h"

//and the texture shader class
//#include "textureshaderclass.h"

//and text class. NEW FOR 0.12
#include "textclass.h"

#include "modellistclass.h" //new for 0.16
#include "frustumclass.h" // NEW FOR 0.16

#include "multitextureshaderclass.h" // NEW for 0.17. It replaces the include of the textureshaderclass as seen above in comments.

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
	bool Frame(int, int,int,int, float,float);
	//frame has two new args in 0.13
	//frame has another new arg in 0.15

	bool Render(float);

	//Render is no longer a private method. in 0.13

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	//TextureShaderClass* m_TextureShader; //used for 2d rendering

	MultiTextureShaderClass* m_MultiTextureShader;// NEW IN 0.17 Replaces textureshaderclass as seen above in comments.
	BitmapClass* m_Bitmap; // new for 0.10

	TextClass* m_Text;// NEW FOR 0.12. 

	ModelListClass* m_ModelList; // NEW FOR 0.16
	FrustumClass* m_Frustum; // NEW FOR 0.16
};

#endif