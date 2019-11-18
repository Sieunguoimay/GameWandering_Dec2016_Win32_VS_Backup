#include "LightningKiller.h"



LightningKiller::LightningKiller(b2World*world, Texture*texture, Texture*texturePointer, Sound*sound)
	:world(world),texture(texture),texturePointer(texturePointer),sound(sound)
{
	path = new LinkedList<Vector2D*>();
	tinyParticles = new LinkedList<b2Body*>();
	data = new ObjectData(SomeDefine::LIGHTNING_KILLER, NULL);
	shutle = path->head;
}


LightningKiller::~LightningKiller()
{
	delete data;
	data = NULL;
	for (Node<Vector2D*>*iter = path->getFirstNode(); iter != path->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete path;
	path = NULL;
	for (Node<b2Body*>*iter = tinyParticles->getFirstNode(); iter != tinyParticles->tail; iter = iter->next)
	{
		world->DestroyBody(iter->data);
		iter->data = NULL;
	}
	delete tinyParticles;
	tinyParticles = NULL;
	sound = NULL;
	texture = NULL;
}

void LightningKiller::display(const Camera & camera)
{
	if (!path->isEmpty()) {
		//shutle = path->getFirstNode();
		shutle = shutle->next;
		if (shutle->next != path->tail){
			tinyParticles->addFirst(addNewParticleBody(shutle->next->data->x, shutle->next->data->y));
			for (Node<Vector2D*>*iter = path->getFirstNode(); iter != shutle->next; iter = iter->next)
				//draw a line
				renderLine(*iter->data, *iter->next->data, camera);
		}else{
			for (Node<Vector2D*>*iter = path->getFirstNode(); iter != path->tail; iter = iter->next)
			{
				delete iter->data;
				iter->data = NULL;
			}
			for (Node<b2Body*>*iter = tinyParticles->getFirstNode(); iter != tinyParticles->tail; iter = iter->next)
			{
				world->DestroyBody(iter->data);
				iter->data = NULL;
			}
			path->emptyList();
			tinyParticles->emptyList();
			shutle = path->head;
		}
		texturePointer->setColor(0xff, 0xee, 0x77);
	}
}

void LightningKiller::calculateDischarge(Vector2D&cloudPosition, Vector2D&target, bool beingShown)
{
	if (path->isEmpty()) {
		float height = fabsf(cloudPosition.y - target.y);
		float middle = cloudPosition.y + height/2;
		float y = cloudPosition.y;
		float width = fabsf(middle - y);
		float tempTarget = cloudPosition.x;
		while (y < target.y) {
			tempTarget += 0.1f*(target.x - tempTarget);//tempTarget tends to target.x
			float w = width - fabsf(middle - y);
			float deltaX = (float)((int)w > 0 ? (rand() % (int)w) : 0);
			float x = tempTarget +deltaX- w*0.5f;
			y += ((int)(height / 15+deltaX*0.5f)>0 ? (rand() % (int)(height / 15+deltaX*0.5f)) : 0) + 1;
			path->addLast(new Vector2D(x, y));
		}
		//sound is played here
		if (beingShown)
			sound->play();
	}
}

void LightningKiller::renderLine(Vector2D&pos1, Vector2D & pos2, const Camera&camera)
{
	static float w = 4.0f;
	Vector2D temp = Vector2D::subtract(pos2, pos1);
	float angle = temp.getAngle();
	float h = temp.magnitude();
	texture->render(camera,
		(pos1.x + pos2.x) *0.5f - w*0.5f - camera.getPosition().x,
		(pos1.y + pos2.y) *0.5f - h*0.5f - camera.getPosition().y,
		1.0f, h, 0,angle-PI/2.0f);
}

b2Body * LightningKiller::addNewParticleBody(float x, float y)
{

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());
	bd.gravityScale = 0;
	b2Body*newBody = world->CreateBody(&bd);
	b2CircleShape cs;
	cs.m_radius = 4 * UtilityFunctions::P2M();

	b2FixtureDef fd;
	fd.shape = &cs;
	fd.isSensor = true;

	newBody->CreateFixture(&fd);
	newBody->SetUserData(data);
	return newBody;
}
