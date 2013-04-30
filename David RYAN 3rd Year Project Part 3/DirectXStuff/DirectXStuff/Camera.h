


#ifndef _Camera_H_
#define _Camera_H_



// INCLUDES //

#include <d3dx10math.h>





class Camera
{
public:
	Camera();
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

	void GenerateBaseViewMatrix();
	void GetBaseViewMatrix(D3DXMATRIX&);




private:
	
	float _rotX, _rotY, _rotZ;
//There is also an additional matrix for reflection view.

	D3DXMATRIX _viewMx, _baseViewMx, _reflectionViewMx;

public:
	float _posX, _posY, _posZ;
};

#endif