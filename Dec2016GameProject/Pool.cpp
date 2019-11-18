#include "Pool.h"



Pool::Pool(b2World*world, PoolMarkPair*poolMarkPair, float sizeError)
	:world(world), poolMarkPair(poolMarkPair)
{
	poolMarkPair->first->data->state = true;//married
	poolMarkPair->second->data->state = true;//married
	position1 = new Vector2D((poolMarkPair->first->data->position->x < poolMarkPair->second->data->position->x ?
		poolMarkPair->first->data->position : poolMarkPair->second->data->position));
	position1->x -= sizeError;
	position1->y -= sizeError;

	size = new Size(fabsf(poolMarkPair->first->data->position->x - poolMarkPair->second->data->position->x)+2*sizeError,
		fabsf(poolMarkPair->first->data->position->y - poolMarkPair->second->data->position->y)+2 * sizeError);
	woods = new LinkedList<Wood*>();
	flockOfFish = new LinkedList<Fish*>();
	data = new ObjectData(SomeDefine::WATER_POOL_ID, this);
	data2 = new ObjectData(SomeDefine::SURFACE_BRICK_ID, NULL);
}


Pool::~Pool()
{
	world->DestroyBody(body);
	body = NULL;
	for (Node<Wood*>*iter = woods->getFirstNode(); iter != woods->tail; iter = iter->next)
	{
		world->DestroyBody(iter->data->first);
		iter->data->first = NULL;
		delete iter->data->second;
		iter->data->second = NULL;
		delete iter->data;
		iter->data = NULL;
	}
	delete woods;
	woods = NULL;
	for (Node<Fish*>*iter = flockOfFish->getFirstNode(); iter != flockOfFish->tail; iter = iter->next) {
		delete iter->data;
		iter->data = NULL;
	}
	delete flockOfFish;
	flockOfFish = NULL;

	delete size;
	size = NULL;
	delete data;
	data = NULL;
	//kill itself - without effecting on any thing else
	poolMarkPair->first->next->prev = poolMarkPair->first->prev;
	poolMarkPair->first->prev->next = poolMarkPair->first->next;
	poolMarkPair->first->next = NULL;
	poolMarkPair->first->prev = NULL;
	(*pointerToNumNodes)--;
	delete poolMarkPair->first->data;
	poolMarkPair->first->data = NULL;
	delete poolMarkPair->first;
	poolMarkPair->first = NULL;
	
	poolMarkPair->second->next->prev = poolMarkPair->second->prev;
	poolMarkPair->second->prev->next = poolMarkPair->second->next;
	poolMarkPair->second->next = NULL;
	poolMarkPair->second->prev = NULL;
	(*pointerToNumNodes)--;
	delete poolMarkPair->second->data;
	poolMarkPair->second->data = NULL;
	delete poolMarkPair->second;
	poolMarkPair->second = NULL;
	delete poolMarkPair;
	poolMarkPair = NULL;
}

void Pool::setup(Texture * texture, Texture*woodTexture, Texture*fishTexture, bool includedWoods,int*pointerToNumNodes, bool isThereFish)
{
	this->pointerToNumNodes = pointerToNumNodes;
	this->foreLayer = texture;
	this->woodTexture = woodTexture;
	foreLayer->setAlpha(100);
	foreLayer->setColor(50, 100,255);//light blue
	createBox();
	if (includedWoods)
		addWoods();

	if (isThereFish) {
		int randomFishPopulation = rand() % (int)(10 + size->getHalfPerimeter() / 100) + 1;
		for (int i = 0; i < randomFishPopulation; i++) {
			Fish*newFish = new Fish(world, fishTexture);
			if (newFish) {
				newFish->setup(position1->x + size->w / 2 + rand() % 20,
					position1->y + size->h / 2 + rand() % 20);
				flockOfFish->addFirst(newFish);
			}
		}
	}
}

void Pool::display(const Camera & camera)
{
	for (Node<Wood*>*iter = woods->getFirstNode(); iter != woods->tail; iter = iter->next)
	{
		b2Vec2 pos = iter->data->first->GetPosition();
		float angle = iter->data->first->GetAngle();
		pos *= (float32)UtilityFunctions::M2P();
		this->woodTexture->render(camera,
			pos.x - camera.getPosition().x - iter->data->second->w / 2,
			pos.y - camera.getPosition().y - iter->data->second->h / 2,
			iter->data->second->w, iter->data->second->h, 0, angle);
	}
	static const float spaceFromWall = 50;
	for (Node<Fish*>*iter = flockOfFish->getFirstNode(); iter != flockOfFish->tail; iter = iter->next) {
		iter->data->stayWithinPool(
			position1->x + spaceFromWall, position1->y + spaceFromWall,
			size->w - 2 * spaceFromWall, size->h - 2 * spaceFromWall);
		if (checkInside(*iter->data->getPosition()))
		{
			iter->data->flock(flockOfFish);
			iter->data->pursuePrey();
		}
		else iter->data->clearAllForce();
		iter->data->display(camera);
	}
	//temporary
	display2(camera);
}

void Pool::display2(const Camera & camera)
{
	foreLayer->render(camera, position1->x - camera.getPosition().x, position1->y - camera.getPosition().y, size->w, size->h);
}

void Pool::findOutMoverUnderWater(Vector2D * position)
{
	for (Node<Fish*>*iter = flockOfFish->getFirstNode(); iter != flockOfFish->tail; iter = iter->next)
		iter->data->addPreyTarget(*position);
}

bool Pool::checkInside(Vector2D & objectPosition) const
{
	if (objectPosition.x < position1->x)
		return false;
	if (objectPosition.x > position1->x+size->w)
		return false;
	//if (objectPosition.y < position1->y)
		//return false;
	if (objectPosition.y > position1->y+size->h)
		return false;
	return true;
}

bool Pool::isDead() const
{
	return (poolMarkPair->first->data->isItTimeToDestroy()&& poolMarkPair->second->data->isItTimeToDestroy());
}

void Pool::createBox()
{

	{
		b2BodyDef bd;
		bd.type = b2BodyType(0);
		bd.position.Set(position1->x + size->w / 2, position1->y + size->h / 2);
		bd.position *= (float32)UtilityFunctions::P2M();
		bd.angle = 0.000000000000000e+00f;
		bd.linearVelocity.Set(0.000000000000000e+00f, 0.000000000000000e+00f);
		bd.angularVelocity = 0.000000000000000e+00f;
		bd.linearDamping = 0.000000000000000e+00f;
		bd.angularDamping = 0.000000000000000e+00f;
		
		bd.allowSleep = bool(4);
		bd.awake = bool(2);
		bd.fixedRotation = bool(0);
		bd.bullet = bool(0);
		bd.active = bool(32);
		bd.gravityScale = 1.000000000000000e+00f;
		body = world->CreateBody(&bd);

		{
			b2FixtureDef fd;
			fd.friction = 2.000000029802322e-01f;
			fd.restitution = 0.000000000000000e+00f;
			fd.density = 4.000000000000000e+00f;
			fd.isSensor = bool(1);
			fd.filter.categoryBits = uint16(1);
			fd.filter.maskBits = uint16(65535);
			fd.filter.groupIndex = int16(0);
			b2PolygonShape ps;
			ps.SetAsBox(size->w *0.5f * (float32)UtilityFunctions::P2M(), size->h *0.5f * (float32)UtilityFunctions::P2M());

			fd.shape = &ps;

			body->CreateFixture(&fd);
			body->SetUserData(data);
		}
	}
}

void Pool::addWoods()
{
	const int WOOD_STEP = 240;
	for (int i = 0; i < size->w / WOOD_STEP-1; i++) {
		Size*newSize = new Size(WOOD_STEP /1.5f, WOOD_STEP / 6);
		woods->addFirst(
			new Wood(createBodyBox((
				position1->x + (i + 1)*WOOD_STEP)*UtilityFunctions::P2M(),
				position1->y*UtilityFunctions::P2M(),
			newSize->w*(float32)UtilityFunctions::P2M()*0.5f,newSize->h*0.6f*UtilityFunctions::P2M()*0.5f),newSize));
	}
}

b2Body*Pool::createBodyBox(float x, float y,float w, float h)
{
	b2Body*newBody = NULL;
	{
		b2BodyDef bd;
		bd.type = b2BodyType(2);
		bd.position.Set(x, y);
		bd.angle = 0.000000000000000e+00f;
		bd.linearVelocity.Set(0.000000000000000e+00f, 0.000000000000000e+00f);
		bd.angularVelocity = 0.000000000000000e+00f;
		bd.linearDamping = 0.000000000000000e+00f;
		bd.angularDamping = 0.000000000000000e+00f;
		bd.allowSleep = bool(4);
		bd.awake = bool(2);
		bd.fixedRotation = bool(0);
		bd.bullet = bool(0);
		bd.active = bool(32);
		bd.gravityScale = 1.000000000000000e+00f;
		newBody = world->CreateBody(&bd);

		{
			b2FixtureDef fd;
			fd.friction = 2.000000029802322e-01f;
			fd.restitution = 0.000000000000000e+00f;
			fd.density = 1.000000000000000e+00f;
			fd.isSensor = bool(0);
			fd.filter.categoryBits = uint16(1);
			fd.filter.maskBits = uint16(65535);
			fd.filter.groupIndex = int16(0);

			b2PolygonShape ps2;
			ps2.SetAsBox(w, h);

			fd.shape = &ps2;
			newBody->CreateFixture(&fd);
			newBody->SetUserData(data2);
		}
	}
	return newBody;
}
