////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Direct3DHandler.h"
#include "cameraclass.h"
#include "Model.h"
#include "LightShader.h"
#include "Light.h"

//a new addition made in 0.10
// the bitmap class
#include "bitmapclass.h"

//and the texture shader class
//#include "TextureShader.h"

//and text class. NEW FOR 0.12
#include "textclass.h"

#include "modellistclass.h" //new for 0.16
#include "frustumclass.h" // NEW FOR 0.16

#include "multiTextureShader.h" // NEW for 0.17. It replaces the include of the TextureShader as seen above in comments.

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
	Direct3DHandler* m_D3D;
	CameraClass* m_Camera;
	Model* m_Model;
	LightShader* m_LightShader;
	Light* m_Light;
	//TextureShader* m_TextureShader; //used for 2d rendering

	MultiTextureShader* m_MultiTextureShader;// NEW IN 0.17 Replaces TextureShader as seen above in comments.
	BitmapClass* m_Bitmap; // new for 0.10

	TextClass* m_Text;// NEW FOR 0.12. 

	ModelListClass* m_ModelList; // NEW FOR 0.16
	FrustumClass* m_Frustum; // NEW FOR 0.16
};

#endif