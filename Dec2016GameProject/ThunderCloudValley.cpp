#include "ThunderCloudValley.h"


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
WanderingCloudValley::WanderingCloudValley(ValleyMarkPair * valleyMarkPair, int * pointerToNumNodes)
	:valleyMarkPair(valleyMarkPair), pointerToNumNodes(pointerToNumNodes)
{
	clouds = new LinkedList<Cloud*>();
	area = new Rectangle();
}

WanderingCloudValley::~WanderingCloudValley()
{
	delete area;
	area = NULL;
	for (Node<Cloud*>*iter = clouds->getFirstNode(); iter != clouds->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete clouds;
	clouds = NULL;

	delete valleyMarkPair->first->data;
	valleyMarkPair->first->data = NULL;
	valleyMarkPair->first->next->prev = valleyMarkPair->first->prev;
	valleyMarkPair->first->prev->next = valleyMarkPair->first->next;
	valleyMarkPair->first->next = NULL;
	valleyMarkPair->first->prev = NULL;
	delete valleyMarkPair->first;
	valleyMarkPair->first = NULL;
	(*pointerToNumNodes)--;

	delete valleyMarkPair->second->data;
	valleyMarkPair->second->data = NULL;
	valleyMarkPair->second->next->prev = valleyMarkPair->second->prev;
	valleyMarkPair->second->prev->next = valleyMarkPair->second->next;
	valleyMarkPair->second->next = NULL;
	valleyMarkPair->second->prev = NULL;
	delete valleyMarkPair->second;
	valleyMarkPair->second = NULL;
	(*pointerToNumNodes)--;

	delete valleyMarkPair;
	valleyMarkPair = NULL;
}

void WanderingCloudValley::setup(Texture * cloudTexture, Texture * rainTexture)
{
	this->cloudTexture = cloudTexture;
	this->rainTexture = rainTexture;
	valleyMarkPair->first->data->state = true;
	valleyMarkPair->second->data->state = true;
	//calculate the area where clouds exist
	Vector2D tempPos(
		(valleyMarkPair->first->data->position->x < valleyMarkPair->second->data->position->x ?
			valleyMarkPair->first->data->position : valleyMarkPair->second->data->position));
	area->set(tempPos.x, tempPos.y,
		fabsf(valleyMarkPair->first->data->position->x - valleyMarkPair->second->data->position->x),
		fabsf(valleyMarkPair->first->data->position->y - valleyMarkPair->second->data->position->y));

	int randNum = ((int)(area->w / 100)>0 ? rand() % (int)(area->w / 100) : 0) + 5;
	for (int i = 0; i < randNum; i++) {
		Cloud*newCloud = new Cloud(NULL, cloudTexture, NULL,NULL, false);
		newCloud->setup(area->x + (float)((int)area->w?(rand() % (int)area->w):0),
			area->y + (float)((int)(area->h / 3)?rand() % ((int)area->h / 3):0), area->h);
		clouds->addFirst(newCloud);
	}
}

void WanderingCloudValley::display(const Camera & camera)
{
	for (Node<Cloud*>*iter = clouds->getFirstNode(); iter != clouds->tail; iter = iter->next) {
		iter->data->blow(*clouds);
		iter->data->stayWithinValleyArea(area->x, area->y, area->w, area->h);
		iter->data->display(camera);
	}
}

bool WanderingCloudValley::isDead() const
{
	return (valleyMarkPair->first->data->isItTimeToDestroy() && valleyMarkPair->second->data->isItTimeToDestroy());
}

void WanderingCloudValley::destroyValley()
{
	this->~WanderingCloudValley();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

ThunderCloudValley::ThunderCloudValley(b2World*world, ValleyMarkPair*velleyMarkPair, int*pointerToNumNodes)
	:world(world),WanderingCloudValley(velleyMarkPair,pointerToNumNodes)
{
	data = new ObjectData(SomeDefine::VALLEY_ID, this);
	deliver = new Vector2D();
}


ThunderCloudValley::~ThunderCloudValley()
{
	world->DestroyBody(body);
	body = NULL;
	delete data;
	data = NULL;
	delete deliver;
	deliver = NULL;
}

void ThunderCloudValley::setup(Texture * cloudTexture,Texture*rainTexture, Texture * thunderTexture, Sound*thunderSound)
{
	//this->cloudTexture = cloudTexture;
	this->isThereMessage = false;
	valleyMarkPair->first->data->state = true;
	valleyMarkPair->second->data->state = true;
	//calculate the area where clouds exist
	Vector2D tempPos(
		(valleyMarkPair->first->data->position->x < valleyMarkPair->second->data->position->x ? 
		valleyMarkPair->first->data->position : valleyMarkPair->second->data->position));
	area->set(tempPos.x, tempPos.y,
		fabsf(valleyMarkPair->first->data->position->x - valleyMarkPair->second->data->position->x),
		fabsf(valleyMarkPair->first->data->position->y - valleyMarkPair->second->data->position->y));

	int randNum = ((int)(area->w / 100)>0? (rand() % (int)(area->w / 100)):0)+ 5;
	for (int i = 0; i < randNum; i++) {
		Cloud*newCloud = new Cloud(world, cloudTexture, thunderTexture,thunderSound,true);
		newCloud->setup(
			area->x+(area->w>0?(rand() % (int)area->w):0),
			area->y+ ((int)area->h / 3>0?(rand()%((int)area->h/3)):0),
			area->h);
		clouds->addFirst(newCloud);
	}
	
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set((area->x+area->w/2)*UtilityFunctions::P2M(), (area->y+area->h/2)*UtilityFunctions::P2M());
	bd.gravityScale = 0;
	body = world->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(area->w *0.5f * UtilityFunctions::P2M(), area->h*0.5f * UtilityFunctions::P2M());
	
	b2FixtureDef fd;
	fd.isSensor = true;
	fd.shape = &ps;
	body->CreateFixture(&fd);
	body->SetUserData(data);
}

void ThunderCloudValley::display(const Camera & camera)
{
	for (Node<Cloud*>*iter = clouds->getFirstNode(); iter != clouds->tail; iter = iter->next) {
		iter->data->blow(*clouds);
		iter->data->stayWithinValleyArea(area->x, area->y, area->w, area->h/3);
		iter->data->display(camera);

		if (isThereMessage) 
			iter->data->workOnNewTarget(deliver);
	}
	isThereMessage = false;
}


void ThunderCloudValley::keepTrackOfObjectInsideArea(Vector2D * position)
{
	this->isThereMessage = true;
	deliver->set(position);
}

void ThunderCloudValley::destroyValley()
{
	this->~ThunderCloudValley();
}
