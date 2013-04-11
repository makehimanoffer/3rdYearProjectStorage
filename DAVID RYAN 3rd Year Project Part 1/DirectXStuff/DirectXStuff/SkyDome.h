
#ifndef _SkyDome_H_
#define _SkyDome_H_


#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;


class SkyDome
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
	SkyDome();
	SkyDome(const SkyDome&);
	~SkyDome();

	bool Init(ID3D11Device*);
	void KillSafely();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	D3DXVECTOR4 GetApexColor();
	D3DXVECTOR4 GetCenterColor();

private:
	bool LoadSkyDomeModel(char*);
	void ReleaseSkyDomeModel();

	bool InitBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ModelType* m_model;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	D3DXVECTOR4 m_apexColor, m_centerColor;
};

#endif