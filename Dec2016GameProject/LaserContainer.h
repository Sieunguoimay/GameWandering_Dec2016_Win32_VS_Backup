#pragma once
#ifndef LASERCONTAINER_H
#define LASERCONTAINER_H

#include"Laser.h"
#include"Mover.h"
class LaserContainer
{
public:
	LaserContainer(b2World*world);
	~LaserContainer();
	void setup(Texture*texture1, Texture*texture2,Sound*sound);
	void display(const Camera&camera);
	void addNewLaser(float x, float y);
	Laser*getNewLaser();
private:
	LinkedList<Laser*>*lasers;
	LinkedList<Photon*>*photonContainer;
	Texture*texture1;
	Texture*texture2;
	Sound*sound;
	b2World*world;
};

#endif // !LASERCONTAINER_H
