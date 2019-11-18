#pragma once
#ifndef CLOUD_H
#define CLOUD_H
#include"globalheader.h"
#include"Texture.h"
#include"LightningKiller.h"
//do we need separate and check egde???
//it will look more beautiful 
class Cloud
{
public:
	Cloud(b2World*world,Texture*cloudTexture,Texture*lightningTexture,Sound*sound,bool isDarkCloud);
	~Cloud();
	void setup(float x, float y,float h);
	void display(const Camera&camera);//override
	void blow(LinkedList<Cloud*>&others);
	
	void stayWithinValleyArea(float x, float y, float w, float h);
	bool isInsideCloudArea(Vector2D*position)const;
	void workOnNewTarget(Vector2D*position);
private:
	//void separate(LinkedList<Cloud*>&others);
	void applyForce(Vector2D&force);

	Vector2D align(LinkedList<Cloud*>&flock);
	Vector2D cohesion(LinkedList<Cloud*>&flock);
	Vector2D separate(LinkedList<Cloud*>&flock);
	Vector2D seek(Vector2D&target); 
	
	LightningKiller*lightning;
	LinkedList<Vector2D*>*targetForLightning;
	
	Vector2D*position;
	Vector2D*velocity;
	Vector2D*acceleration;
	
	float maxSpeed;
	float maxForce;
	
	Texture*cloudTexture;
	Size*size;
	Size*area;
	bool isDarkCloud;
};

#endif // !CLOUD_H
