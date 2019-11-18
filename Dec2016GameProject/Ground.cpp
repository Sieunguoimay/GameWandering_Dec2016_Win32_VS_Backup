#include "Ground.h"



Ground::Ground(b2World*world)
	:world(world)
{
	perlinNoise = new PerlinNoise();
	bricks = new LinkedList<Brick*>();
	size = new Size();
}


Ground::~Ground()
{
	for (Node<Brick*>*iter = bricks->getFirstNode(); iter != bricks->tail; iter = iter->next) {
		delete iter->data;
		iter->data = NULL;//to be safe
	}
	delete bricks;
	bricks = NULL;
	delete size;
	size = NULL;
}

void Ground:: setup(Texture*texture)
{
	size->set(51.0f, 51.0f);
	this->texture = texture;
}

void Ground::display(const Camera&camera)
{
	for (Node<Brick*>*iter = bricks->getFirstNode(); iter != bricks->tail; iter = iter->next) 
		this->texture->render(
			camera,
			(iter->data->getPosition().x*UtilityFunctions::M2P() - size->w *0.5f - camera.getPosition().x),
			(iter->data->getPosition().y*UtilityFunctions::M2P() - size->h *0.5f - camera.getPosition().y),
			size->w,
			size->h,
			iter->data->getType()-1, 0, false);
}


const Size & Ground::getSize() const
{
	// TODO: insert return statement here
	return *size;
}

void Ground::putNewBrick(float x, float y,int type, bool leftOrRight)
{
	this->leftOrRight = leftOrRight;
	Brick*newBrick = new Brick(world, size);
	newBrick->setup(x, y,type);
	if (newBrick)
		if (leftOrRight)
			bricks->addFirst(newBrick);//right
		else
			bricks->addLast(newBrick);//left
}

void Ground::delBrick(bool leftOrRight)
//if left or right is true then we delete the first brick on the right
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

void Ground::setPointerTo(SendMessage * anyKindOfObject)
{
	if (leftOrRight)
		bricks->getFirstNode()->data->setPointerTo(anyKindOfObject);
	else 
		bricks->tail->prev->data->setPointerTo(anyKindOfObject);
}




