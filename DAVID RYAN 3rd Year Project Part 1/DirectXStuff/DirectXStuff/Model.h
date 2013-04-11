
#ifndef _Model_H_
#define _Model_H_



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

	struct ModelType //introduction of a new struct in 0.6 for the file format
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Init(ID3D11Device*, char*, WCHAR*); //extended argument in 0.6 for file handling
	//arg has an extra WCHAR* added in 0.17 for multitexturing
	void KillSafely();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
	//ID3D11ShaderResourceView** GetTextureArray();// NEW IN 0.17, replaces the get texture function above.

private:
	bool InitBuffers(ID3D11Device*);
	void KillSafelyBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*); 
	
	void ReleaseTexture();
	


	bool LoadModel(char*); //new for 0.6. for loading model
	void ReleaseModel(); //new for 0.6. for unloading model

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	Texture* m_Texture;

	
	ModelType* m_model;//new in 0.6. used to read in and hold model data before vertex buffer
};

#endif