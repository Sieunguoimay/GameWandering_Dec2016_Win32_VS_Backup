#include "Monster_Tomato.h"



int MonsterTomato::numberMonster = 0;
MonsterTomato::MonsterTomato(b2World*world)
	:Shape(world)
{
	position = new Vector2D();
	velocity = new Vector2D();
	desired = new Vector2D();
	target = new Vector2D();
	data = new ObjectData(SomeDefine::MONSTER_TOMATO, this);
	numberMonster++;

}

MonsterTomato::~MonsterTomato()
{
	numberMonster--;
	world->DestroyBody(body);
	body = NULL;
	delete data;
	data = NULL;
	delete position;
	position = NULL;
	delete velocity;
	velocity = NULL;
	delete target;
	target = NULL;
	delete desired;
	desired = NULL;
	sound = NULL;
	sound2 = NULL;
	textureExplosion = NULL;
	outOfScreen = false;
	findNewTarget = false;
}


void MonsterTomato::setup(float x, float y, Size * size, Texture*texture, Texture*textureExplosion, Sound*sound, Sound*sound2)
{	
	//all the pointer
	this->size = size;
	this->skin = texture;
	this->sound = sound;
	this->sound2 = sound2;
	this->textureExplosion = textureExplosion;
	
	//some switcher
	findNewTarget = true;
	outOfScreen = false;
	
	currentSpriteIndex = 0;
	
	maxForce = 10.0f;
	maxSpeed = 700.0f;//0k

	//a circle body for this monster. we need as simple as posible 
	body = addCircle(x, y, size->w, true, 0.02f, false);
	body->SetUserData(data);

}

void MonsterTomato::seek(Mover*mover)
{
	//update mover's target for lightning :))
	mover->setTarget(*position);

	//flip update here
	float newPosX = body->GetPosition().x*UtilityFunctions::M2P();
	if ((int)(newPosX - oldPosX) != 0)
		flip = (velocity->x > 0);
	oldPosX = newPosX;


	//all are in world units
	//if mover is nonNULL then find it
	timePerReaching = 0;
	if (findNewTarget) {//if reached target then point to new target
		target->set(mover->getPosition()->x,position->y);
		findNewTarget = false;
		timePerReaching = 50;
	}

	velocity->set(
		body->GetLinearVelocity().x*UtilityFunctions::M2P(),
		body->GetLinearVelocity().y*UtilityFunctions::M2P());
	
	desired->set(newPosX,position->y);

	desired->subtract(target);
	float distance = desired->magnitude();

	if (distance < size->w * 5) {//if the target is in a distance of 10 times its width
		desired->normalize();
		//update sprites
		grab();
		if (distance < size->w) {//slow down as distance is smaller and smaller
			float m = UtilityFunctions::map(distance, 0, 100.0f, 0, maxSpeed);
			desired->multiply(-m);
		}
		else desired->multiply(-maxSpeed);

		if (timePerReaching > 0)// if time for a reaching time is up, then set to initialization and find new target
			timePerReaching--;
		else findNewTarget = true;

		desired->subtract(velocity);
		desired->limit(maxForce);
		applyForce(desired);
	}
	else findNewTarget = true;
}
	
void MonsterTomato::display(const Camera&camera)
{

	position->set(body->GetPosition().x*UtilityFunctions::M2P(), body->GetPosition().y*UtilityFunctions::M2P());
	//if (!explored)
	skin->render(
		camera,
		position->x - camera.getPosition().x - size->w/2,
		position->y - camera.getPosition().y - size->h/ 2,
		size->w,
		size->h,
		currentSpriteIndex, 0, flip);

	if (position->y > camera.getPosition().y + WINDOWSIZE_H*2.0f) {
		outOfScreen = true;
		dead = true;
	}
}

bool MonsterTomato::isDead() const
{
	return dead;
}

bool MonsterTomato::isOutOfScreen() const
{
	return outOfScreen;
}

void MonsterTomato::touchMover()
{
	//if touch mover then what happen????
	dead = true;
}

Vector2D * MonsterTomato::getPosition()
{
	return this->position;
}


void MonsterTomato::applyForce(Vector2D * force)
{
	body->ApplyForce(b2Vec2(force->x*UtilityFunctions::P2M(), force->y*UtilityFunctions::P2M()), body->GetWorldCenter(), true);
}

void MonsterTomato::grab()
{
	static float timeCounter = 0.0f;
	float framePerSecond = 10;
	timeCounter += 1.0f / FPS;
	if (timeCounter >= 1 / framePerSecond) {
		currentSpriteIndex++;
		if (currentSpriteIndex >= 5) {
			currentSpriteIndex = 0;
			sound2->play(10+numberMonster);
		}
		timeCounter = 0.0f;
	}
}
