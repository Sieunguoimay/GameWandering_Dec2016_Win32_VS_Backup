#pragma once
#ifndef SHAPE_H
#define SHAPE_H
#include"globalheader.h"
class Shape
{
public:
	Shape(b2World*world);
	~Shape();
	void display(SDL_Renderer**renderer);
	b2Body* addRectangle(float x, float y, int w, int h, bool dyn = false , bool fixedRotation = false,float density = 1, bool isSensor = false);
	b2Body* addCircle(float x, float y, float radius, bool dyn,float density = 1.0f,bool fixedRotate = false,bool isSensor = false);
	
	//the fucking functons used to display only should be removed soon
	void drawRectangle(SDL_Renderer**renderer,b2Vec2*points, b2Vec2 center, float angle);
	void drawCircle(SDL_Renderer**renderer,b2Vec2 center,float redius,float angle);
	void rotationTranslate(b2Vec2&vector, b2Vec2 center, float angle);

	void connectRevolution(b2Body*bodyA, b2Body*bodyB);
	void connectDistance(b2Body * bodyA, b2Body * bodyB,float frequency = 2.0f);
protected://any object derive this class has this 2 prorerties
	b2Body*body;
	b2World*world;
};

#endif // !SHAPE_H
