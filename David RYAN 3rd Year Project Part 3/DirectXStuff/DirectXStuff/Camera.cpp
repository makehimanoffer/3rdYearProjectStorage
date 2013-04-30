
 

#include "Camera.h"


Camera::Camera()
{
	_posX = 0.0f;
	_posY = 0.0f;
	_posZ = 0.0f;

	_rotX = 0.0f;
	_rotY = 0.0f;
	_rotZ = 0.0f;
}

Camera::~Camera()
{
}


void Camera::SetPosition(float x, float y, float z)
{
	_posX = x;
	_posY = y;
	_posZ = z;
	return;
}


void Camera::SetRotation(float x, float y, float z)
{
	_rotX = x;
	_rotY = y;
	_rotZ = z;
	return;
}


D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(_posX, _posY, _posZ);
}


D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(_rotX, _rotY, _rotZ);
}


void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMx;


	// Setup vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup position of camera in world.
	position.x = _posX;
	position.y = _posY;
	position.z = _posZ;

	// Setup where camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = _rotX * 0.0174532925f;
	yaw   = _rotY * 0.0174532925f;
	roll  = _rotZ * 0.0174532925f;

	// Create rotation matrix from yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMx, yaw, pitch, roll);

	// Transform lookAt and up vector by rotation matrix so view is correctly rotated at origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMx);
	D3DXVec3TransformCoord(&up, &up, &rotationMx);

	// Translate rotated camera position to location of viewer.
	lookAt = position + lookAt;

	// Finally create view matrix from three updated vectors.
	D3DXMatrixLookAtLH(&_viewMx, &position, &lookAt, &up);

	return;
}


void Camera::GetViewMatrix(D3DXMATRIX& viewMx)
{
	viewMx = _viewMx;
	return;
}


void Camera::GenerateBaseViewMatrix()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMx;


	// Setup vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup position of camera in world.
	position.x = _posX;
	position.y = _posY;
	position.z = _posZ;

	// Setup where camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = _rotX * 0.0174532925f;
	yaw   = _rotY * 0.0174532925f;
	roll  = _rotZ * 0.0174532925f;

	// Create rotation matrix from yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMx, yaw, pitch, roll);

	// Transform lookAt and up vector by rotation matrix so view is correctly rotated at origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMx);
	D3DXVec3TransformCoord(&up, &up, &rotationMx);

	// Translate rotated camera position to location of viewer.
	lookAt = position + lookAt;

	// Finally create view matrix from three updated vectors.
	D3DXMatrixLookAtLH(&_baseViewMx, &position, &lookAt, &up);

	return;
}


void Camera::GetBaseViewMatrix(D3DXMATRIX& viewMx)
{
	viewMx = _baseViewMx;
	return;
}
//This is new function that is used for generating reflection view matrix. only difference between regular view matrix and reflection one is that invert Y position based on height of plane, and also invert pitch.




