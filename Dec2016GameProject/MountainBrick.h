#pragma once
#ifndef MOUNTAIN_BRICK_H
#define MOUNTAIN_BRICK_H
#include"globalheader.h"
#include"Texture.h"
#include"Camera.h"
class MountainBrick
{
public:
	MountainBrick(float x, float y,float w, float h,Texture*texture);
	~MountainBrick();
	void display(const Camera&camera, float cameraRate);
	Rectangle&getRectangle();
private:
	Rectangle rectangle;
	Texture*texture;
};

#endif // !MOUNTAIN_BRICK_H
