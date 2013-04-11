//class doesn't require any initialization or whatever.
//the function ConstructFrustum called after camera is updated

 Frustum.h

#ifndef _Frustum_H_
#define _Frustum_H_



// INCLUDES //

#include <d3dx10math.h>



// Class name: Frustum

class Frustum
{
public:
	Frustum();
	Frustum(const Frustum&);
	~Frustum();

	void ConstructFrustum(float, D3DXMATRIX, D3DXMATRIX);

	bool CheckPoint(float, float, float);
	bool CheckCube(float, float, float, float);
	bool CheckSphere(float, float, float, float);
	bool CheckRectangle(float, float, float, float, float, float);

private:
	D3DXPLANE m_planes[6];
};

#endif

