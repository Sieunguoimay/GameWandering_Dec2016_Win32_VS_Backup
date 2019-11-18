#pragma once
#ifndef POWERPOLE_H
#define POWERPOLE_H
#include"globalheader.h"
#include"Camera.h"
#include"Texture.h"
#include"SendMessage.h"
#include"Bird.h"
#include"Path.h"
//each power pole is a environment for its own flock of birds
//so its should store all the neccessary pointers
//that a bird requires
class PowerPole:public SendMessage
{
public:
	PowerPole(b2World*world);
	~PowerPole();
	void setup(Vector2D*position,Texture*texture);
	void display(const Camera&camera);
	Vector2D&getPosition2();
	float getHeight()const;
private:
	//pointer
	Vector2D*position1;
	Vector2D*position2;
	Texture*texture;
	Size*size;
	b2World*world;
};

#endif // !POWERPOLE_H
