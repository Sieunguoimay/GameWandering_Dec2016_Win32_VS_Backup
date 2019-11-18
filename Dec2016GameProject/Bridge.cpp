#include "Bridge.h"



Bridge::Bridge(b2World*world, Texture*texture, TreeRootPair*treeRootPair, int*pointerToNumNodes,float radius)
	:world(world),texture(texture),treeRootPair(treeRootPair),radius(radius),pointerToNumNodes(pointerToNumNodes)
{
	data = new ObjectData(SomeDefine::SURFACE_BRICK_ID, NULL);
	woods = new LinkedList<b2Body*>();
	setup();
}


Bridge::~Bridge()
{
	delete treeRootPair->first->data;
	treeRootPair->first->next->prev = treeRootPair->first->prev;
	treeRootPair->first->prev->next = treeRootPair->first->next;
	treeRootPair->first->next = NULL;
	treeRootPair->first->prev = NULL;
	delete treeRootPair->first;
	(*pointerToNumNodes)--;

	delete treeRootPair->second->data;
	treeRootPair->second->next->prev = treeRootPair->second->prev;
	treeRootPair->second->prev->next = treeRootPair->second->next;
	treeRootPair->second->next = NULL;
	treeRootPair->second->prev = NULL;
	delete treeRootPair->second;
	(*pointerToNumNodes)--;

	delete treeRootPair;


	for (Node<b2Body*>*iter = woods->getFirstNode(); iter != woods->tail; iter = iter->next)
		world->DestroyBody(iter->data);
	delete woods;
	delete data;
}

void Bridge::setup()
{
	//check is body1's X is smaller than body2's X or not
	checkStaticBodys();//what if staticBody2's position is in front of the other's position
	treeRootPair->first->data->state = true;
	treeRootPair->second->data->state = true;

	b2Vec2 pos1 = treeRootPair->first->data->body->GetPosition();// +4 * radius*UtilityFunctions::P2M();
	b2Vec2 pos2 = treeRootPair->second->data->body->GetPosition();// -3 * radius*UtilityFunctions::P2M();

	float y = (treeRootPair->first->data->body->GetPosition().y
		+ treeRootPair->second->data->body->GetPosition().y) / 2.0f;//incase 2 body have different Y position
	
	float distance = (pos2 - pos1).Length();
	int n = (int)(distance / (2.0f*radius*UtilityFunctions::P2M()));
	//put woods and connect bodys
	float sinY = (pos2.y - pos1.y) / distance;
	float cosX = (pos2.x - pos1.x) / distance;
	//while (x1 < x2) {
	for(int i = 1; i<n-1; i++){
		float delta = (i + 1)*2.0f*radius*UtilityFunctions::P2M();
		putNewWood(pos1.x+delta*cosX, pos1.y+delta*sinY);
		
		if (woods->getSize() >= 2)//connect the 2 last added body
			connect2Body(woods->getFirstNode()->next->data, woods->getFirstNode()->data);
	}
	if (!woods->isEmpty()) {
		connect2Body(treeRootPair->first->data->body, woods->tail->prev->data);
		connect2Body(treeRootPair->second->data->body, woods->getFirstNode()->data);
	}
}

void Bridge::display(const Camera & camera)
{

	for (Node<b2Body*>*iter = woods->getFirstNode(); iter != woods->tail; iter = iter->next)
	{
		b2Vec2 pos = iter->data->GetPosition();
		float angle = iter->data->GetAngle();
		texture->render(camera,
			pos.x*UtilityFunctions::M2P() - camera.getPosition().x - radius,
			pos.y*UtilityFunctions::M2P() - camera.getPosition().y - radius,
			radius * 2, radius * 2,
			0, angle, false);

		//render connecting lines
	}
}

bool Bridge::isDead() const
{
	return (treeRootPair->first->data->isItTimeToDestroy() && treeRootPair->second->data->isItTimeToDestroy());
}

void Bridge::checkStaticBodys()
{
	if (treeRootPair->first->data->body->GetPosition().x > treeRootPair->second->data->body->GetPosition().x) {
		//switch 2 bodys
		TreeRoot*temp = treeRootPair->first->data;
		treeRootPair->first->data = treeRootPair->second->data;
		treeRootPair->second->data = temp;
	}
}

void Bridge::connect2Body(b2Body * body1, b2Body * body2)
{
	
	b2RevoluteJointDef djd;
	djd.Initialize(body1, body2, body1->GetWorldCenter());
	world->CreateJoint(&djd);
}

void Bridge::putNewWood(float x, float y)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x, y);//no converting position
	b2Body*newWood = world->CreateBody(&bd);
	
	//b2PolygonShape ps;
	//ps.SetAsBox(radius*UtilityFunctions::P2M(), radius*UtilityFunctions::P2M());
	b2CircleShape cs;
	cs.m_radius = radius*UtilityFunctions::P2M();
	
	b2FixtureDef fd; 
	fd.shape = &cs;
	fd.density = 1;
	newWood->CreateFixture(&fd);
	newWood->SetUserData(data);
	woods->addFirst(newWood);
}
