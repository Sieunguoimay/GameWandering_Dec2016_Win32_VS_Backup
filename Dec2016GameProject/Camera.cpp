#include "Camera.h"



Camera::Camera(Size*windowSize)
	:size(windowSize)
{
	location = new Vector2D(-size->w/3,0);
	velocity = new Vector2D();
	acceleration = new Vector2D();
	desired = new Vector2D();
	target = new Vector2D();
}


Camera::~Camera()
{
	delete target;
	target = NULL;
	delete desired;
	desired = NULL;
	delete location;
	location = NULL;
	delete velocity;
	velocity = NULL;
	delete acceleration;
	acceleration = NULL;
	size = NULL;
}

void Camera::setup()
{
	//this->target = target;
	maxForce = 5.0f;
	maxSpeed = 12.0f;
}

void Camera::reset()
{
	delete target;
	target = NULL;
	delete desired;
	desired = NULL;
	delete location;
	location = NULL;
	delete velocity;
	velocity = NULL;
	delete acceleration;
	acceleration = NULL;
	//this is how we reset the class =>
	location = new Vector2D(-size->w / 3.0f, 0.0f);
	velocity = new Vector2D();
	acceleration = new Vector2D();
	desired = new Vector2D();
	target = new Vector2D();
}

void Camera::setTarget(float x, float y)
{
	target->set(x, y);
}

void Camera::update()
{
	float x = target->x - (size->w/2.0f)*WINDOWSIZE_W/size->w;
	float y = target->y - (size->h *2.0f/3.0f)*WINDOWSIZE_H/size->h;

	if (target->y*size->w / WINDOWSIZE_W > location->y+size->h||
		location->y+target->y*size->w / WINDOWSIZE_W < size->h/3.0f)
		desired->set(x, y);
	else
		desired->set(x, 0.0f);//set like that in order to camera just go right or left

	//calculate desired velocity
	desired->subtract(location);
	float d = desired->magnitude();
	desired->normalize();

	if (d < 50.0f) {
		float m = UtilityFunctions::map(d, 0.0f, 50.0f, 0.0f, maxSpeed);//the closer the smaller maxspeed is
		desired->multiply(m);
	}
	else desired->multiply(maxSpeed);

	//calculate steering force
	desired->subtract(velocity);//this vector now is steering for
	desired->limit(maxForce);

	acceleration->set(desired);

	velocity->add(acceleration);
	location->add(velocity);

}

const Vector2D&Camera::getPosition()const
{
	return *this->location;
}

const Size & Camera::getSize() const
{
	// TODO: insert return statement here
	return *size;
}
