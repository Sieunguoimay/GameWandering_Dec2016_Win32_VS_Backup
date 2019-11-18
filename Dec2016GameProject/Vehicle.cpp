#include "Vehicle.h"



Vehicle::Vehicle(b2World*world, Texture*texture)
	:world(world),texture(texture)
{
	position = new Vector2D();
	velocity = new Vector2D();
	acceleration = new Vector2D();
	size = new Size;
}


Vehicle::~Vehicle()
{
	if (bodyCreated)
	{
		world->DestroyBody(body);
		body = NULL;
	}
	world = NULL;
	bodyCreated = false;
	delete size;
	size = NULL;
	delete position;
	position = NULL;
	delete velocity;
	velocity = NULL;
	delete acceleration;
	acceleration = NULL;
}

void Vehicle::setup(float x, float y)
{
	bodyCreated = false;
	this->mass = 1.0f;
	this->maxForce = 1.0f;
	this->maxSpeed = 10.0f;
	this->position->set(x, y);
	this->size->set(40.0f, 20.0f);
	createBody();
}

//do nothing than update the movement of vehicle and render the texture
void Vehicle::display(const Camera & camera)
{
	updateMovement();
	//update angle - or direction of the texture
	static const float MINIMUM_VELOCITY = 0.005f;
	if (velocity->magnitude() > MINIMUM_VELOCITY)//incase velocity is equal to 0, let angle remain
		angle = velocity->getAngle();
	//update the velocity of the body	
	if (bodyCreated)
		body->SetLinearVelocity(b2Vec2(velocity->x *0.5f, velocity->y *0.5f));

	texture->render(camera,
		position->x - size->w*0.5f - camera.getPosition().x, 
		position->y - size->h*0.5f - camera.getPosition().y,
		size->w, size->h,0, angle,false);
}

void Vehicle::applyForce(Vector2D&force)
{
	Vector2D componentAcc = Vector2D::divide(force, mass);
	acceleration->add(&componentAcc);
}

bool Vehicle::seek(Vector2D & target, float maxForce)
{
	bool sought = false;
	Vector2D desired = Vector2D::subtract(&target, position);
	float d = desired.magnitude();
	desired.normalize();

	//to slow down the speed of vehicle
	if (d < 100) {
		float m = UtilityFunctions::map(d, 0, 100, 0, maxSpeed);
		desired.multiply(m);
		sought = true;
	}
	else desired.multiply(maxSpeed);
	//create steer
	Vector2D steer = Vector2D::subtract(&desired, velocity);
	steer.limit(maxForce);
	applyForce(steer);
	return sought;
}

void Vehicle::separate(LinkedList<Vehicle*>* otherVehicles)
{
	const int desiredSpace = (const int)size->getHalfPerimeter();
	int counter = 0;
	Vector2D sum;// = new Vector2D();//create one time use many times - when will it be destroyed???
	for (Node<Vehicle*>*iter = otherVehicles->getFirstNode(); iter != otherVehicles->tail; iter = iter->next) {
		float distance = Vector2D::distance(position, iter->data->position);
		if (distance > 0 && distance < desiredSpace) {
			Vector2D difference = Vector2D::subtract(*position, *iter->data->position);
			difference.normalize();
			difference.divide(distance);
			sum.add(&difference);
			counter++;
		}
	}
	if (counter > 0) {
		sum.divide((float)counter);
		sum.normalize();
		sum.multiply(maxSpeed);

		Vector2D steeringForce = Vector2D::subtract(sum, *velocity);
		steeringForce.limit(maxForce);
		applyForce(steeringForce);
	}
}

Vector2D * Vehicle::getPosition()
{
	return position;
}

void Vehicle::updateMovement()
{
	//lifespan--;
	velocity->add(acceleration);
	velocity->limit(maxSpeed);
	position->add(velocity);
	acceleration->multiply(0);//i know why this is 0 =))
}


//let it simple by only creating a box
//this body is nothing more than a sensor for checking collision
//so set the data for this body inside the object it serves
void Vehicle::createBody()
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(position->x*UtilityFunctions::P2M(), position->y*UtilityFunctions::P2M());
	bd.gravityScale = 0;
	body = world->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(0.5f*size->w* UtilityFunctions::P2M(), 0.5f*size->h * UtilityFunctions::P2M());

	b2FixtureDef fd;
	fd.shape = &ps;
	fd.isSensor = true;
	body->CreateFixture(&fd);
	bodyCreated = true;
}

