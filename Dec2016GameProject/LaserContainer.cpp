#include "LaserContainer.h"




LaserContainer::LaserContainer(b2World * world)
	:world(world)
{
	lasers = new LinkedList<Laser*>();
	photonContainer = new LinkedList<Photon*>();
}

LaserContainer::~LaserContainer()
{
	for (Node<Laser*>*iter = lasers->getFirstNode(); iter != lasers->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete lasers;
	lasers = NULL;

	for (Node<Photon*>*iter = photonContainer->getFirstNode(); iter != photonContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete photonContainer;
	photonContainer = NULL;
}

void LaserContainer::setup(Texture*texture1, Texture*texture2,Sound*sound)
{
	this->sound = sound;
	this->texture1 = texture1;
	this->texture2 = texture2;
}

void LaserContainer::display(const Camera & camera)
{
	for (Node<Laser*>*iter = lasers->getFirstNode(); iter != lasers->tail; iter = iter->next) {
		iter->data->display(camera);
		if (iter->data->isItTimeToDestroy())
		{
			delete iter->data;
			iter->data = NULL;
			iter = lasers->delNode(iter);
		}
	}

	for (Node<Photon*>*iter = photonContainer->getFirstNode(); iter != photonContainer->tail; iter = iter->next)
	{
		//what if they never collapse any objects, so it collapses with camera's edge
		if (iter->data->getPosition()->x > camera.getPosition().x + WINDOWSIZE_W*1.5f
			|| iter->data->getPosition()->x < camera.getPosition().x - WINDOWSIZE_W*0.5f)
			iter->data->collapse();
		if (iter->data->getPosition()->y > WINDOWSIZE_H*1.5f
			|| iter->data->getPosition()->y < -WINDOWSIZE_H*2.0f)
			iter->data->collapse();

		iter->data->display(camera);
		if (iter->data->isDead())
		{
			//printf("destroyed photon.%d\n",photonContainer->getSize());
			delete iter->data;
			iter->data = NULL;
			iter = photonContainer->delNode(iter);
		}
	}
}

void LaserContainer::addNewLaser(float x, float y)
{
	Laser *newLaser = new Laser(world, texture1, texture2,sound, photonContainer);
	if (newLaser) {
		newLaser->setup(x, y, 2);
		newLaser->start();
		lasers->addFirst(newLaser);
	}
}
Laser * LaserContainer::getNewLaser()
{
	return lasers->getFirstNode()->data;
}
