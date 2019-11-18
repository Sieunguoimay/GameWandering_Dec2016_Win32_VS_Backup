#pragma once
#ifndef MONSTER_TOMATOES_H
#define MONSTER_TOMATOES_H
#include"Monster_Tomato.h"
#include"Camera.h"
#include"globalheader.h"
#include"Mover.h"

//what the fuck is this class??
//it includes a LinkedList of Monster_Tomato-es 
//a linkedList of explosions - when ever a moster die - a explosion will be added into that container
//a vector to store textures - one for monster and one for explosion
//a vector container to store sounds used as a source for pointer 

class Monster_Tomatoes
{
public:
	Monster_Tomatoes(b2World*world);
	~Monster_Tomatoes();
	void reset();
	void setup(Texture*texture1, Texture*texture2,Mover*mover, Sound*soundExplosion, Sound*soundSwallow);
	void display(const Camera&camera);
	void putNewMonster(float x, float y,float ID);
	bool checkID(float ID);//is this new ID in collection or not
	MonsterTomato*getNewMonster();
private:
	
	void putNewExplosion(Vector2D*position);
	LinkedList<Explosion*>*explosionContainer;
	LinkedList<ObjectData*>*monsterContainer;
	std::vector<Texture*>*textureContainer;
	Sound*soundExplosion;
	Sound*soundSwallow;
	Vector2D*target;
	Size*size;
	b2World*world;
	Mover*mover;
};

#endif // !MONSTER_TOMATOES_H
