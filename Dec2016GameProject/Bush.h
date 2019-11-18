#pragma once
#ifndef BUSH_H
#define BUSH_H

#include"globalheader.h"
#include "SendMessage.h"
class Bush :
	public SendMessage
{
public:
	Bush(Vector2D*position,int ID);
	~Bush();
	Vector2D*getPosition();
	int getID();
private:
	Vector2D*position;
	int ID;
};

#endif // !BUSH_H
