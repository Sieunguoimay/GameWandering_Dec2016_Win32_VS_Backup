#pragma once
#ifndef EXPLOSION_H
#define EXPLOSION_H
#include"globalheader.h"
#include"ParticleExplosion.h"
#include"Sound.h"
//story:
//explosion is a class that includes a linkedlist of particle-system
//posision occur the explosion

class Explosion
{
public:
	Explosion();
	~Explosion();
	virtual void setup(Vector2D*position,Texture*texture,Sound*sound,bool mute = false);
	void display(const Camera&camera);
	bool isDead()const;
protected:
	virtual void putAParticleSystem(Texture&texture);
	Vector2D*position;
	LinkedList<ParticleExplosion*>*particles;
	Texture*texture;
	Color*color;
	Sound*soundEffect;
};
class Explosion2:public Explosion
{
public: 
	Explosion2();
	~Explosion2();
	void setup(Vector2D*position, Texture*texture, Sound*sound);
private:
	void putAParticleSystem(Texture&texture);

};
#endif // !EXPLOSION_H
