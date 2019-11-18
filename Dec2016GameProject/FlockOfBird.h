#pragma once
#ifndef FLOCKOFBIRD_H
#define FLOCKOFBIRD_H
#include"Bird.h"
#include"Mover.h"
#include"Explosion.h"
class FlockOfBird
{
public:
	FlockOfBird(b2World*world,Texture*texture, Texture*textureExp,SoundContainer*soundCollection);
	~FlockOfBird();
	void setup(float radarRadius,Vector2D&position, float ID);
	void display(const Camera&camera,Mover&moverPointer);
	bool isDead()const;
	float getID()const;
private:
	void createNewBird(float radarRadius, Vector2D&position);
	LinkedList<Explosion*>*explosions;
	LinkedList<Bird*>*flock;
	SoundContainer*soundCollection;
	Path*path;
	float ID;
	b2World*world;
	Texture*texture;
	Texture*textureExp;
};

#endif // !FLOCKOFBIRD_H
