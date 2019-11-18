#include "Photon.h"



Photon::Photon(b2World * world)
	:ParticleSystem(),world(world)
{
	collapsed = false;
}

Photon::~Photon()
{
	if (body != NULL)
	{
		world->DestroyBody(body);
		body = NULL;
	}
	collapsed = false;
	delete color;
	color = NULL;
	delete data;
	data = NULL;
	//call the destructor of the particle
}

void Photon::setup(Vector2D * position, Vector2D * velocity, Size * size, Texture * texture)
{
	color = new Color(255,rand()%150,rand()%100);

	ParticleSystem::setup(new Vector2D(position), new Vector2D(velocity), size, texture,10,0);
	
	b2BodyDef bd;
	bd.position.Set(position->x*UtilityFunctions::P2M(), position->y*UtilityFunctions::P2M());
	bd.type = b2_dynamicBody;
	bd.gravityScale = 0;
	body = world->CreateBody(&bd);
	b2CircleShape cs;
	cs.m_radius = 5* UtilityFunctions::P2M();
	b2FixtureDef fd;
	fd.isSensor = true;
	fd.shape = &cs;
	fd.density = 0.001f;
	body->CreateFixture(&fd);
	//apply data for the body
	data = new ObjectData(SomeDefine::PHOTON_LAZER_ID, this);
	body->SetUserData(data);
}

void Photon::collapse()
{
	collapsed = true;
	lifespan = 0;//set it to zero
}

bool Photon::hasCollapsed() const
{
	return collapsed;
}

void Photon::display(const Camera & camera)
{
	if (body != NULL)
		body->SetLinearVelocity(b2Vec2(velocity->x*0.5f, velocity->y *0.5f));

	texture->setColor(color);
	ParticleSystem::display(camera);
	texture->setColor(0xff, 0xff, 0xff);//set to normal
}

Vector2D * Photon::getPosition()
{
	return this->position;
}

