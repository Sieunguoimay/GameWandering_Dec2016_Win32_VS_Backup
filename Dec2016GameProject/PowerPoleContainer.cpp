#include "PowerPoleContainer.h"



PowerPoleContainer::PowerPoleContainer(b2World*world, Mover*moverPointer)
	:world(world),moverPointer(moverPointer)
{
	powerPoles = new LinkedList<PowerPole*>();
	flocksOfBirds = new LinkedList<FlockOfBird*>();
}


PowerPoleContainer::~PowerPoleContainer()
{
	for (Node<PowerPole*>*iter = powerPoles->getFirstNode(); iter != powerPoles->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete powerPoles;
	powerPoles = NULL;
	for (Node<FlockOfBird*>*iter = flocksOfBirds->getFirstNode(); iter != flocksOfBirds->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete flocksOfBirds;
	flocksOfBirds = NULL;
}

void PowerPoleContainer::setup(Texture * birdTexture, Texture * poleTexture, Texture*textureExp, SoundContainer*soundCollection)
{
	this->textureExp = textureExp;
	this->birdTexture = birdTexture;
	this->poleTexture = poleTexture;
	this->soundCollection = soundCollection;
}

void PowerPoleContainer::display(const Camera & camera)
{
	for (Node<PowerPole*>*iter = powerPoles->getFirstNode(); iter != powerPoles->tail; iter = iter->next)
	{
		iter->data->display(camera);
		if (iter->data->isItTimeToDestroy()) {
			delete iter->data;
			iter->data = NULL;
			iter = powerPoles->delNode(iter);//ok nhe.
		}
	}
	for (Node<FlockOfBird*>*iter = flocksOfBirds->getFirstNode(); iter != flocksOfBirds->tail; iter = iter->next) {
		iter->data->display(camera, *moverPointer);
		if (iter->data->isDead()) {
			delete iter->data;
			iter->data = NULL;
			iter = flocksOfBirds->delNode(iter);
		}
	}
}

//there is only one small bug when putting a power pole at the begin initialization of the map
//so before I can fix it. It's better not to put any power pole at begining. OK
//from tomorrow, let's create a water pool with an other kind of creature - fishesssssssss////
void PowerPoleContainer::putNewPowerPole(float x, float y, float ID)
{
	PowerPole*newPowerPole = new PowerPole(world);
	newPowerPole->setup(new Vector2D(x, y), poleTexture);
	if (newPowerPole)
		powerPoles->addFirst(newPowerPole);
	bool isFlockThere = false;
	for (Node<FlockOfBird*>*iter = flocksOfBirds->getFirstNode(); iter != flocksOfBirds->tail; iter = iter->next) {
		if (iter->data->getID() == ID) {
			isFlockThere = true;
			break;
		}
	}
	if (!isFlockThere) {
		FlockOfBird*newFlock = new FlockOfBird(world, birdTexture,textureExp,soundCollection);
		if (newFlock) {//to be safe
			newFlock->setup(newPowerPole->getHeight(), newPowerPole->getPosition2(), ID);
			flocksOfBirds->addFirst(newFlock);
		}
	}
	//too many bugs - fuck off
}

PowerPole * PowerPoleContainer::getNewPowerPole()
{
	return powerPoles->getFirstNode()->data;
}
