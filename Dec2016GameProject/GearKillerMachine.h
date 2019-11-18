#pragma once
#ifndef GEARKILLERMACHINE_H
#define GEARKILLERMACHINE_H
#include"GearKiller.h"
#include"SendMessage.h"
class GearKillerMachine:public SendMessage
{
public:
	GearKillerMachine(b2World*world, Mover*moverPointer, Vector2D*positioin, Texture*gearTexture = NULL, Texture*stickTexture = NULL, int numberGear = 3);
	~GearKillerMachine();
	void setup();
	void display(const Camera&camera);
	Vector2D*getPosition();
private:
	void intallNewGearKiller();
	LinkedList<GearKiller*>*gearKillerContainer;
	b2Body*staticBodyOrigin;
	Vector2D*position;

	b2World*world;
	Mover*moverPointer;
	Texture*gearTexture;
	Texture*stickTexture;

	float speedRate;
	float radius;
	int numberGear;
};

#endif // !GEARKILLERMACHINE_H
