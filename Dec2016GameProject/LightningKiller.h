#pragma once
#ifndef LIGHTNINGKILLER_H
#define	LIGHTNINGKILLER_H
#include"globalheader.h"
#include"Camera.h"
#include"Texture.h"
#include"Sound.h"
class LightningKiller
{
public:
	LightningKiller(b2World*world, Texture*texture, Texture*texturePointer, Sound*sound);
	~LightningKiller();
	void display(const Camera&camera);
	void calculateDischarge(Vector2D&cloudPosition,Vector2D&target,bool beingShown);
private:
	void renderLine(Vector2D&pos1,Vector2D&pos2,const Camera&camera);
	b2Body*addNewParticleBody(float x, float y);

	LinkedList<b2Body*>*tinyParticles;
	LinkedList<Vector2D*>*path;
	
	Node<Vector2D*>*shutle ;
	Texture*texture;
	b2World*world;
	ObjectData*data;
	Texture*texturePointer;
	Sound*sound;
};

#endif // !LIGHTNINGKILLER_H
