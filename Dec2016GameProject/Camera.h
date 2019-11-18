#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include"globalheader.h"

class Camera
{
public:
	Camera(Size*windowSize);
	~Camera();
	void setup();
	void reset();
	void setTarget(float x, float y);//set it equals the velocity of mover
	void update();
	const Vector2D&getPosition()const;
	const Size&getSize()const;
private:
	Vector2D*acceleration;
	Vector2D*velocity;
	Vector2D*location;
	Vector2D*target;
	Vector2D*desired;
	float maxSpeed;
	float maxForce;
	Size*size;
};

#endif // !CAMERA_H
