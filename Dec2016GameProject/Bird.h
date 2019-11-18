#pragma once
#ifndef BIRD_H
#define BIRD_H

#include "Vehicle.h"
#include"Path.h"
#include"Sound.h"
//this Bird class derives the base class - Vehicle
class Bird :
	public Vehicle
{
public:
	Bird(b2World*world,Texture*texture, Path**path,Sound*pursueSound,Sound*deadSound);
	~Bird();
	virtual void setup(float x, float y);
	void fly();
	void followPath(Path*path);
	float radar(Vector2D&moverPosition);
	void pursuePrey();
	void setRadarRadius(float radius);
	Vector2D*getPosition();
	bool isDead()const;
	void die(bool dieOfOut = false);
private:
	bool stayThere();
	Vector2D getNormalPoint(const Vector2D&predicLocation, const  Vector2D&vertice1, const Vector2D&vertice2);
	//pointer to the path
	Path**path;
	LinkedList<Vector2D*>*preyTarget;
	bool dead;
	bool pursueSoundPlayed;
	float radarRadius;
	float delayTimeAfterSoughtTarget;
	float pursuePreyForce;
	ObjectData*data;
	Sound*pursueSound;
	Sound*deadSound;
};

#endif // !BIRD_H
