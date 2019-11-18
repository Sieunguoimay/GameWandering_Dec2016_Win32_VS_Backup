#include "Bush.h"




Bush::Bush(Vector2D * position, int ID)
	:position(position),ID(ID)
{
}
//what the fuck is going on the hostle baby
//what should i do now???

Bush::~Bush()
{
	delete position;
	ID = 0;
}

Vector2D * Bush::getPosition()
{
	return this->position;
}

int Bush::getID()
{
	return this->ID;
}
