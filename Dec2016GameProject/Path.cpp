#include "Path.h"



Path::Path()
{
	position = new Vector2D();
	paths = new LinkedList<Vector2D*>();
}

Path::~Path()
{
	for (Node<Vector2D*>*iter = paths->getFirstNode()->next; iter != paths->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete paths;
	paths = NULL;
	delete position;
	position = NULL;
}

void Path::setup(Vector2D & position)
{
	this->radius = 20.0f;
	this->position->set(position);
	setupInCircle();
}

LinkedList<Vector2D*>* Path::getPaths()
{
	return this->paths;
}

void Path::setupInCircle()
{
	float angle = 0.0f;
	while(angle<3.141592f*2.0f) {
		angle += PI / 6.0f;
		Vector2D*newVertice = UtilityFunctions::polarToCartesian(*position,50.0f, angle);
		paths->addFirst(newVertice);
	}
	paths->addFirst(paths->tail->prev->data);
}
