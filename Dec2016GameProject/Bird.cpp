#include "Bird.h"



Bird::Bird(b2World*world, Texture*texture, Path**path, Sound*pursueSound, Sound*deadSound)
	:Vehicle(world,texture),path(path),pursueSound(pursueSound),deadSound(deadSound)
{
	preyTarget = new LinkedList<Vector2D*>();
	data = new ObjectData(SomeDefine::BIRD_ID, this);
}


Bird::~Bird()
{
	//call the destructor of base class vehicle
	for (Node<Vector2D*>*iter = preyTarget->getFirstNode(); iter != preyTarget->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete preyTarget;
	preyTarget = NULL;
	delete data;
	data = NULL;
	pursueSound = NULL;
	deadSound = NULL;
	delayTimeAfterSoughtTarget = 0;
	pursuePreyForce = 0.0f;
	dead = false;
	pursueSoundPlayed = false;
}

void Bird::setup(float x, float y)
{
	this->mass = 1;
	this->maxForce = 1;
	this->maxSpeed = 20;
	this->position->set(x, y);
	this->size->set(20,texture->getSize()->h*20/ texture->getSize()->w);
	dead = false;
	pursueSoundPlayed = false;

	delayTimeAfterSoughtTarget = 0;
	pursuePreyForce = 20.0f; 
	createBody(); 
	this->body->SetUserData(data);
}

void Bird::fly()
{
	static Vector2D gravity(0, 0.30f);
	applyForce(gravity);

	if (!preyTarget->isEmpty())
	{
		pursuePrey();
	}
}

void Bird::followPath(Path * path)
{
	//make sure that all this Vector2D are used to store data inside this scope only
	Vector2D target;
	static Vector2D predictPosition;
	static Vector2D shiftDistance;
	static Vector2D normalPoint;
	static float distance;

	const float STEP_AHEAD = 25;
	//step 1: find predicted position
	predictPosition = *velocity->get();//new dynamic memory created
	predictPosition.normalize();
	predictPosition.multiply(STEP_AHEAD);
	predictPosition.add(position);



	//step 2: find the normal point - projection of predicted position on the current path
	//Vector2D*normalPoint = NULL;
	float minDistance = 10000.0f;//let it be a very large value - easy to be beaten by any value
	for (Node<Vector2D*>*iter = path->getPaths()->getFirstNode(); iter != path->getPaths()->tail->prev; iter = iter->next){
		//for (size_t i = 0; i < path->getPaths()->size() - 1; i++) {
		//create temporarity pointers - pass address only
		const Vector2D &vertice1 =iter->data;
		const Vector2D &vertice2 = iter->next->data;

		//calculate all the normal Point
		Vector2D&tempNormalPoint = getNormalPoint(predictPosition, vertice1, vertice2);

		//if the normal Point is outside the 2 vertices
		//set it to be the next vertice in order to the distance is greater than that next one's normal distance
		if (tempNormalPoint.x >UtilityFunctions::max(vertice2.x, vertice1.x)
			|| tempNormalPoint.x < UtilityFunctions::min(vertice2.x, vertice1.x))
			tempNormalPoint.set(vertice2);

		//get the normal distance to compare 
		float distance = Vector2D::distance(tempNormalPoint, predictPosition);
		if (minDistance > distance) {
			//change the min Distance value to use for the following loop
			minDistance = distance;

			//assign all the needing pointer
			normalPoint = tempNormalPoint;//pass address only
										  //if prevous check is true - there is a shiftDistance dynamic memory was created
			shiftDistance = Vector2D::subtract(vertice2, vertice1);//dynamic
		}
	}

	//step 3: find the target by moving the normal point along the direction of the path
	shiftDistance.normalize();
	shiftDistance.multiply(1.5f*STEP_AHEAD);

	target = Vector2D::add(normalPoint, shiftDistance);//dynamic

	//step 4: if the predicted position is off the path, seek that target to stay on the path
	distance = Vector2D::distance(predictPosition, normalPoint);
	if (distance > path->radius)
		seek(target);
}

float Bird::radar(Vector2D & moverPosition)
{
	float distanceSqr = Vector2D::distanceSqr(moverPosition, *position);
	if (preyTarget->isEmpty()) {
		if (stayThere()) {//so i swear that this stay there function should be placed there "---*---" in fish class 
			if (distanceSqr < radarRadius*radarRadius) {//fall into the radar's area
				preyTarget->addFirst(new Vector2D(moverPosition));
			}
		}
	}
	return distanceSqr;
}

void Bird::pursuePrey()
{
	for (Node<Vector2D*>*iter = preyTarget->getFirstNode(); iter != preyTarget->tail; iter = iter->next)
	{
		if (!pursueSoundPlayed) {
			pursueSound->play();
			pursueSoundPlayed = true;
		}
		if (seek(*iter->data,pursuePreyForce)) {
			//here babe--->>>     "---*---"
				delete iter->data;
				iter->data = NULL;
				iter = preyTarget->delNode(iter);
				pursueSoundPlayed = false;
		}
	}
}

void Bird::setRadarRadius(float radius)
{
	this->radarRadius = radius;
}

Vector2D * Bird::getPosition()
{
	return this->position;
}

bool Bird::isDead() const
{
	return dead;
}

void Bird::die(bool dieOfOut)
{
	dead = true;
	if (!dieOfOut)
		deadSound->play();
}

bool Bird::stayThere()
{
	delayTimeAfterSoughtTarget += 1.0f / FPS;
	if (delayTimeAfterSoughtTarget > 1.50f) {//1.5seconds
		delayTimeAfterSoughtTarget = 0;
		return true;
	}
	return false;
}

Vector2D Bird::getNormalPoint(const Vector2D & predicLocation, const Vector2D & vertice1, const Vector2D & vertice2)
{
	// TODO: insert return statement here
	Vector2D a = Vector2D::subtract(predicLocation, vertice1);
	Vector2D b = Vector2D::subtract(vertice2, vertice1);

	b.normalize();
	b.multiply(Vector2D::scalarMultiply(a, b));
	b.add(vertice1.x, vertice1.y);
	return b;
}
