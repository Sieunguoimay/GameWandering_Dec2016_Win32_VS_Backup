#include "Cloud.h"



Cloud::Cloud(b2World*world, Texture*cloudTexture,Texture*lightningTexture, Sound*sound, bool isDarkCloud)
	:cloudTexture(cloudTexture),isDarkCloud(isDarkCloud)
{
	position = new Vector2D();
	velocity = new Vector2D();
	acceleration = new Vector2D();
	size = new Size();
	area = new Size();
	targetForLightning = new LinkedList<Vector2D*>();
	lightning = new LightningKiller(world,lightningTexture,cloudTexture,sound);
}


Cloud::~Cloud()
{
	for (Node<Vector2D*>*iter = targetForLightning->getFirstNode(); iter != targetForLightning->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete targetForLightning;
	targetForLightning = NULL;
	delete lightning;
	lightning = NULL;
	delete position;
	position = NULL;
	delete velocity;
	velocity = NULL;
	delete acceleration;
	acceleration = NULL;
	delete size;
	size = NULL;
	isDarkCloud = false;
}

void Cloud::setup(float x, float y,float heigth)
{
	maxForce = 0.02f;
	maxSpeed = 1.0f;

	position->set(x, y);
	velocity->set((float)(rand() % 10+1), (float)(rand() % 10) / 10.0f);
	size->w = (float)(100 + rand() % 50);
	size->h = cloudTexture->getSize()->h * size->w/ cloudTexture->getSize()->w;

	//area is from position down to ground surface
	//area->x = position->x - size->w / 2;
	//area->y = position->y;
	area->w = size->w * 2.0f;
	area->h = heigth;
}

void Cloud::display(const Camera & camera)
{

	velocity->add(acceleration);
	velocity->limit(maxSpeed);
	position->add(velocity);
	acceleration->multiply(0.0f);


	for (Node<Vector2D*>*iter = targetForLightning->getFirstNode(); iter != targetForLightning->tail; iter = iter->next)
	{
		lightning->calculateDischarge(*position, *iter->data,
			//this line of code is for checking the cloud is inside the window or not
			position->x > camera.getPosition().x&&position->x < camera.getPosition().x + WINDOWSIZE_W);

		delete iter->data;
		iter->data = NULL;
		iter = targetForLightning->delNode(iter);
	}

	if (isDarkCloud)
	{
		if (targetForLightning->isEmpty()) {
			if (rand() % 10000 < 20) {
				lightning->calculateDischarge(*position,
					Vector2D(position->x + ((int)area->w / 2 ?(rand() % (int)area->w / 2):0) - area->w / 4,
						position->y + ((int)area->h / 3?rand() % (int)area->h / 3:0) + area->h / 3),
					//this line of code is for checking the cloud is inside the window or not
					position->x > camera.getPosition().x&&
					position->x < camera.getPosition().x + WINDOWSIZE_W);
			}
		}
		cloudTexture->setColor(0x55, 0x55, 0x55);
	}

	lightning->display(camera);
	cloudTexture->render(camera,
		(position->x - size->w / 2.0f - camera.getPosition().x),
		(position->y - size->h / 2.0f - camera.getPosition().y),
		size->w, size->h, 0);
	cloudTexture->setColor(0xff, 0xff, 0xff);

}

void Cloud::blow(LinkedList<Cloud*>&others)
{	
	applyForce(separate(others));
	applyForce(align(others));
	applyForce(cohesion(others));
}

void Cloud::stayWithinValleyArea(float x, float y, float w, float h)
{

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
	if (position->y < y ) {
		desired.set(velocity->x, maxSpeed);
		metWall = true;
	}
	else if (position->y > y + h) {
		desired.set(velocity->x, -maxSpeed);
		metWall = true;
	}
	if (metWall) {
		Vector2D steer = Vector2D::subtract(desired, *velocity);
		steer.limit(maxForce);
		applyForce(steer);
	}
}

bool Cloud::isInsideCloudArea(Vector2D * target) const
{

	if (target->x < position->x-area->w/2.0f)
		return false;
	if (target->x > position->x+area->w/2.0f)
		return false;
	if (target->y < position->y)
		return false;
	//if (target->y > position->y + area->h);
//		return false;
		//printf("xxx.%f    %f\n", target->y, position->y + area->h);

	return true;
}

void Cloud::workOnNewTarget(Vector2D * newTarget)
{
	if (isInsideCloudArea(newTarget)) {
		if (targetForLightning->isEmpty())
			targetForLightning->addFirst(new Vector2D(newTarget));
		else if (newTarget->y < targetForLightning->getFirstNode()->data->y)
		{
			delete targetForLightning->getFirstNode()->data;
			targetForLightning->emptyList();
			targetForLightning->addFirst(new Vector2D(newTarget));
		}
	}
	/*else {
		if (!targetForLightning->isEmpty()) {
			delete targetForLightning->getFirstNode()->data;
			targetForLightning->emptyList();
		}
	}*/
}

Vector2D Cloud::align(LinkedList<Cloud*>& flock)
{
	float neighborDistanceSqr = 50.0f * 50.0f;//visible distance of a fish
	Vector2D sum;
	int visibleFishCounter = 0;
	for (Node<Cloud*>*iter = flock.getFirstNode(); iter != flock.tail; iter = iter->next) {
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

Vector2D Cloud::cohesion(LinkedList<Cloud*>& flock)
{
	Vector2D sum;
	float visibleFishCounter = 0.0f;
	float visibleDistanceSqr = 50.0f * 50.0f;
	for (Node<Cloud*>*iter = flock.getFirstNode(); iter != flock.tail; iter = iter->next) {
		float distanceSqr = Vector2D::distanceSqr(position, iter->data->position);
		if (distanceSqr > 0 && distanceSqr < visibleDistanceSqr) {
			sum.add(iter->data->position);
			visibleFishCounter++;
		}
	}
	if (visibleFishCounter > 0) {
		sum.divide(visibleFishCounter);
		return seek(sum);
	}
	else
		return Vector2D();

}

Vector2D Cloud::separate(LinkedList<Cloud*>& others)
{
	const int desiredSpace = (const int)(size->getHalfPerimeter()/2.0f);
	int counter = 0;
	Vector2D sum;// = new Vector2D();//create one time use many times - when will it be destroyed???
	for (Node<Cloud*>*iter = others.getFirstNode(); iter != others.tail; iter = iter->next) {
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
		return steeringForce;
		//applyForce(steeringForce);
	}
	return Vector2D();
}

Vector2D Cloud::seek(Vector2D&target)
{
	Vector2D desired = Vector2D::subtract(target, *position);
	float d = desired.magnitude();
	desired.normalize();

	//to slow down the speed of vehicle
	if (d < 100.0f) {
		float m = UtilityFunctions::map(d, 0.0f, 100.0f, 0.0f, maxSpeed);
		desired.multiply(m);
	}
	else desired.multiply(maxSpeed);

	//create steer
	Vector2D steer = Vector2D::subtract(desired, *velocity);
	steer.limit(maxForce);

	return steer;

}

void Cloud::applyForce(Vector2D & force)
{
	acceleration->add(force.x, force.y);
}
