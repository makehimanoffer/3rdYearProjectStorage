
 

#ifndef _Model_H_
#define _Model_H_



// INCLUDES //

#include <d3d11.h>
#include <d3dx10math.h>

#include "Texture.h"





#include <fstream> //new for 0.6
using namespace std; //new for 0.6





class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal; // new addition for 0.5 to accomodate lighting normal

	};

	struct ModelType //introduction of a new struct in 0.6 for file format
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model();
	~Model();

	bool Init(ID3D11Device*, char*, WCHAR*); //extended argument in 0.6 for file handling
	void KillSafely();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
	//ID3D11ShaderResourceView** GetTextureArray();// NEW IN 0.17, replaces get texture function above.

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*); 
	
	void ReleaseTexture();
	


	bool LoadModel(char*); //new for 0.6. for loading model
	void ReleaseModel(); //new for 0.6. for unloading model

private:
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	int _vertexCount, _indexCount;

	Texture* _Texture;

	
	ModelType* _model;//new in 0.6. used to read in and hold model data before vertex buffer
};

#endif