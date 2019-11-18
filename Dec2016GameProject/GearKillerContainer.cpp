#include "GearKillerContainer.h"



GearKillerContainer::GearKillerContainer(b2World*world)
	:world(world)
{
	gearKillerContainer = new LinkedList<GearKillerMachine*>();
}


GearKillerContainer::~GearKillerContainer()
{
	for (Node<GearKillerMachine*>*iter = gearKillerContainer->getFirstNode(); iter != gearKillerContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete gearKillerContainer;
	gearKillerContainer = NULL;
}

void GearKillerContainer::reset()
{
	for (Node<GearKillerMachine*>*iter = gearKillerContainer->getFirstNode(); iter != gearKillerContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
		iter = gearKillerContainer->delNode(iter);
	}
}

void GearKillerContainer::setup(Texture * gearTexture, Texture * stickTexture,Mover*mover)
{
	this->gearTexture = gearTexture;
	this->stickTexture = stickTexture;
	this->moverPointer = mover;
}

void GearKillerContainer::display(const Camera & camera)
{
	for (Node<GearKillerMachine*>*iter = gearKillerContainer->getFirstNode(); iter != gearKillerContainer->tail; iter = iter->next)
	{
		iter->data->display(camera);

		//story has been changed
		/*if (iter->data->getPosition()->x + 200<camera.getPosition().x
			|| iter->data->getPosition()->x-200>camera.getPosition().x+camera.getSize().w ) {//this rule is so uneffictive
			//delete it
			delete iter->data;
			iter = gearKillerContainer->delNode(iter);
		}*/
		if (iter->data->isItTimeToDestroy()) {
			delete iter->data;
			iter->data = NULL;
			iter = gearKillerContainer->delNode(iter);
		}

	}
}

void GearKillerContainer::addNewKiller(float x, float y, int numberGear)
{
	for (Node<GearKillerMachine*>*iter = gearKillerContainer->getFirstNode(); iter != gearKillerContainer->tail; iter = iter->next)
		if (iter->data->getPosition()->x == x&& iter->data->getPosition()->y == y)
			return;
	GearKillerMachine*newKiller = new GearKillerMachine(world, moverPointer, new Vector2D(x, y), gearTexture, stickTexture,numberGear);
	if (newKiller)
		gearKillerContainer->addFirst(newKiller);
}

GearKillerMachine * GearKillerContainer::getNewKiller()
{
	return gearKillerContainer->getFirstNode()->data;
}
