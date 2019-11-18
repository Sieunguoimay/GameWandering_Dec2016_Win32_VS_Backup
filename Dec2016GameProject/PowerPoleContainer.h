#pragma once
#ifndef POWERPOLECONTAINER_H
#define POWERPOLECONTAINER_H
#include"globalheader.h"
#include"Camera.h"
#include"Texture.h"
#include"PowerPole.h"
#include"Mover.h"
#include"FlockOfBird.h"
//this class is a container of power poles
//and includes a linkedList of Birds

class PowerPoleContainer
{
public:
	PowerPoleContainer(b2World*world, Mover*moverPointer);
	~PowerPoleContainer();
	void setup(Texture*birdTexture,Texture*poleTexture, Texture*textureExp, SoundContainer*soundCollection);
	void display(const Camera&camera);
	void putNewPowerPole(float x, float y, float ID);
	PowerPole* getNewPowerPole();
private:
	LinkedList<PowerPole*>*powerPoles;
	LinkedList<FlockOfBird*>*flocksOfBirds;
	SoundContainer*soundCollection;
	//some Pointers
	b2World*world;
	Texture*birdTexture;
	Texture*poleTexture;
	Texture*textureExp;
	//bird texture
	//pole texture

	Mover*moverPointer;//mover pointer to give birds the target
};

#endif // !POWERPOLECONTAINER_H
