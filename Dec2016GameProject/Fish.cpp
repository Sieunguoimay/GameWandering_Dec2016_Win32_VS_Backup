#include "Fish.h"



Fish::Fish(b2World*world, Texture*texture)
	:Vehicle(world,texture)
{
	data = new ObjectData(SomeDefine::FISH_ID, NULL);
	preyTarget = new LinkedList<Vector2D*>();
}


Fish::~Fish()
{
	delete data;
	data = NULL;
	for (Node<Vector2D*>*iter = preyTarget->getFirstNode(); iter != preyTarget->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete preyTarget;
	preyTarget = NULL;
}

void Fish::setup(float x, float y)
{
	mass = 1.0f;
	maxSpeed = 5.0f;
	maxForce = 0.1f;
	this->pursuePreyForce = 5.0f;
	this->delayTimeAfterSoughtTarget = 0.0f;

	position->set(x, y);
	size->set(40.0f, texture->getSize()->h*40.0f/ texture->getSize()->w);

	createBody();
	this->body->SetUserData(data);

	//randomly create velocity
	float anlge = ((float)(rand()%360)/180.0f)*PI;
	float speed = (float)(rand() % (int)maxSpeed + 1);
	velocity->set(cosf(angle)*speed, sinf(angle)*speed);
}

void Fish::flock(LinkedList<Fish*>* flock)
{
	Vector2D separationForce = separate(*flock);
	Vector2D alignmentForce = align(*flock);
	Vector2D cohesionForce = cohesion(*flock);

	separationForce.multiply(1.5f);
	alignmentForce.multiply(1.0f);
	cohesionForce.multiply(1.0f);

	applyForce(separationForce);
	applyForce(alignmentForce);
	applyForce(cohesionForce);
}

void Fish::clearAllForce()
{
	velocity->multiply(-1.0f);//good
}

Vector2D Fish::align(LinkedList<Fish*>& flock)
{
	float neighborDistanceSqr = 50.0f*50.0f;//visible distance of a fish
	Vector2D sum;
	int visibleFishCounter = 0;
	for (Node<Fish*>*iter = flock.getFirstNode(); iter != flock.tail; iter = iter->next) {
		float distanceSqr = Vector2D::distanceSqr(position, iter->data->position);
		if (distanceSqr > 0 && distanceSqr < neighborDistanceSqr) {
			sum.add(iter->data->velocity);
			visibleFishCounter++;
		}
	}

	if (visibleFishCounter > 0) {
		sum.divide((float)visibleFishCounter);
		sum.normalize();
		sum.multiply(maxSpeed);
		Vector2D steer = Vector2D::subtract(sum, *velocity);
		steer.limit(maxForce);
		return steer;//pass value to out side
	}
	else return Vector2D();//this data is used for one time
}


Vector2D Fish::cohesion(LinkedList<Fish*>& flock)
//cohesion: average position of all the other fishes
{
	Vector2D sum;
	float visibleFishCounter = 0;
	float visibleDistanceSqr = 50.0f * 50.0f;
	for (Node<Fish*>*iter = flock.getFirstNode(); iter != flock.tail; iter = iter->next) {
		float distanceSqr = Vector2D::distanceSqr(position, iter->data->position);
		if (distanceSqr > 0 && distanceSqr < visibleDistanceSqr) {
			sum.add(iter->data->position);
			visibleFishCounter++;
		}
	}
	if (visibleFishCounter > 0) {
		sum.divide(visibleFishCounter);
		return seekTarget(sum);
	}
	else
		return Vector2D();
}

bool Fish::stayWithinPool(float x, float y, float w, float h)
{
	float force = 2.0f*maxForce;
	bool outofWater = false;
	bool metWall = false;
	Vector2D desired;
	if (position->x < x) {
		desired.set(maxSpeed, velocity->y);
		metWall = true;
	}
	else if (position->x > x + w) {
		desired.set(-maxSpeed, velocity->y);
		metWall = true;
	}
	if (position->y < y - 40.0f) {
		desired.set(velocity->x, maxSpeed);
		metWall = true;
		outofWater = true;
	}
	else if (position->y > y + h) {
		desired.set(velocity->x, -maxSpeed);
		metWall = true;
	}
	desired.multiply(2.0f);
	if (outofWater)
	{
		desired.multiply(2.0f);
		force *= 2.0f;
	}

	if (metWall) {
		Vector2D steer = Vector2D::subtract(desired, *velocity);
		steer.limit(force);
		applyForce(steer);
	}
	return metWall;
}

bool Fish::stayThere()
{
	delayTimeAfterSoughtTarget += 1.0f / FPS;
	if (delayTimeAfterSoughtTarget > 1.50f) {//1.5seconds
		delayTimeAfterSoughtTarget = 0.0f;
		return true;
	}
	return false;
}

Vector2D Fish::seekTarget(Vector2D & target)
{

	Vector2D desired = Vector2D::subtract(&target, position);
	float d = desired.magnitude();
	desired.normalize();

	//to slow down the speed of vehicle
	if (d < 100) {
		float m = UtilityFunctions::map(d, 0, 100, 0, maxSpeed);
		desired.multiply(m);
	}
	else
		desired.multiply(maxSpeed);

	//create steer
	Vector2D steer = Vector2D::subtract(desired, *velocity);
	steer.limit(maxForce);

	return steer;
}

Vector2D Fish::separate(LinkedList<Fish*>& flock)
{
	const int desiredSpace = 20;
	int counter = 0;
	Vector2D sum;// = new Vector2D();//create one time use many times - when will it be destroyed???
	for (Node<Fish*>*iter = flock.getFirstNode(); iter != flock.tail; iter = iter->next) {
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

		Vector2D steeringForce = Vector2D::subtract(sum,*velocity);
		steeringForce.limit(maxForce);
		return steeringForce;
	}
	else return Vector2D();
}

void Fish::addPreyTarget(Vector2D & target)
{
	if (preyTarget->isEmpty())
		//if (stayThere())
			this->preyTarget->addFirst(new Vector2D(&target));
}
void Fish::pursuePrey()
{
	for (Node<Vector2D*>*iter = preyTarget->getFirstNode(); iter != preyTarget->tail; iter = iter->next)
	{
		if (seek(*iter->data, pursuePreyForce)) {
			//here babe--->>>     "---*---"
			delete iter->data;
			iter->data = NULL;
			iter = preyTarget->delNode(iter);
		}
	}
}
