#pragma once
#ifndef GEARKILLER_H
#define GEARKILLER_H
#include"globalheader.h"
#include"Mover.h"
class GearKiller
{
public:
	GearKiller(b2World*world,Mover*moverPointer,Vector2D*positioin,Texture*gearTexture = NULL,Texture*stickTexture = NULL,float radius = 40.0f);
	~GearKiller();
	void setup();
	void display(const Camera&camera);
	void plugin(b2Body*staticBody,float motorSpeed = 3.141592f);

	void setRadius(float radius);

	Vector2D*getDynamicPosition();
	b2Body*getStickBody();
private:
	b2Body*gearBody;
	b2Body*stickBody;
	Vector2D*staticPosition;
	Vector2D*dynamicPosition;

	ObjectData*data;//data for the gear to check collision with mover inside contactListener
	float gearRadius;//srick's length is 4 times gear's radius
					 //and width is 0.5 times of it
	Mover*moverPointer;
	b2World*world;

	Texture*gearTexture;
	Texture*stickTexture;
};

#endif // !GEARKILLER_H
