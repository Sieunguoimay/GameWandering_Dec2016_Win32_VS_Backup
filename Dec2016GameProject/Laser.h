#pragma once
#ifndef LASER_H
#define LASER_H
#include"SendMessage.h"
#include"Photon.h"
#include"Sound.h"
class Laser:public SendMessage
{
public:
	Laser(b2World*world, Texture*texture, Texture*texture2, Sound*sound,LinkedList<Photon*>*photonContainer);
	~Laser();
	//direction param here is:
	//1-up 2-right 3- down 4-left
	void setup(float x, float y,int direction);
	void display(const Camera&camera);

	void start();
	void cease();
private:
	void emit();
	LinkedList<Photon*>*photonContainer;
	Vector2D*position;
	Vector2D*velocity;
	bool switcher;
	int emitCouter;
	float time;
	float size;
	Texture*texture;
	Texture*texture2;
	Sound*sound;
	bool beingShown;
	b2World*world;
	b2Body*body;
	ObjectData*data;
};

#endif // !LASER_H
