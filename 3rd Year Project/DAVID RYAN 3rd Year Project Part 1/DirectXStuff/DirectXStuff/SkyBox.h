
#ifndef _SkyBox_H_
#define _SkyBox_H_


#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


class SkyBox
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
	};

public:
	SkyBox();
	~SkyBox();

	bool Init(ID3D11Device*);
	void KillSafely();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	D3DXVECTOR4 GetApexColor();
	D3DXVECTOR4 GetCenterColor();

private:
	bool LoadSkyBoxModel(char*);
	void ReleaseSkyBoxModel();

	bool InitBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ModelType* _model;
	int _vertexCount, _indexCount;
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	D3DXVECTOR4 _apexColor, _centerColor;
};

#endif