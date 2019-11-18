#include "Lightning.h"



Lightning::Lightning(b2World*world,float maxSpeed , float maxForce)
	:world(world),maxForce(maxForce),maxSpeed(maxSpeed)
{
}

Lightning::~Lightning()
{
	this->hasDestroyedBody = false;
}

void Lightning::destroyBody()
{
	world->DestroyBody(body);
	body = NULL;
	objectData->pointer = NULL;
	delete objectData;//object Data should be destroyed after the body is cleared-because body sill point to that data memory
	this->hasDestroyedBody = true;
	b2Vec2 a;
}

void Lightning::display(const Camera & camera)
{
	ParticleSystem::display(camera);

	if(!hasDestroyedBody)
	body->SetLinearVelocity(b2Vec2(velocity->x/2.0f, velocity->y/2.0f));//yes we have to devide by 2 ><

	if(lifespan<=0&&!hasDestroyedBody)
		destroyBody();

}

void Lightning::setup(Vector2D * position, Vector2D * velocity, Size * size, Texture * texture)
{
	this->hasDestroyedBody = false;
	this->position = position;
	ParticleSystem::setup(position, velocity, size, texture, 100);

	b2BodyDef bd;
	bd.position.Set(position->x*UtilityFunctions::P2M(), position->y*UtilityFunctions::P2M());
	bd.type = b2_kinematicBody;
	body = world->CreateBody(&bd);

	b2CircleShape cs;
	cs.m_radius = 10*UtilityFunctions::P2M();

	b2FixtureDef fd;
	fd.shape = &cs;
	fd.isSensor = true;
	body->CreateFixture(&fd);

	objectData = new ObjectData(SomeDefine::MOVER_LIGHTNING, this);
	body->SetUserData(objectData);
}

void Lightning::seek(Vector2D * target)
{
	//storyboard:
	//we have a target - to calculate the desired velocity
	//we have current velocity
	//hence: we can calculate the steering force to apply for the body

	Vector2D*desired = Vector2D::subtract(target, position);
	desired->normalize();
	desired->multiply(maxSpeed);

	//steering force
	desired->subtract(velocity);
	desired->limit(maxForce);
	
	//apply steering force for the body
	applyForce(*desired);
	delete desired;
}

