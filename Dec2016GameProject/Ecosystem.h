#pragma once
#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H
#include"globalheader.h"

#include"Ground.h"

#include"Camera.h"

#include"Bushes.h"

#include"Trees.h"

#include"Monster_Tomatoes.h"

#include"GearKillerContainer.h"

#include"LaserContainer.h"

#include"ReflectorContainer.h"

#include"BridgeContainer.h"

#include"PowerPoleContainer.h"

#include"PoolContainer.h"

#include"ValleyContainer.h"

#include"Destination.h"

#include"LevelManager.h"

#include"SoundContainer.h"
class Ecosystem
{
public:
	Ecosystem(b2World*world, Mover*mover,LevelManager**pointer,
		std::vector<Texture*>**textureColection, SoundContainer*soundCollection);
	~Ecosystem();
	void setup(SDL_Renderer ** renderer);
	void reset();
	void display(const Camera&camera);
	void display2(const Camera&camera);//display forelayers
	Map*getMapPointer();
private:

	void updateLandscape(float cameraPosX);
	void putEverything(float x,float y, char ID, bool leftOrRight = false);

	void putEverythingAtFirst();
	bool conditionToPutBush(char&ID);
	
	void loadTexture(SDL_Renderer**renderer);
	
	Ground*ground;
	Bushes*bushes;
	Trees*trees;
	Monster_Tomatoes*monster_tomatoes;
	GearKillerContainer*gearKillers;
	LaserContainer*lasers;
	ReflectorContainer*reflectors;

	BridgeContainer*bridges;
	PowerPoleContainer*powerPoles;
	PoolContainer*pools;
	ValleyContainer*valleys;	
	Destination*destination;
	

	std::vector<Texture*>*textureCollection;
	SoundContainer*soundCollection;
	int textureNumber;
	Map*map;
	
	Mover*mover;
	b2World*world;
};

#endif // !ECOSYSTEM_H
