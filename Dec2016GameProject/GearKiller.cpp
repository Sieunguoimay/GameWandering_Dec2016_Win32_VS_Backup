#include "GearKiller.h"



GearKiller::GearKiller(b2World*world, Mover * moverPointer, Vector2D * position, Texture*gearTexture, Texture*stickTexture, float radius)
	:world(world),moverPointer(moverPointer),staticPosition(position),gearTexture(gearTexture),stickTexture(stickTexture),gearRadius(radius)
{
	//having staticposition now
	//caculate the position of dynamicposition where to put gear
	//gearRadius = 40;
	dynamicPosition = new Vector2D(staticPosition->x, staticPosition->y - gearRadius * 4);

	//create data for the gear's body
	data = new ObjectData(SomeDefine::GEARKILLER_ID, this);
	setup();
}

GearKiller::~GearKiller()
{
	world->DestroyBody(gearBody);
	gearBody = NULL;
	world->DestroyBody(stickBody);
	stickBody = NULL;
	delete staticPosition;
	staticPosition = NULL;
	delete dynamicPosition;
	dynamicPosition = NULL;
	delete data;
	data = NULL;
	gearRadius = 0;
}

//create 2 bodys and join them with a motor revolution joint
void GearKiller::setup()
{
	//create greatBody
	//a circle body, dynamic, sensor
	//at dynamicPosition
		b2BodyDef bd;
		bd.position.Set(dynamicPosition->x*UtilityFunctions::P2M(), dynamicPosition->y*UtilityFunctions::P2M());
		//bd.userData = data;//be careful set it after create body
		bd.type = b2_dynamicBody;
		gearBody = world->CreateBody(&bd);

		b2CircleShape cs;
		cs.m_radius = gearRadius*UtilityFunctions::P2M();

		b2FixtureDef fd;
		fd.density = 0.01f;//a bit light
		fd.isSensor = true;
		fd.shape = &cs;

		gearBody->CreateFixture(&fd);
		gearBody->SetUserData(data);
	//finish creating gear's body
	

	//create stick's body
	//a rectangular shape, dynamic, sensor
	//at the position of middle of staticPosition and dynamicPosition

	b2BodyDef bd2;
	bd2.position.Set(
		(staticPosition->x)*UtilityFunctions::P2M(),
		0.5f*(dynamicPosition->y + staticPosition->y)*UtilityFunctions::P2M());
	bd2.type = b2_dynamicBody;
	stickBody = world->CreateBody(&bd2);

	b2PolygonShape ps;
	ps.SetAsBox(0.125f*gearRadius*UtilityFunctions::P2M(), 2.0f*gearRadius*UtilityFunctions::P2M());
	
	b2FixtureDef fd2;
	fd2.shape = &ps;
	fd2.density = 1;//a bit light
	fd2.isSensor = true;

	stickBody->CreateFixture(&fd2);
	//finish creating stick's body

	//finially create a Revolution joint with motor is enabled
	//at the position of gear's world center
	b2RevoluteJointDef rjd;
	rjd.Initialize(gearBody, stickBody, gearBody->GetWorldCenter());
	rjd.enableMotor = true;
	rjd.maxMotorTorque = 20;//N-m
	rjd.motorSpeed = 2.0f*3.141592f;
	rjd.collideConnected = false;
	world->CreateJoint(&rjd);
	//finish create a joint
}

void GearKiller::display(const Camera & camera)
{

	if (stickTexture != NULL) {
		b2Vec2 pos = stickBody->GetPosition();
		float angle = stickBody->GetAngle();
		stickTexture->render(camera,
			pos.x*UtilityFunctions::M2P() - camera.getPosition().x - gearRadius*0.125f,
			pos.y*UtilityFunctions::M2P() - camera.getPosition().y - gearRadius*2.0f,
			gearRadius*0.25f, gearRadius*4.0f, 0, angle, false);
		//printf("%f\n", angle);
	}

	if (gearTexture != NULL) {
		b2Vec2 pos = gearBody->GetPosition();
		float angle = gearBody->GetAngle();
		gearTexture->render(camera,
			pos.x*UtilityFunctions::M2P() - camera.getPosition().x - gearRadius,
			pos.y*UtilityFunctions::M2P() - camera.getPosition().y - gearRadius,
			2.0f*gearRadius, 2.0f*gearRadius, 0, angle, false);
	}

}
//plugin the gear killer to the static body
//at the position of the static position
void GearKiller::plugin(b2Body * staticBody, float motorSpeed)
{

	//finially create a Revolution joint with motor is enabled
	//at the position of gear's world center
	b2RevoluteJointDef rjd;
	rjd.Initialize(staticBody, stickBody, b2Vec2(staticPosition->x*UtilityFunctions::P2M(), staticPosition->y*UtilityFunctions::P2M()));
	rjd.enableMotor = true;
	rjd.maxMotorTorque = 200;//N-m
	rjd.motorSpeed = motorSpeed;
	rjd.collideConnected = false;
	world->CreateJoint(&rjd);
	//finish create a joint

}

void GearKiller::setRadius(float radius)
{
	this->gearRadius = radius;
}

Vector2D * GearKiller::getDynamicPosition()
{
	return dynamicPosition;
}

b2Body * GearKiller::getStickBody()
{
	return stickBody;
}
