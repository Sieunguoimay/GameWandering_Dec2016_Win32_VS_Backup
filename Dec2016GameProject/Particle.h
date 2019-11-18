#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H
#include"globalheader.h"
#include"Texture.h"
#include"Camera.h"
class Particle
{
public:
	Particle();
	~Particle();
	void setup(Vector2D*position,Vector2D*velocity,Size*Size,Texture*texture,int lifespan = 255,int speedOfLifespan = 7);
	virtual void display(const Camera&camera);
	void applyForce(Vector2D&force);
	bool isDead();
protected:
	virtual void update();
	void updateParticle();
	Vector2D*position;
	Vector2D*velocity;
	Vector2D*accelaration;
	Texture*texture;
	Size*size;
	int lifespan;
	int speedOfLifespan;
	float maxSize;
};

#endif // !PARTICLE_H
