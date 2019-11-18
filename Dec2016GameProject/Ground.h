#pragma once


#ifndef GROUND_H
#define GROUND_H
#include"PerlinNoise.h"
#include"globalheader.h"
#include"Brick.h"
#include"Texture.h"
#include"Map.h"
#include"Camera.h"
#include"Bushes.h"
class Ground
{
public:
	Ground(b2World*world);
	~Ground();
	void setup(Texture*texture);
	void display(const Camera&camera);//reference to the dynamic data of Camera's Position 
	//to use data directly without copy
	const Size&getSize()const;
	void putNewBrick(float x, float y, int type,bool leftOrRight = false);//true is right - false is left
	void delBrick(bool leftOrRight = false);
	void setPointerTo(SendMessage*anyKindOfObject);
private:
	LinkedList<Brick*>*bricks;
	Texture*texture;
	Size*size;

	bool leftOrRight;
	PerlinNoise*perlinNoise;
	//pointers
	b2World*world;
};

#endif // !GROUND_H
