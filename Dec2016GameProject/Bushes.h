#pragma once

#ifndef BUSHES_H
#define BUSHES_H
#include"globalheader.h"
#include"Texture.h"
#include"Camera.h"
#include"Bush.h"
class Bushes//:public SendMessage
{
public:
	Bushes();
	~Bushes();
	virtual void setup(Texture*texture);
	void display(const Camera&camera);
	void plant(float x, float y,int ID);
	Bush*getNewBush();
protected:
	LinkedList<Bush*>*bushContainer;
	Texture*skins;
	Size*size;
};

#endif // !BUSH_H
