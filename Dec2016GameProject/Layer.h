#pragma once
#ifndef LAYER_H
#define LAYER_H
#include"globalheader.h"
#include"Texture.h"
class Layer
{
public:
	Layer();
	~Layer();
	void setup(Vector2D*position,Size*size,float distanceCoefficient,Texture*texture);
	void display(const Camera&camera);
private:
	Vector2D*position;
	Size*size;
	Texture*texture;
	float distanceCoefficient;//smaller than or equal to 1
};

#endif // !LAYER_H
