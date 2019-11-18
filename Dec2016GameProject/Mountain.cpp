#include "Mountain.h"


float Mountain::differenceNoise = 0.0f;

Mountain::Mountain(Camera*camera)
	:camera(camera)
{
	bricks = new MountainMatrix();
	size = new Size();
	perlinNoise = new Perlin();
}


Mountain::~Mountain()
{
	for (Node<LinkedList<MountainBrick*>*>*iter = bricks->getFirstNode(); iter != bricks->tail; iter = iter->next)
	{
		for (Node<MountainBrick*>*iter2 = iter->data->getFirstNode(); iter2 != iter->data->tail; iter2 = iter2->next)
		{
			delete iter2->data;
			iter2->data = NULL;
		}
		delete iter->data;
		iter->data = NULL;
	}
	delete bricks;
	bricks = NULL;
	delete size;
	size = NULL;
	delete perlinNoise;
	perlinNoise = NULL;
}

void Mountain::setup(Texture*texture,float cameraRate,Color*color)
{
	if (differenceNoise > 10)
		differenceNoise = 0;
	differenceNoise += 0.1f;
	this->cameraRate = cameraRate;
	this->color = color;
	this->texture = texture;
	size->set(40.0f, 40.0f);
	putBricksAtFirst();
}

void Mountain::display(const Camera & camera)
{
	texture->setColor(color);
	updateMountain();
	for (Node<LinkedList<MountainBrick*>*>*iter = bricks->getFirstNode(); iter != bricks->tail; iter = iter->next)
		for (Node<MountainBrick*>*iter2 = iter->data->getFirstNode(); iter2 != iter->data->tail; iter2 = iter2->next)
			iter2->data->display(camera,cameraRate);
	
}

void Mountain::updateMountain()
{
	float x = bricks->getFirstNode()->data->getFirstNode()->data->getRectangle().x;
	//the left is inside the window
	if (checkInsideWindow(x)) {
	 	bricks->addFirst(putBrickAccordingToNoise(x-size->w));
		
		for (Node<MountainBrick*>*iter = bricks->tail->prev->data->getFirstNode(); iter != bricks->tail->prev->data->tail; iter = iter->next) {
			delete iter->data;
			iter->data = NULL;
			iter = bricks->tail->prev->data->delNode(iter);
		}
		delete bricks->tail->prev->data;
		bricks->tail->prev->data = NULL;
		bricks->delLast();
	}

	x = bricks->tail->prev->data->getFirstNode()->data->getRectangle().x;
	//the right is inside the window
	if (checkInsideWindow(x)) {

		bricks->addLast(putBrickAccordingToNoise(x+size->w));
		
		for (Node<MountainBrick*>*iter = bricks->head->next->data->getFirstNode(); iter != bricks->head->next->data->tail; iter = iter->next) {
			delete iter->data;
			iter->data = NULL;
			iter = bricks->head->next->data->delNode(iter);
		}
		delete bricks->head->next->data;
		bricks->head->next->data = NULL;
		bricks->delFist();
	}
}

bool Mountain::checkInsideWindow(float x)
{
	if (x< camera->getPosition().x*cameraRate)
		return false;
	if (x> camera->getPosition().x*cameraRate+WINDOWSIZE_W)
		return false;
	return true;
}

LinkedList<MountainBrick*>* Mountain::putBrickAccordingToNoise(float x)
{
	float xOff = x / 400.0f;
	LinkedList<MountainBrick*>*newColumn = new LinkedList<MountainBrick*>();
	if (newColumn) {
		float mountainHeight = UtilityFunctions::map(
			(float)perlinNoise->noise(xOff, differenceNoise,0)
			, -1, 1, -camera->getSize().h*0.25f, camera->getSize().h*1.50f);

		int numberBrick = (int)(mountainHeight / size->h);

		for (int i = 0; i < numberBrick; i++) {
			MountainBrick*newMountainBrick = new MountainBrick(
				x, (20.0f - (float)i)*size->h,
				size->w, size->h, texture);
			if (newMountainBrick)
				newColumn->addLast(newMountainBrick);
		}
	}
	return newColumn;
}

void Mountain::delBrick(bool leftOrRight)
{
	if (leftOrRight) {
		delete bricks->getFirstNode()->data;
		bricks->getFirstNode()->data = NULL;
		bricks->delFist();//right
	}
	else {
		delete bricks->tail->prev->data;
		bricks->tail->prev->data = NULL;
		bricks->delLast();//left	
	}
}

void Mountain::putBricksAtFirst()
{
	int extraBrickCollumn = 10;
	int numBrickColumn = (int)(camera->getSize().w / size->w);
	numBrickColumn += extraBrickCollumn;

	float x = -extraBrickCollumn*0.5f * size->w;
	for (int i = 0; i < numBrickColumn; i++)
		bricks->addLast(putBrickAccordingToNoise(x + i*size->w));
}
