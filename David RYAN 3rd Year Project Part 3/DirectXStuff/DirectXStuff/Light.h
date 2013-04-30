
 

#ifndef _Light_H_
#define _Light_H_



// INCLUDES //

#include <d3dx10math.h>




class Light
{
public:
	Light();
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	D3DXVECTOR4 GetAmbientColor();
	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR3 GetDirection();

private:
	D3DXVECTOR4 _ambientColor;
	D3DXVECTOR4 _diffuseColor;
	D3DXVECTOR3 _direction;
};

#endif