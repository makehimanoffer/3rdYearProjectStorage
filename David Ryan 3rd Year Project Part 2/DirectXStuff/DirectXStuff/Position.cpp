
// Position.cpp

#include "Position.h"

Position::Position()
{
	m_positionX = 0.0f; //NEW in TERRAIN 0.1
	m_positionY = 0.0f; //NEW in TERRAIN 0.1
	m_positionZ = 0.0f; //NEW in TERRAIN 0.1
	
	m_rotationX = 0.0f; //NEW in TERRAIN 0.1
	m_rotationY = 0.0f; 
	m_rotationZ = 0.0f; //NEW in TERRAIN 0.1

	m_frameTime = 0.0f;

	m_forwardSpeed   = 0.0f; //NEW in TERRAIN 0.1
	m_backwardSpeed  = 0.0f; //NEW in TERRAIN 0.1
	m_upwardSpeed    = 0.0f; //NEW in TERRAIN 0.1
	m_downwardSpeed  = 0.0f; //NEW in TERRAIN 0.1
	m_leftSpeed  = 0.0f; //NEW in TERRAIN 0.1
	m_rightSpeed = 0.0f; //NEW in TERRAIN 0.1
	m_lookUpSpeed    = 0.0f; //NEW in TERRAIN 0.1
	m_lookDownSpeed  = 0.0f; //NEW in TERRAIN 0.1

}


Position::Position(const Position& other)
{
}


Position::~Position()
{
}

// both the set position and set rotation methods are used for setting position and rotation of the camera
//used to init position of camera at somewhere other than the origin 
//will be set slightly back  and in the center of it.


void Position::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void Position::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}

//the get position and get rotation used to return current pos and rot of camera
//they will be called to provide location/rotation of camera for display purposes .
//will be drawn as text strings. useful for debugging some stuff.
void Position::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


void Position::GetRotation(float& x, float& y, float& z)
{
	x = m_rotationX;
	y = m_rotationY;
	z = m_rotationZ;
	return;
}




//used to set frame speed.
//needs to be called each frame.
//done so that regardless of app speed the rotation and movement. otherwise movement speed would go up and down with frame rate

void Position::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}


//now come about eight movement functions. they are all called each frame
//the camera will accelerate with buttons, will also slow down
//speed is calc'd against frame time. this ensure movement remains same regardless of framerate.
//basic maths to calc camera then

void Position::MoveForward(bool keydown)
{
	float radians;


	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_forwardSpeed += m_frameTime * 0.001f;

		if(m_forwardSpeed > (m_frameTime * 0.03f))
		{
			m_forwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_forwardSpeed -= m_frameTime * 0.0007f;

		if(m_forwardSpeed < 0.0f)
		{
			m_forwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_forwardSpeed;
	m_positionZ += cosf(radians) * m_forwardSpeed;

	return;
}

void Position::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_backwardSpeed += m_frameTime * 0.001f;

		if(m_backwardSpeed > (m_frameTime * 0.03f))
		{
			m_backwardSpeed = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_backwardSpeed -= m_frameTime * 0.0007f;
		
		if(m_backwardSpeed < 0.0f)
		{
			m_backwardSpeed = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_backwardSpeed;
	m_positionZ -= cosf(radians) * m_backwardSpeed;

	return;
}

//move upward calcs upward speed and movement of camera
void Position::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_upwardSpeed += 0.3f;

		
	}
	else
	{
		m_upwardSpeed = 0;

		
	}

	// Update the height position.
	m_positionY += m_upwardSpeed;

	return;
}

//move downward calc downward speed
void Position::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_downwardSpeed += 0.3f;

		
	}
	else
	{
		m_downwardSpeed =0;

	}

	// Update the height position.
	m_positionY -= m_downwardSpeed;

	return;
}



void Position::GoLeft(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_leftSpeed -= m_frameTime * 0.01f;

		
	}
	else{
		m_leftSpeed=0;
	}
		
	

	

	m_positionX+=m_leftSpeed;

	return;
}



void Position::GoRight(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_rightSpeed += m_frameTime * 0.01f;

		
	}
	else{
		m_rightSpeed=0;
	}
	
		
	

	

	m_positionX+=m_rightSpeed;

	return;
}

//look upward calcs upward turn speed and ROTATION of camera
void Position::LookUpward(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpSpeed += m_frameTime * 0.01f;

		if(m_lookUpSpeed > (m_frameTime * 0.15f))
		{
			m_lookUpSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpSpeed -= m_frameTime* 0.005f;

		if(m_lookUpSpeed < 0.0f)
		{
			m_lookUpSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX -= m_lookUpSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}

	return;
}


//look downward calcs downward turn speed and ROTATION 

void Position::LookDownward(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookDownSpeed += m_frameTime * 0.01f;

		if(m_lookDownSpeed > (m_frameTime * 0.15f))
		{
			m_lookDownSpeed = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookDownSpeed -= m_frameTime* 0.005f;

		if(m_lookDownSpeed < 0.0f)
		{
			m_lookDownSpeed = 0.0f;
		}
	}

	// Update the rotation.
	m_rotationX += m_lookDownSpeed;

	// Keep the rotation maximum 90 degrees.
	if(m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}

	return;
}
