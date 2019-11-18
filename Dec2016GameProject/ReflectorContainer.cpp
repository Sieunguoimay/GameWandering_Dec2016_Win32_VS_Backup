#include "ReflectorContainer.h"




ReflectorContainer::ReflectorContainer(b2World * world)
	:world(world)
{
	reflectors = new LinkedList<Relector*>();
}

ReflectorContainer::~ReflectorContainer()
{
	for (Node<Relector*>*iter = reflectors->getFirstNode(); iter != reflectors->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete reflectors;
	reflectors = NULL;
}

void ReflectorContainer::setup(Texture*texture)
{
	this->texture = texture;
	angles[LEFT_UP] = -PI *0.25f;
	angles[LEFT_DOWN] = -PI*0.75f;
	angles[RIGHT_UP] = PI *0.25f;
	angles[RIGHT_DOWN] = -PI *1.25f;
}

void ReflectorContainer::display(const Camera & camera)
{
	for (Node<Relector*>*iter = reflectors->getFirstNode(); iter != reflectors->tail; iter = iter->next)
	{
		iter->data->display(camera);
		if (iter->data->isItTimeToDestroy())
		{
			delete iter->data;
			iter->data = NULL;
			iter = reflectors->delNode(iter);
		}
	}
}

void ReflectorContainer::addNewReflector(float x, float y, int ID)
{
	Relector*newReflector = new Relector(world,texture);
	newReflector->setup(x, y, angles[ID - int('a')]);
	if (newReflector)
		reflectors->addFirst(newReflector);
}

Relector * ReflectorContainer::getNewReflector()
{
	return this->reflectors->getFirstNode()->data;
}
