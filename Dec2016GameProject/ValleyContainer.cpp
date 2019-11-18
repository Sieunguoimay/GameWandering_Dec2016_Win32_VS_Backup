#include "ValleyContainer.h"



ValleyContainer::ValleyContainer(b2World*world)
	:world(world)
{
	valleys = new LinkedList<WanderingCloudValley*>();
	valleyMarks = new LinkedList<ValleyMark*>();
}

ValleyContainer::~ValleyContainer()
{
	for (Node<WanderingCloudValley*>*iter = valleys->getFirstNode(); iter != valleys->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete valleys;
	valleys = NULL;
	for (Node<ValleyMark*>*iter = valleyMarks->getFirstNode(); iter != valleyMarks->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete valleyMarks;
	valleyMarks = NULL;
}

void ValleyContainer::reset()
{
	for (Node<WanderingCloudValley*>*iter = valleys->getFirstNode(); iter != valleys->tail; iter = iter->next)
	{
		iter->data->destroyValley();
		iter->data = NULL;
		iter = valleys->delNode(iter);
	}
	for (Node<ValleyMark*>*iter = valleyMarks->getFirstNode(); iter != valleyMarks->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
		iter = valleyMarks->delNode(iter);
	}
}

void ValleyContainer::setup(Texture * cloudTexture, Texture * rainTexture,
	Texture*thunderTexture, Sound*thunderSound)
{
	this->cloudTexture = cloudTexture;
	this->rainTexture = rainTexture;
	this->thunderTexture = thunderTexture;
	this->thunderSound = thunderSound;
}

void ValleyContainer::display(const Camera & camera)
{
	for (Node<WanderingCloudValley*>*iter = valleys->getFirstNode(); iter != valleys->tail; iter = iter->next)
	{
		iter->data->display(camera);
		if (iter->data->isDead()) {
			iter->data->destroyValley();//the problem of delete operator overide was solved by this virtual distroy function
			iter->data = NULL;
			iter = valleys->delNode(iter);
		}
	}
	for (Node<ValleyMark*>*iter = valleyMarks->getFirstNode(); iter != valleyMarks->tail; iter = iter->next) {
		if (iter->data->isItTimeToDestroy() && (iter->data->state == false)) {
			delete iter->data;
			iter->data = NULL;
			iter = valleyMarks->delNode(iter);
		}
	}
}

void ValleyContainer::addValleyMark(float x, float y, int type)
{
	if (!isThereAnyMark(x)) {
		ValleyMark*newValleyMark = new ValleyMark(new Vector2D(x, y));
		if (newValleyMark)
			valleyMarks->addFirst(newValleyMark);

		if (valleyMarks->getSize() % 2 == 0 && (!valleyMarks->isEmpty())) {
			ValleyMarkPair*newValleyMarkPair = new ValleyMarkPair(valleyMarks->getFirstNode(),
				valleyMarks->getFirstNode()->next);
			WanderingCloudValley*newThunderCloudValley = NULL;
			if (type == 1)
			{
				newThunderCloudValley = new WanderingCloudValley(newValleyMarkPair, &valleyMarks->numNodes);
				newThunderCloudValley->setup(cloudTexture, rainTexture);
			}
			else if (type == 2)
			{
				newThunderCloudValley = new ThunderCloudValley(world, newValleyMarkPair, &valleyMarks->numNodes);
				((ThunderCloudValley*)newThunderCloudValley)->setup(cloudTexture, rainTexture,
					thunderTexture, thunderSound);
			}
			valleys->addFirst(newThunderCloudValley);
		}
	}
}

ValleyMark * ValleyContainer::getNewMark()
{
	return valleyMarks->getFirstNode()->data;
}

bool ValleyContainer::isThereAnyMark(float x) const
{
	for (Node<ValleyMark*>*iter = valleyMarks->getFirstNode(); iter != valleyMarks->tail; iter = iter->next)
		if (iter->data->position->x == x) {
			iter->data->shouldBeDestroyed = false;
			return true;
		}
	return false;
}

