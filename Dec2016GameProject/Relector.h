#pragma once
#ifndef REFLECTOR_H
#define REFLECTOR_H
#include"globalheader.h"
#include"Photon.h"
#include"SendMessage.h"
class Relector: public SendMessage
{
public:
	Relector(b2World*world, Texture*texture = NULL);
	~Relector();
	void setup(float x, float y,float angle);
	void display(const Camera&camera);
	void reflect(Photon&photon);

private:
	float angle;
	float x;
	float y;
	b2Body*body;
	ObjectData*data;
	b2World*world;
	Texture*texture;
};

#endif // !REFLECTOR_H
