#include "Bushes.h"



Bushes::Bushes()
{
	bushContainer = new LinkedList<Bush*>();
	size = new Size();
}


Bushes::~Bushes()
{
	for (Node<Bush*>*iter = bushContainer->getFirstNode(); iter != bushContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete bushContainer;
	bushContainer = NULL;
	delete size;
	size = NULL;
}

void Bushes::setup(Texture * texture)
{
	skins = texture;
	size->set(200.0f, 100.0f);//adjust yourself to fit the 1366x768
}

void Bushes::display(const Camera&camera)
{
	for (Node<Bush*>*iter = bushContainer->getFirstNode(); iter != bushContainer->tail; iter = iter->next){
		skins->render(
			camera,
			iter->data->getPosition()->x- camera.getPosition().x - size->w/2.0f, 
			iter->data->getPosition()->y - camera.getPosition().y - size->h,
			size->w,
			size->h,
			iter->data->getID(),0,false);
		//story has been changed
		//no more destroying as a bush go off the screen
		//instead destroying as it recieves a message of a secret guy namely Brick :V
		/*if (((Vector2D*)(iter->data->pointer))->x + size->w<camera.getPosition().x ||
			((Vector2D*)(iter->data->pointer))->x - size->w>camera.getPosition().x + camera.getSize().w)
			iter = ID_PositionCollection->delNode(iter);
			*/	
		if (iter->data->isItTimeToDestroy()){
			//clean the dynamic memory
			delete iter->data;
			iter->data = NULL;
			//delete the empty node
			iter = bushContainer->delNode(iter);
		}
	}
}

void Bushes::plant(float x, float y, int ID)
{ 
	//convert to from 0 used for rendering
	Bush*newBush = new Bush(new Vector2D(x, y), ID);
	if (newBush)//tobe safe
		bushContainer->addFirst(newBush);
}

Bush * Bushes::getNewBush()
{
	return bushContainer->getFirstNode()->data;
}
