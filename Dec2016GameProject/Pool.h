#pragma once
#ifndef POOL_H
#define POOL_H
#include"globalheader.h"
#include"Camera.h"
#include"Texture.h"
#include"SendMessage.h"
#include"Fish.h"
class PoolMark :public SendMessage
{
public:
	PoolMark(Vector2D*position) 
		:position(position){
		state = false;//single
	}
	~PoolMark() {
		delete position;
		state = false;
	}
	Vector2D*position;
	bool state;
};
typedef std::pair<b2Body*, Size*> Wood;
typedef std::pair<Node<PoolMark*>*, Node<PoolMark*>*>PoolMarkPair;
class Pool
{
public:
	friend class PoolContainer;
	//for saved we use pointer
	Pool(b2World*world, PoolMarkPair*poolMarkPair,float sizeError);
	~Pool();
	void setup(Texture*texture, Texture*woodTexture,Texture*fishTexture,bool includedWoods, int*pointerToNumNodes, bool isThereFish = true);
	void display(const Camera&camera);
	void display2(const Camera&camera);

	void findOutMoverUnderWater(Vector2D*position);
	//object inside this object will under a force of water
	bool checkInside(Vector2D&objectPosition)const;
	//as both of the mark reach the dead point then this method return true
	bool isDead()const;
private:
	void createBox();
	void addWoods();
	b2Body*createBodyBox(float x, float y, float w, float h);

	Vector2D*position1;
	Size*size;
	//LinkedList<FloatingObject*>*floatingObjects;
	Texture*foreLayer;//a white rectangular texture
	Texture*woodTexture;
	
	b2Body*body;
	LinkedList<Wood*>*woods;
	LinkedList<Fish*>*flockOfFish;
	//used for blitting scaling to fit the rectangle area
	//pointers
	PoolMarkPair*poolMarkPair;
	int*pointerToNumNodes;
	b2World*world;
	ObjectData*data;
	ObjectData*data2;
};

#endif // !POOL_H
