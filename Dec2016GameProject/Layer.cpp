#include "Layer.h"



Layer::Layer()
{
	distanceCoefficient = 0.0f;
}


Layer::~Layer()
{
	delete position;
	position = NULL;
	delete size;
	size = NULL;
	distanceCoefficient = 0.0f;
}

void Layer::setup(Vector2D * position, Size * size, float distanceCoefficient, Texture * texture)
{
	this->position = position;
	this->size = size;
	this->distanceCoefficient = distanceCoefficient;
	this->texture = texture;
}

void Layer::display(const Camera & camera)
{
	texture->render(
		(int)(position->x - size->w/2 - camera.getPosition().x*distanceCoefficient),
		(int)(position->y - size->h/2 - camera.getPosition().y*distanceCoefficient*0.5f),
		(int)size->w,(int)size->h,0);
}
