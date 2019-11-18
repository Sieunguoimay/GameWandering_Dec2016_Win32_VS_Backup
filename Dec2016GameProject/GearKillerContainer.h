#pragma once
#ifndef GEARKILLERCONTAINER_H
#define GEARKILLERCONTAINER_H

#include"GearKillerMachine.h"
class GearKillerContainer
{
public:
	GearKillerContainer(b2World*world);
	~GearKillerContainer();
	void reset();
	void setup(Texture*gearTexture,Texture*stickTexture,Mover*mover);
	void display(const Camera&camera);
	void addNewKiller(float x, float y, int numberGear);
	GearKillerMachine*getNewKiller();
	void del(bool leftOrRight);
private:
	LinkedList<GearKillerMachine*>*gearKillerContainer;
	
	Texture*gearTexture;
	Texture*stickTexture;

	b2World*world;
	Mover*moverPointer;
};

#endif // !GEARKILLERCONTAINER_H
