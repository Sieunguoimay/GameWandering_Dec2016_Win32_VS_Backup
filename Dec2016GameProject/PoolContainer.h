#pragma once
#ifndef POOLCONTAINER_H
#define POOLCONTAINER_H
#include"Pool.h"
class PoolContainer
{
public:
	PoolContainer(b2World*world);
	~PoolContainer();
	void reset();
	void setup(Texture*poolTexture,Texture*fishTextrue,Texture*woodTexture);
	void display(const Camera&camera);
	void addPoolMark(float x, float y, float brickSize,int type);
	PoolMark*getNewPoolMark();
private:
	void createPool(float brickSize, int type);
	bool isThereAnyMark(float x, float y) const;
	LinkedList<PoolMark*>*poolMarks;
	LinkedList<Pool*>*pools;
	b2World*world;
	Texture*poolTexture;
	Texture*fishTexture;
	Texture*woodTexture;
};

#endif // !POOLCONTAINER_H
