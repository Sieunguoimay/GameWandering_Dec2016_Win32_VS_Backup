#pragma once
#include "Shape.h"
#include"SendMessage.h"
class Brick :
	public Shape
{
public:
	Brick(b2World*world,Size*size);
	~Brick();
	void setup(float x, float y,int typeBrick);
	b2Body*getBody();//to joint with some other things like trees
	const b2Vec2&getPosition()const;
	//first const is (const b2Vec2) a constant type - only constant type can store it
	//so we only use it to pass value out for another variable only - PASS VALUE
	//we use & operator to return the reference to the variable without create new memory location
	//to store it before give to other thing - therefore const-& is a good way to pass value
	//the second const is tell the function that there is no change value inside it
	const int&getType()const;
	//void setID(char newID);
	//put new item into container of pointes
	void setPointerTo(SendMessage*pointer);
private:
	Size*size;//tobe synchronous - with skin also use this pointer lying inside ground class
	void createBodyBrick(float x, float  y, float w, float h,bool isSensor);
	//there is only one body 
	//inside this class (derived from Shape)
	//destroyed together with this class
	int typeBrick;
	ObjectData*data;
	LinkedList<SendMessage*>*pointers;//container of pointers to items hung on this brick 
};

