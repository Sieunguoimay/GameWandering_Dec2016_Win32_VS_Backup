#include "Relector.h"




Relector::Relector(b2World * world, Texture*texture)
	:world(world), texture(texture){}

Relector::~Relector()
{
	world->DestroyBody(body);
	body = NULL;
	delete data;
	data = NULL;
}

void Relector::setup(float x, float y, float angle)
{
	this->x = x;
	this->y = y;
	this->angle = angle;
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());
	bd.gravityScale = 0;
	body = world->CreateBody(&bd);

	b2CircleShape cs;//we need a circle shape to the collision ends right when it change direction
	//it's what i am thinking
	cs.m_radius = 20 * UtilityFunctions::P2M();

	b2FixtureDef fd;
	fd.shape = &cs;
	fd.isSensor = true;
	body->CreateFixture(&fd);

	data = new ObjectData(SomeDefine::REFLECTOR_ID, this);
	body->SetUserData(data);
}

void Relector::display(const Camera & camera)
{
	//maybe there is nothing to display here
	if (texture != NULL)
		texture->render(camera, x-camera.getPosition().x - 20.0f, y - camera.getPosition().y - 20.0f
			, 40.0f, 40.0f, 0, angle);
}

void Relector::reflect(Photon & photon)
{
	//calculate the angle of the current velocity
	float velocityAngle = photon.velocity->getAngle();
	//printf("tested%f      %f\n", photon.velocity->x, photon.velocity->y);//this is the problem
	
	//I find out the phenomena of the problem
	// that's because checking contact of one photon to the reflector occurs 2 times or more.
	//which makes the velocity change ocsillatingly for 2 times
	//how to solve this problem is unknown. 
	//let's think ablou it tomorrow

	if (angle < velocityAngle&&angle+PI > velocityAngle) {//everything is solved with this only line of code =))
		float deltaAngle = angle - velocityAngle;
		velocityAngle += 2.0f * deltaAngle;
		float magnitude = photon.velocity->magnitude();
		photon.velocity->set(magnitude*cosf(velocityAngle), magnitude*sinf(velocityAngle));//thats the story
	}
}
