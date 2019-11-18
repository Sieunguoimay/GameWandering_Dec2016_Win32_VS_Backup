#pragma once
#ifndef FISH_H
#define FISH_H
#include"Vehicle.h"

//a fish will has additional abilities:
//aligement
//cohension
//stayWithinWall
//->flock

class Fish:public Vehicle
{
public:
	Fish(b2World*world, Texture*texture);
	~Fish();
	virtual void setup(float x, float y);

	void flock(LinkedList<Fish*>*flock);
	void clearAllForce();
	void pursuePrey();

	Vector2D align(LinkedList<Fish*>&flock);
	Vector2D cohesion(LinkedList<Fish*>&flock);
	Vector2D separate(LinkedList<Fish*>&flock);
	Vector2D seekTarget(Vector2D&target);

	void addPreyTarget(Vector2D&target);
	bool stayWithinPool(float x, float y, float w, float h);
private:
	bool stayThere();

	LinkedList<Vector2D*>*preyTarget;
	float radarRadius;
	float delayTimeAfterSoughtTarget;
	float pursuePreyForce;
	ObjectData*data;
};

#endif // !FISH_H
