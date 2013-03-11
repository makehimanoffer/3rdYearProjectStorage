////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>

//#include "textureclass.h"

#include "texturearrayclass.h" //THIS is NEW IN 0.17. It is meant to replace the texture class header above in comments. 



#include <fstream> //new for 0.6
using namespace std; //new for 0.6


////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal; // new addition for 0.5 to accomodate lighting normal

	};

	struct ModelType //introduction of a new struct in 0.6 for the file format
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*); //extended argument in 0.6 for file handling
	//arg has an extra WCHAR* added in 0.17 for multitexturing
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	//ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();// NEW IN 0.17, replaces the get texture function above.

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	//bool LoadTexture(ID3D11Device*, WCHAR*); 
	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*); // NEW IN 0.17 Replaces the load texture as seen above. 
	//void ReleaseTexture();
	void ReleaseTextures();//replaces the Release Texture above. new in 0.17


	bool LoadModel(char*); //new for 0.6. for loading model
	void ReleaseModel(); //new for 0.6. for unloading model

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	//TextureClass* m_Texture;

	TextureArrayClass* m_TextureArray; // this is new in 0.17. it replaces the TextureClass in comments above.
	ModelType* m_model;//new in 0.6. used to read in and hold model data before vertex buffer
};

#endif