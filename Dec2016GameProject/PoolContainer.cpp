#include "PoolContainer.h"



PoolContainer::PoolContainer(b2World*world)
	:world(world)
{
	poolMarks = new LinkedList<PoolMark*>();
	pools = new LinkedList<Pool*>();
}

PoolContainer::~PoolContainer()
{
	for (Node<Pool*>*iter = pools->getFirstNode(); iter != pools->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete pools;
	pools = NULL;

	for (Node<PoolMark*>*iter = poolMarks->getFirstNode(); iter != poolMarks->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete poolMarks;
	poolMarks = NULL;
}

void PoolContainer::reset()
{

	for (Node<Pool*>*iter = pools->getFirstNode(); iter != pools->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
		iter = pools->delNode(iter);
	}
	for (Node<PoolMark*>*iter = poolMarks->getFirstNode(); iter != poolMarks->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
		iter = poolMarks->delNode(iter);
	}
}

void PoolContainer::setup(Texture * poolTexture, Texture * fishTextrue, Texture * woodTexture)
{
	this->poolTexture = poolTexture;
	this->fishTexture = fishTextrue;
	this->woodTexture = woodTexture;
	
}

void PoolContainer::display(const Camera & camera)
{
	for (Node<Pool*>*iter = pools->getFirstNode(); iter != pools->tail; iter = iter->next) {
		iter->data->display(camera);

		if (iter->data->isDead()) {
			delete iter->data;
			iter->data = NULL;
			iter = pools->delNode(iter);
		}
	}
	//if any single mark is called in dead function then it must be destroyed
	for (Node<PoolMark*>*iter = poolMarks->getFirstNode(); iter != poolMarks->tail; iter = iter->next)
		if (iter->data->state == false && iter->data->isItTimeToDestroy())//single
		{
			delete iter->data;
			iter->data = NULL;
			iter = poolMarks->delNode(iter);
		}
}
//the story tells that:
//when ever the addPoolMark function is called:
//it check the values passed in isThereAnyMark there - at the position x,y
//if the answer is yes.. then the value is invalided, and then be ignored
//if the answer is no> then a new pool mark will be created with the given state is single and then
// the function create Pool is called
//inside that createPool function, it checks wheather enough pool mark for a pool or not
//if true then a new pool will be created and the passed-in poolMarks will change it own state to be married - true
// that is all the story here
//at another place, the function reset is made to make sure that the containers of poolmark and pool are empty for each new era
//and inside the display() function, we add a loop of PoolMark container to make sure that
//is there any poolmark is called to die but still single. if any, it must be destroyed
void PoolContainer::addPoolMark(float x, float y, float brickSize, int type)
{
	if (!isThereAnyMark(x, y)) {
		PoolMark*newPoolMark = new PoolMark(new Vector2D(x, y));
		if (poolMarks) {
			poolMarks->addFirst(newPoolMark);
			createPool(brickSize, type);
		}
	}

}

PoolMark * PoolContainer::getNewPoolMark()
{
	return poolMarks->getFirstNode()->data;
}

void PoolContainer::createPool(float brickSize, int type)
{
	if ((poolMarks->getSize() % 2 == 0) && (!poolMarks->isEmpty())) {
		Pool*newPool = new Pool(
			world, new PoolMarkPair(
				poolMarks->getFirstNode()->next,
				poolMarks->getFirstNode()), brickSize);
		if (type == 1)
			newPool->setup(poolTexture, woodTexture, fishTexture, false, &poolMarks->numNodes,true);
		else if (type == 2)
			newPool->setup(poolTexture, woodTexture, fishTexture, true, &poolMarks->numNodes,true);
		else if (type == 3)
			newPool->setup(poolTexture, woodTexture, fishTexture, false, &poolMarks->numNodes, false);
		pools->addFirst(newPool);
	}
}

bool PoolContainer::isThereAnyMark(float x, float y) const
{
	for (Node<PoolMark*>*iter = poolMarks->getFirstNode(); iter != poolMarks->tail; iter = iter->next)
		if (iter->data->position->x == x) {
			iter->data->shouldBeDestroyed = false;
			return true;
		}
	return false;
}
