#include "Brick.h"


Brick::Brick(b2World*world, Size*size)
	:Shape(world),size(size)
{
	pointers = new LinkedList<SendMessage*>();
}

//by calling this destructor function
//this brick will send a message of "it's time to die baby" to all of it's hung over items

Brick::~Brick()
{

	//send message away before kill himself
	for (Node<SendMessage*>*iter = pointers->getFirstNode(); iter != pointers->tail; iter = iter->next) {
		iter->data->destroyNow();
		//sent complete we throw the item away from this container
		//of course, without delete the dynamic memory of that item
		iter = pointers->delNode(iter);//delete the Node only
	}
	delete pointers;
	pointers = NULL;
	world->DestroyBody(body);
	size = NULL;//put its value stored inside to be NULL
	delete data;
	data = NULL;
	//any pointer create has its own memory location
	//its value stored is address of another memory location
	//so even the pointer size inside of Ground - to which this->size take in value - is the same to this->size

	//so we use delete - so that before the pointer points to that dynamic memory be automatically destroyed
	//it must take care for that memory first
}

void Brick::setup(float x, float y, int typeBrick)
{
	this->typeBrick = typeBrick;
	if (typeBrick == 1) {
		data = new ObjectData(SomeDefine::UNDERGROUND_BRICK_ID, NULL);
		createBodyBrick(x, y, size->w, size->h, false);
	}
	else if (typeBrick == 2) {
		data = new ObjectData(SomeDefine::SURFACE_BRICK_ID, NULL);
		createBodyBrick(x, y, size->w, size->h, false);
	}
	else if (typeBrick == 3) {
		createBodyBrick(x, y, size->w, size->h, true);
		return;
	}
	body->SetUserData(data);
}

b2Body * Brick::getBody()
{
	return body;
}

const b2Vec2 & Brick::getPosition() const
{
	// TODO: insert return statement here
	return body->GetPosition();
}

const int & Brick::getType() const
{
	// TODO: insert return statement here
	return typeBrick;
}

void Brick::setPointerTo(SendMessage * pointer)
{
	pointers->addFirst(pointer);
}

void Brick::createBodyBrick(float x, float y, float w, float h,bool isSensor)
{
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());
	body = world->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(w/2*UtilityFunctions::P2M(), h/2*UtilityFunctions::P2M());

	b2FixtureDef fd;
	fd.shape = &ps;
	if (isSensor)
		fd.isSensor = true;
	body->CreateFixture(&fd);
}
