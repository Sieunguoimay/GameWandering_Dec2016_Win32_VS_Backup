#include "ParticleSystem.h"



ParticleSystem::ParticleSystem()
{
	particles = new LinkedList<Particle*>();
}


ParticleSystem::~ParticleSystem()
{
	for (Node<Particle*>*iter = particles->getFirstNode(); iter != particles->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete particles;
	particles = NULL;
	//call the destructor of the base class Particle
}

void ParticleSystem::display(const Camera & camera)
{
	update();
	for (Node<Particle*>*iter = particles->getFirstNode(); iter != particles->tail; iter = iter->next) {
		iter->data->display(camera);
		if (iter->data->isDead()) {
			delete iter->data;
			iter->data = NULL;
			iter = particles->delNode(iter);
		}
	}
}

bool ParticleSystem::isDead()
{
	return (particles->getSize()==0);
}

void ParticleSystem::update()
{
	if (lifespan > 0&&particles->getSize()<20)
		putParticle();
	Particle::update();//to update its own Vectors
}

void ParticleSystem::updateSize()
{
	this->size->w -= speedOfLifespan;
	this->size->h -= speedOfLifespan;
}


void ParticleSystem::putParticle()
{
	//calculate random velocity
	float angle = (float)(rand() % 360 + 1)*3.141592f / 180.0f;
	float maxSpeed = (float)(rand() % 10 - 5) / 5.0f;

	Particle*newParticle = new Particle();
	if (newParticle) {
		newParticle->setup(
			new Vector2D(position),
			new Vector2D(maxSpeed*cosf(angle), maxSpeed*sinf(angle)),
			new Size(size->w, size->h), texture, 255, 20);
		particles->addFirst(newParticle);
	}
}
