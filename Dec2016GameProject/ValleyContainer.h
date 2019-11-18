#pragma once
#ifndef VALLEYCONTAINER_H
#define VALLEYCONTAINER_H
#include"ThunderCloudValley.h"
class ValleyContainer
{
public:
	ValleyContainer(b2World*world);
	~ValleyContainer();
	void reset();
	void setup(Texture*cloudTexture, Texture*rainTexture, Texture*thunderTexture,Sound*thunderSound);
	void display(const Camera&camera);
	void addValleyMark(float x, float y,int type);
	ValleyMark*getNewMark();
	void del(bool leftOrRight);
private:
	bool isThereAnyMark(float x)const;
	LinkedList<WanderingCloudValley*>*valleys;
	LinkedList<ValleyMark*>*valleyMarks;
	Texture*cloudTexture; 
	Texture*rainTexture;
	Texture*thunderTexture;
	Sound*thunderSound;
	b2World*world;
};

#endif // !VALLEYCONTAINER_H
