#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H
#include"globalheader.h"
#include"Ground.h"
#include"SoundContainer.h"
#include"Bushes.h"
#include"GearKillerContainer.h"
#include"Mover.h"
#include"Monster_Tomatoes.h"
#include"Destination.h"
#include"LevelManager.h"
#include"LaserContainer.h"
#include"ReflectorContainer.h"
#include"PowerPoleContainer.h"
#include"PoolContainer.h"
#include"BridgeContainer.h"
#include"ValleyContainer.h"

class Ecosystem{
public:
	Ecosystem(b2World*world,std::vector<Texture*>*textureCollection,
	SoundContainer*soundCollection,Map*map,Mover*mover,LevelManager*levelManager);
	~Ecosystem();
	void display(const Camera&camera);
private:
	void putEverythingAtFirst();
	void updateLandscape(float cameraPosX);
	void putEverything(float x,float y, char ID, bool leftOrRight = false);
	void delEverything(char ID,bool leftOrRight);
	bool conditionToPutBush(char&ID);
	
	PowerPoleContainer*powerPoles;
	PoolContainer*pools;
	ValleyContainer*valleys;

	BridgeContainer*bridges;
	
	ReflectorContainer*reflectors;
	LaserContainer*lasers;
	Ground*ground;
	Bushes*bushes;
	Trees*trees;
	GearKillerContainer*gearKillers;
	Monster_Tomatoes*monsters;
	Destination*destination;
	
	std::vector<Texture*>*textureCollection;
	SoundContainer*soundCollection;
	Map*map;
	b2World*world;
	Mover*mover;
};

#endif //ECOSYSTEM_H
