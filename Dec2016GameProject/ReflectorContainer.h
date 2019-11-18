#pragma once
#ifndef REFLECTORCONTAINER_H
#define REFLECTORCONTAINER_H 

#include"Relector.h"
enum DirectionChange {
	LEFT_UP,
	LEFT_DOWN,
	RIGHT_UP,
	RIGHT_DOWN
};
class ReflectorContainer
{
public:
	ReflectorContainer(b2World*world);
	~ReflectorContainer();
	void setup(Texture*texture);
	void display(const Camera&camera);
	void addNewReflector(float x, float y,int ID);
	Relector*getNewReflector();
private:
	LinkedList<Relector*>*reflectors;
	b2World*world;
	float angles[4];
	Texture*texture;
};

#endif // !REFLECTORCONTAINER_H
