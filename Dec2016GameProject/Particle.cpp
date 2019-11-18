#include "Particle.h"



Particle::Particle()
{
}


Particle::~Particle()
{
	delete accelaration;
	accelaration = NULL;
	delete position;
	position = NULL;
	delete velocity;
	velocity = NULL;
	delete size;
	size = NULL;
	texture = NULL;
}

void Particle::setup(Vector2D * position, Vector2D * velocity, Size * size, Texture * texture, int lifespan, int speedOfLifespan)
{
	this->position = position;
	this->velocity = velocity;
	this->texture = texture;
	this->size = size;
	this->lifespan = lifespan;
	this->accelaration = new Vector2D();
	this->maxSize = size->w;//keep it
	this->speedOfLifespan = speedOfLifespan;
}

void Particle::display(const Camera & camera)
{
	update();
	updateParticle();
	texture->render(camera,
		position->x - size->w / 2 - camera.getPosition().x,
		position->y - size->h / 2 - camera.getPosition().y,
		size->w, size->h,0, 0, false);	
}

void Particle::applyForce(Vector2D & force)
{
	this->accelaration->set(&force);
}

bool Particle::isDead()
{
	return (lifespan <= 0);
}

void Particle::update()
{
	this->velocity->add(accelaration);
	this->position->add(velocity);
	lifespan -= speedOfLifespan;
}

void Particle::updateParticle()
{
	if (lifespan < 0)
		lifespan = 0;
	size->w = (maxSize*(float)(255 - lifespan) / 255.0f);
	size->h = (maxSize*(float)(255 - lifespan) / 255.0f);
	texture->setAlpha(lifespan);
}
