#pragma once
#ifndef MONSTER_TOMATO_H
#define MONSTER_TOMATO_H
#include"globalheader.h"
#include"Texture.h"
#include"Camera.h"
#include"Mover.h"
#include"Shape.h"
#include"Explosion.h"
#include"SendMessage.h"

class MonsterTomato:public Shape,public SendMessage
{
public:
	MonsterTomato(b2World*world);
	~MonsterTomato();
	void setup(float x, float y,Size*size,Texture*texture, Texture*textureExplosion,Sound*sound, Sound*sound2);
	void seek(Mover*mover);
	void display(const Camera&camera);
	bool isDead()const;
	bool isOutOfScreen()const;
	void touchMover();
	Vector2D*getPosition();
	static int numberMonster;
private:
	void applyForce(Vector2D*force);
	void grab();//update sprites

	b2Body*body;
	ObjectData*data;

	Sound*sound;
	Sound*sound2;

	Vector2D*position;
	Vector2D*velocity;
	Vector2D*target;
	Vector2D*desired;

	float maxForce;
	float maxSpeed;

	int timePerReaching;
	bool findNewTarget;
	int currentSpriteIndex;
	bool dead;
	bool outOfScreen;
	
	bool flip;
	float oldPosX;

	Texture*skin;//pointer
	Texture*textureExplosion;//pointer
	Size*size;
};

#endif // !MONSTER_TOMATO_H
