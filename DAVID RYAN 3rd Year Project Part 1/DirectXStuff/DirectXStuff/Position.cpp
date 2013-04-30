
#include "Position.h"

Position::Position()
{
	_posX = 0.0f; //NEW in TERRAIN 0.1
	_posY = 0.0f; //NEW in TERRAIN 0.1
	_posZ = 0.0f; //NEW in TERRAIN 0.1
	
	_rotX = 0.0f; //NEW in TERRAIN 0.1
	_rotY = 0.0f; 
	_rotZ = 0.0f; //NEW in TERRAIN 0.1

	_frameTime = 0.0f;

	_forwardSpeed   = 0.0f; //NEW in TERRAIN 0.1
	_backwardSpeed  = 0.0f; //NEW in TERRAIN 0.1
	_upwardSpeed    = 0.0f; //NEW in TERRAIN 0.1
	_downwardSpeed  = 0.0f; //NEW in TERRAIN 0.1
	_leftSpeed  = 0.0f; //NEW in TERRAIN 0.1
	_rightSpeed = 0.0f; //NEW in TERRAIN 0.1
	_lookUpSpeed    = 0.0f; //NEW in TERRAIN 0.1
	_lookDownSpeed  = 0.0f; //NEW in TERRAIN 0.1

}





Position::~Position()
{
}

// both set position and set rotation methods are used for setting position and rotation of camera
//used to init position of camera at somewhere other than origin 
//will be set slightly back  and in center of it.


void Position::SetPosition(float x, float y, float z)
{
	_posX = x;
	_posY = y;
	_posZ = z;
	return;
}


void Position::SetRotation(float x, float y, float z)
{
	_rotX = x;
	_rotY = y;
	_rotZ = z;
	return;
}

//get position and get rotation used to return current pos and rot of camera
//they will be called to provide location/rotation of camera for display purposes .
//will be drawn as text strings. useful for debugging some stuff.
void Position::GetPosition(float& x, float& y, float& z)
{
	x = _posX;
	y = _posY;
	z = _posZ;
	return;
}


void Position::GetRotation(float& x, float& y, float& z)
{
	x = _rotX;
	y = _rotY;
	z = _rotZ;
	return;
}




//used to set frame speed.
//needs to be called each frame.
//done so that regardless of app speed rotation and movement. otherwise movement speed would go up and down with frame rate

void Position::SetFrameTime(float time)
{
	_frameTime = time;
	return;
}


//now come about eight movement functions. they are all called each frame
//camera will accelerate with buttons, will also slow down
//speed is calc'd against frame time. this ensure movement remains same regardless of framerate.
//basic maths to calc camera then

void Position::MoveForward(bool keydown)
{
	float radians;


	// Update forward speed movement based on frame time and whether user is holding key down or not.
	if(keydown)
	{
		_forwardSpeed += _frameTime * 0.001f;

		if(_forwardSpeed > (_frameTime * 0.03f))
		{
			_forwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_forwardSpeed -= _frameTime * 0.0007f;

		if(_forwardSpeed < 0.0f)
		{
			_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = _rotY * 0.0174532925f;

	// Update position.
	_posX += sinf(radians) * _forwardSpeed;
	_posZ += cosf(radians) * _forwardSpeed;

	return;
}

void Position::MoveBackward(bool keydown)
{
	float radians;


	// Update backward speed movement based on frame time and whether user is holding key down or not.
	if(keydown)
	{
		_backwardSpeed += _frameTime * 0.001f;

		if(_backwardSpeed > (_frameTime * 0.03f))
		{
			_backwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_backwardSpeed -= _frameTime * 0.0007f;
		
		if(_backwardSpeed < 0.0f)
		{
			_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = _rotY * 0.0174532925f;

	// Update position.
	_posX -= sinf(radians) * _backwardSpeed;
	_posZ -= cosf(radians) * _backwardSpeed;

	return;
}

//move upward calcs upward speed and movement of camera
void Position::MoveUpward(bool keydown)
{
	// Update upward speed movement based on frame time and whether user is holding key down or not.
	if(keydown)
	{
		_upwardSpeed += _frameTime * 0.003f;

		if(_upwardSpeed > (_frameTime * 0.03f))
		{
			_upwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_upwardSpeed -= _frameTime * 0.002f;

		if(_upwardSpeed < 0.0f)
		{
			_upwardSpeed = 0.0f;
		}
	}

	// Update height position.
	_posY += _upwardSpeed;

	return;
}

//move downward calc downward speed
void Position::MoveDownward(bool keydown)
{
	// Update downward speed movement based on frame time and whether user is holding key down or not.
	if(keydown)
	{
		_downwardSpeed += _frameTime * 0.003f;

		if(_downwardSpeed > (_frameTime * 0.03f))
		{
			_downwardSpeed = _frameTime * 0.03f;
		}
	}
	else
	{
		_downwardSpeed -= _frameTime * 0.002f;

		if(_downwardSpeed < 0.0f)
		{
			_downwardSpeed = 0.0f;
		}
	}

	// Update height position.
	_posY -= _downwardSpeed;

	return;
}



void Position::GoLeft(bool keydown)
{
	float radians;


	// Update backward speed movement based on frame time and whether user is holding key down or not.
	if(keydown)
	{
		_leftSpeed -= _frameTime * 0.01f;

		
	}
	else{
		_leftSpeed=0;
	}
		
	

	

	_posX+=_leftSpeed;

	return;
}



void Position::GoRight(bool keydown)
{
	float radians;


	// Update backward speed movement based on frame time and whether user is holding key down or not.
	if(keydown)
	{
		_rightSpeed += _frameTime * 0.01f;

		
	}
	else{
		_rightSpeed=0;
	}
	
		
	

	

	_posX+=_rightSpeed;

	return;
}







