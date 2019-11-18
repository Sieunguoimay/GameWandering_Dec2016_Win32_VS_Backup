#pragma once
#ifndef PHOTON_H
#define PHOTON_H
#include "Lightning.h"
class Photon :
	public ParticleSystem
{
public:
	friend class Relector;
	Photon(b2World*world);
	~Photon();
	void setup(Vector2D*position, Vector2D*velocity, Size*Size, Texture*texture);// , int lifespan = 255, int speedOfLifespan = 7);
	void collapse();
	bool hasCollapsed()const;
	void display(const Camera&camera);
	Vector2D*getPosition();
private:
	bool collapsed;
	b2Body*body;
	b2World*world;
	Color*color;
	ObjectData*data;
};

#endif // !PHOTON_H
