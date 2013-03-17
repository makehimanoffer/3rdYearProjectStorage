////////////////////////////////////////////////////////////////////////////////
// Filename: Position.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _Position_H_
#define _Position_H_


//////////////
// INCLUDES //
//////////////
#include <math.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: Position
////////////////////////////////////////////////////////////////////////////////
class Position
{
public:
	Position();
	Position(const Position&);
	~Position();

	void SetFrameTime(float);
	void GetRotation(float&);

	void SetPosition(float, float, float); //NEW in TERRAIN 0.1
	void SetRotation(float, float, float); //NEW in TERRAIN 0.1

	void GetPosition(float&, float&, float&); //NEW in TERRAIN 0.1
	void GetRotation(float&, float&, float&); //NEW in TERRAIN 0.1
	void MoveForward(bool); //NEW in TERRAIN 0.1
	void MoveBackward(bool); //NEW in TERRAIN 0.1
	void MoveUpward(bool); //NEW in TERRAIN 0.1
	void MoveDownward(bool);//NEW in TERRAIN 0.1
	void GoLeft(bool);
	void GoRight(bool);
	void LookUpward(bool); //NEW in TERRAIN 0.1
	void LookDownward(bool);//NEW in TERRAIN 0.1

private:
	float m_frameTime;
	
	float m_positionX, m_positionY, m_positionZ;//NEW in TERRAIN 0.1
	float m_rotationX, m_rotationY, m_rotationZ; //NEW in TERRAIN 0.1
	
	

	float m_forwardSpeed, m_backwardSpeed; //NEW in TERRAIN 0.1
	float m_upwardSpeed, m_downwardSpeed; //NEW in TERRAIN 0.1
	float m_leftSpeed, m_rightSpeed; 
	float m_lookUpSpeed, m_lookDownSpeed; //NEW in TERRAIN 0.1
};

#endif