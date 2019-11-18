#pragma once
#ifndef VEHICLE_H
#define VEHICLE_H

#include"globalheader.h"
#include"Camera.h"
#include"Texture.h"
//abilities of this vihicle are:
//seeking a given target
//applyForce of environment
//separate

//a bird will has additional abilities:
//follow path

//a fish will has additional abilities:
//aligement
//cohension
//stayWithinWall
//->flock

class Vehicle
{
public:
	Vehicle(b2World*world, Texture*texture);
	~Vehicle();
	virtual void setup(float x, float y);
	void display(const Camera&camera);
	void applyForce(Vector2D&force);
	bool seek(Vector2D&target, float maxForce = 1.0f);
	void separate(LinkedList<Vehicle*>*otherVehicles);
	Vector2D*getPosition();
protected:
	void updateMovement();
	virtual void createBody();//so that you can adjust the shape of that body

	Vector2D*position;
	Vector2D*velocity;
	Vector2D*acceleration;
	
	float maxForce;//constant
	float maxSpeed;//constant
	float mass;//constant
	float angle;//temporary variable
	bool bodyCreated;
	Size*size;
	Texture*texture;

	b2World*world;
	b2Body*body;
};

#endif // VEHICLE_H
