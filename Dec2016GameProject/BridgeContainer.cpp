#include "BridgeContainer.h"



BridgeContainer::BridgeContainer(b2World*world)
	:world(world)
{
	radius = 15.0f;
	bridges = new LinkedList<Bridge*>();
	bridgeMarks = new LinkedList < TreeRoot*>();
}


BridgeContainer::~BridgeContainer()
{
	for (Node<Bridge*>*iter = bridges->getFirstNode(); iter != bridges->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete bridges;
	bridges = NULL;

	for (Node<TreeRoot*>*iter = bridgeMarks->getFirstNode(); iter != bridgeMarks->tail; iter = iter->next)
	{
		world->DestroyBody(iter->data->body);
		iter->data->body = NULL;
		delete iter->data;
		iter->data = NULL;
	}
	delete bridgeMarks;
	bridgeMarks = NULL;
}

void BridgeContainer::reset()
{
	for (Node<Bridge*>*iter = bridges->getFirstNode(); iter != bridges->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
		iter = bridges->delNode(iter);
	}
	for (Node<TreeRoot*>*iter = bridgeMarks->getFirstNode(); iter != bridgeMarks->tail; iter = iter->next)
	{
			world->DestroyBody(iter->data->body);
			iter->data->body = NULL;
			delete iter->data;
			iter->data = NULL;
			iter = bridgeMarks->delNode(iter);
		
	}
}

void BridgeContainer::setup(Texture * texture1, Texture * texture2)
{
	this->texture1 = texture1;
	this->texture2 = texture2;
}

void BridgeContainer::display(const Camera & camera)
{
	for (Node<TreeRoot*>*iter = bridgeMarks->getFirstNode(); iter != bridgeMarks->tail; iter = iter->next)
	{
		b2Vec2 pos = iter->data->body->GetPosition();
		texture2->render(camera,
			pos.x*UtilityFunctions::M2P() - camera.getPosition().x - radius*2,
			pos.y*UtilityFunctions::M2P() - camera.getPosition().y - radius*3*(texture2->getSize()->h / texture2->getSize()->w),
			radius * 4, radius * 4*(texture2->getSize()->h/ texture2->getSize()->w),
			0, 0, false);

		if (iter->data->isItTimeToDestroy() && (iter->data->state == false)) {
			world->DestroyBody(iter->data->body);
			delete iter->data;
			iter = bridgeMarks->delNode(iter);
		}
		
	}
	for (Node<Bridge*>*iter = bridges->getFirstNode(); iter != bridges->tail; iter = iter->next)
	{
		iter->data->display(camera);
		if (iter->data->isDead()) {
			delete iter->data;
			iter->data = NULL;
			iter = bridges->delNode(iter);
		}
	}
}

void BridgeContainer::putBridgeMark(float x, float y)
{
	//check is there any mark or not
	for (Node<TreeRoot*>*iter = bridgeMarks->getFirstNode(); iter != bridgeMarks->tail; iter = iter->next)
		if (iter->data->x == x) {
			iter->data->shouldBeDestroyed = false;
			return;//if so jump out of the function
		}
	b2BodyDef bd;
	bd.type = b2_staticBody;
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());//no converting position
	b2Body*newWood = world->CreateBody(&bd);

	b2CircleShape cs;
	cs.m_radius = radius* UtilityFunctions::P2M();

	b2FixtureDef fd;
	fd.shape = &cs;
	fd.density = 1.0f;
	fd.isSensor = true;
	newWood->CreateFixture(&fd);

	bridgeMarks->addFirst(new TreeRoot(newWood,x));
	buildNewBridge();
}

TreeRoot * BridgeContainer::getNewTreeRoot()
{
	return bridgeMarks->getFirstNode()->data;
}

void BridgeContainer::buildNewBridge()
{
	if (bridgeMarks->getSize() % 2 == 0&&(!bridgeMarks->isEmpty())) {
		//this is the solution for the bug by which the there is one mark left when the game reset.
		bridges->addFirst(
			new Bridge(world, texture1,
			new TreeRootPair(bridgeMarks->getFirstNode()->next, bridgeMarks->getFirstNode()),
				&bridgeMarks->numNodes, radius));
	}
}
