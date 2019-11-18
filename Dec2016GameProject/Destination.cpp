#include "Destination.h"



Destination::Destination(b2World*world, void*pointer)
	:world(world)
{
	destination = new LinkedList<DestinationBody*>();
	data = new ObjectData(SomeDefine::DESTINATION_ID, pointer);
}


Destination::~Destination()
{
	for (Node<DestinationBody*>*iter = destination->getFirstNode(); iter != destination->tail; iter = iter->next) {
		delete iter->data;
	}
	delete destination;
}
void Destination::setup(Texture*textureDestination, Texture*textureNextLevel)
{
	this->textureDestination = textureDestination;
	((LevelManager*)data->pointer)->setup(textureNextLevel);
}
void Destination::display(const Camera & camera){
	for (Node<DestinationBody*>*iter = destination->getFirstNode(); iter != destination->tail; iter = iter->next) {
		iter->data->display(camera);
		if (iter->data->isItTimeToDestroy()) {
			delete iter->data;
			iter = destination->delNode(iter);
		}
	}
}
void Destination::addNewDestination(float x, float y)
{
	DestinationBody*newDestinationBody = new DestinationBody(world, textureDestination);
	if (newDestinationBody) {
		newDestinationBody->setup(x, y);
		newDestinationBody->setUserData(data);
		destination->addFirst(newDestinationBody);
	}
}
DestinationBody * Destination::getNewDestinationBody()
{
	return destination->getFirstNode()->data;
}
///////////////////////////////////////////////
///////////////////////////////////////////////
DestinationBody::DestinationBody(b2World * world, Texture * texture)
	:world(world),texture(texture)
{
	size = new Size();
}

DestinationBody::~DestinationBody()
{
	world->DestroyBody(body);
	body = NULL;
	delete size;
	size = NULL;
}

void DestinationBody::setup(float x, float y)
{
	size->w = 100;
	size->h = (size->w / texture->getSize()->w)*texture->getSize()->h;
	y -= size->h/2;
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.gravityScale = 0;
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());
	body = world->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(size->w / 4 * UtilityFunctions::P2M(), size->h / 4 * UtilityFunctions::P2M());

	b2FixtureDef fd;
	fd.shape = &ps;
	fd.isSensor = true;
	body->CreateFixture(&fd);
}

void DestinationBody::display(const Camera & camera)
{
	b2Vec2 pos = UtilityFunctions::M2P()*body->GetPosition();
	texture->render(camera, pos.x - size->w / 2 - camera.getPosition().x, pos.y - size->h / 2 - camera.getPosition().y, size->w, size->h);
}

void DestinationBody::setUserData(void * pointer)
{
	body->SetUserData(pointer);
}
