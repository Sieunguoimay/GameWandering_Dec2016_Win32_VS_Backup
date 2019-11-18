#include "FlockOfBird.h"



FlockOfBird::FlockOfBird(b2World*world, Texture*texture, Texture*textureExp, SoundContainer*soundCollection)
	:world(world),texture(texture),soundCollection(soundCollection),textureExp(textureExp)
{
	flock = new LinkedList<Bird*>();
	path = new Path();
	explosions = new LinkedList<Explosion*>();
}


FlockOfBird::~FlockOfBird()
{
	for (Node<Bird*>*iter = flock->getFirstNode(); iter != flock->tail->prev->next; iter = iter->next) {
		delete iter->data;
		iter->data = NULL;
	}
	delete flock;
	flock = NULL;
	for (Node<Explosion*>*iter = explosions->getFirstNode(); iter != explosions->tail; iter = iter->next) {
		delete iter->data;
		iter->data = NULL;
	}
	delete explosions;
	explosions = NULL;

	delete path;
	path = NULL;
	ID = 0;
	texture = NULL;
	textureExp = NULL;
}
void FlockOfBird::setup(float radarRadius, Vector2D & position, float ID)
{
	this->ID = ID;
	path->setup(position);
	Node<Bird*>*headNext = flock->getFirstNode();
	
	int randomNumOfBirdsInAFlock = rand() % 5 + 3;//3-8 birds, i think, are ok
	for (int i = 0; i < randomNumOfBirdsInAFlock; i++)
		createNewBird(radarRadius,position);//create new bird and put it into the linkedlist<Bird*>() which is a container pointer itself

}
void FlockOfBird::display(const Camera&camera, Mover&moverPointer)
{
	float moverRadarRaiusSqr = moverPointer.getRadarRadius()*moverPointer.getRadarRadius();//square of 250px - mover->getRadarRadius();
	for (Node<Bird*>*iter = flock->getFirstNode(); iter != flock->tail->prev->next; iter = iter->next) {
		iter->data->separate((LinkedList<Vehicle*>*)flock);
		iter->data->followPath(path);

		//set mover's target
		//the nearest bird who is inside mover's radarRadius ( == 250)
		float tempDistanceSqr = iter->data->radar(*moverPointer.getPosition());
		if (tempDistanceSqr < moverRadarRaiusSqr) {
			moverRadarRaiusSqr = tempDistanceSqr;
			moverPointer.setTarget(*iter->data->getPosition());
		}
		///////////////////////////////////////////////////////////////////////
		//fuck off this world is so fucking crazy and i don't know why that shit allways comes to me and beat me like a holy shitttttttttttttt.
		// i wish this fucking world is all alone. becaze I hate them.
		iter->data->fly();
		iter->data->display(camera);
		
		bool outOfScreen = false;
		if (iter->data->getPosition()->x > camera.getPosition().x + WINDOWSIZE_W*1.5f
			|| iter->data->getPosition()->x < camera.getPosition().x - WINDOWSIZE_W*0.5f)
		{
			iter->data->die(true);//die of falling out window
			outOfScreen = true;
		}

		if (iter->data->getPosition()->y > WINDOWSIZE_H*1.5f
			|| iter->data->getPosition()->y < -WINDOWSIZE_H*3.0f) {
			iter->data->die(true);
			outOfScreen = true;
		}

		if (iter->data->isDead()) {
			if (!outOfScreen) {
				//put new Explosion before being destroyed
				Explosion*newExplosion = new Explosion2();
				if (newExplosion) {
					newExplosion->setup(
						new Vector2D(iter->data->getPosition()->x,iter->data->getPosition()->y),
						textureExp, soundCollection->at(10));
					explosions->addFirst(newExplosion);
				}
			}
			delete iter->data;
			iter->data = NULL;
			iter = flock->delNode(iter);
		}
	}
	for (Node<Explosion*>*iter = explosions->getFirstNode(); iter != explosions->tail; iter = iter->next) {
		iter->data->display(camera);
		if (iter->data->isDead()) {
			delete iter->data;
			iter->data = NULL;
			iter = explosions->delNode(iter);
		}
	}

}
bool FlockOfBird::isDead() const
{
	return (flock->isEmpty()&&explosions->isEmpty());
}

float FlockOfBird::getID() const
{
	return ID;
}

void FlockOfBird::createNewBird(float radarRadius, Vector2D&position)
{
	Bird*newBird = new Bird(world, texture, &path,soundCollection->at(12),soundCollection->at(10));
	if (newBird) {
		newBird->setup(position.x, position.y);
		newBird->setRadarRadius(radarRadius);
		flock->addFirst(newBird);
	}
}
