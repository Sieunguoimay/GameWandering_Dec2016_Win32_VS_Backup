#pragma once
#ifndef DESTINATION_H
#define DESTINATION_H
#include"globalheader.h"
#include"SendMessage.h"
#include"Texture.h"
#include"Camera.h"
#include"LevelManager.h"
class DestinationBody : public SendMessage
{
public:
	DestinationBody(b2World*world,Texture*texture);
	~DestinationBody();
	void setup(float x, float y);
	void display(const Camera&camera);
	void setUserData(void*pointer);
private:
	Texture*texture;
	b2Body*body;
	Size*size;
	b2World*world;
};
class Destination
{
public:
	Destination(b2World*world,void*pointer);
	~Destination();
	void setup(Texture*textureDestination, Texture*textureNextLevel);
	void display(const Camera&camera);
	void addNewDestination(float x, float y);
	DestinationBody*getNewDestinationBody();
	void del(bool leftOrRight);
private:
	LinkedList<DestinationBody*>*destination;
	Texture*textureDestination;
	b2World*world;
	ObjectData*data;
};

#endif // !DESTINATION_H
