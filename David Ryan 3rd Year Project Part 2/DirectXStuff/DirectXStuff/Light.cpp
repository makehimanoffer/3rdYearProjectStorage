
// Light.cpp

#include "Light.h"


Light::Light()
{
}


Light::~Light()
{
}


void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void Light::SetDirection(float x, float y, float z)
{
	_direction = D3DXVECTOR3(x, y, z);
	return;
}


D3DXVECTOR4 Light::GetAmbientColor()
{
	return _ambientColor;
}


D3DXVECTOR4 Light::GetDiffuseColor()
{
	return _diffuseColor;
}


D3DXVECTOR3 Light::GetDirection()
{
	return _direction;
}
