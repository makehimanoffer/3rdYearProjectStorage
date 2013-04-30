
#ifndef _Position_H_
#define _Position_H_



#include <math.h>


class Position
{
public:
	Position();
	
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
	

private:
	float _frameTime;
	
	float _posX, _posY, _posZ;//NEW in TERRAIN 0.1
	float _rotX, _rotY, _rotZ; //NEW in TERRAIN 0.1
	
	

	float _forwardSpeed, _backwardSpeed; //NEW in TERRAIN 0.1
	float _upwardSpeed, _downwardSpeed; //NEW in TERRAIN 0.1
	float _leftSpeed, _rightSpeed; 
	float _lookUpSpeed, _lookDownSpeed; //NEW in TERRAIN 0.1
};

#endif