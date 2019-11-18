#include "PowerPole.h"



PowerPole::PowerPole(b2World*world)
	:world(world)
{
	size = new Size();
	position2 = new Vector2D();
}

PowerPole::~PowerPole()
{
	//delete birdsManagedByThis;//this linkedlist is just a pointer - keeper to manage this own bird 
	//and all the bird kept by this is destroyed outside by their container put in PowerPoleContainer
	//that why we don't allow to detroy them here
	//Now I change my mind
	delete position1;
	position1 = NULL;
	delete position2;
	position2 = NULL;
	delete size;
	size = NULL;
	texture = NULL;
}

void PowerPole::setup(Vector2D*position, Texture * texture)
{
	this->position1 = position;
	this->texture = texture;
	float w = 150.0f;
	size->set(w, texture->getSize()->h*w/texture->getSize()->w);
	position2->set(position1->x, position1->y - size->h);
}

void PowerPole::display(const Camera & camera)
{
	texture->render(camera,
		position2->x - size->w*0.5f - camera.getPosition().x,
		position2->y - camera.getPosition().y,
		size->w, size->h);

	
}

Vector2D & PowerPole::getPosition2()
{
	// TODO: insert return statement here
	return *(this->position2);
}

float PowerPole::getHeight() const
{
	return size->h;
}

