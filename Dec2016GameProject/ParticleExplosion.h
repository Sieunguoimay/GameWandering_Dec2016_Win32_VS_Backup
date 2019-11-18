#pragma once
#ifndef PARTICLEEXPLOSION_H
#define PARTICLEEXPLOSION_H
#include"globalheader.h"
#include"Texture.h"
#include"Camera.h"
class ParticleExplosion
{
public:
	ParticleExplosion(float x, float y,Texture*texture,Color*color);
	~ParticleExplosion();
	void display(const Camera&camera);
	bool done()const;
	void adjust(float damping, int lifespan);
private:
	void calculateVel();
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;

	Texture*texture;
	Color*color;
	float size;
	float damping;
	int lifeSpan;
};

#endif // !PARTICLEEXPLOSION_H
