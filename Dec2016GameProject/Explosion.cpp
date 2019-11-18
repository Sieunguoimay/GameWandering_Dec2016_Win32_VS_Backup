#include "Explosion.h"



Explosion::Explosion()
{
	particles = new LinkedList<ParticleExplosion*>();
}


Explosion::~Explosion()
{
	//we don't need to delete each data here
	//because when container reaches here
	//it's empty.
	for (Node<ParticleExplosion*>*iter = particles->getFirstNode(); iter != particles->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete particles;
	particles = NULL;
	delete color;
	color = NULL;
}

void Explosion::setup(Vector2D * position, Texture * texture, Sound*sound, bool mute)
{
	this->position = position;
	this->texture = texture;//pointer to set the color before display
	this->soundEffect = sound;
	color = new Color(255 - rand()%10, rand() % 20+20, rand() % 10);
	
	int randNumberParticles = rand() % 50 + 50;
	for (int i = 0; i < randNumberParticles; i++)
		putAParticleSystem(*texture);
	if (!mute)
		soundEffect->play();
}

void Explosion::display(const Camera & camera)
{
	for (Node<ParticleExplosion*>*iter = particles->getFirstNode(); iter != particles->tail; iter = iter->next)
	{
		iter->data->display(camera);
		if (iter->data->done()) {
			delete iter->data;
			iter->data = NULL;
			iter = particles->delNode(iter);
		}
	}
}

bool Explosion::isDead() const
{
	return (particles->getSize() == 0);
}

void Explosion::putAParticleSystem(Texture&texture)
{
	//calculate random velocity
	//float angle = (float)(rand() % 360 + 1)*PI/180.0f;
	//float maxSpeed = (float)(rand()%20+20);
	//float radius = (float)(rand() % 100 + 50);
	ParticleExplosion*newParticle = new ParticleExplosion(position->x,position->y,&texture,color);
	if (newParticle)
		particles->addFirst(newParticle);
}

Explosion2::Explosion2()
{
}

Explosion2::~Explosion2()
{
}

void Explosion2::setup(Vector2D * position, Texture * texture, Sound * sound)
{
	this->position = position;
	this->texture = texture;//pointer to set the color before display
	this->soundEffect = sound;
	color = new Color(150 - rand() % 10, 150 - rand() % 10,150 - rand() % 10);

	int randNumberParticles = rand() % 20 + 20;
	for (int i = 0; i < randNumberParticles; i++)
		putAParticleSystem(*texture);
	soundEffect->play();
}


void Explosion2::putAParticleSystem(Texture & texture)
{
	ParticleExplosion*newParticle = new ParticleExplosion(position->x, position->y, &texture, color);
	newParticle->adjust(0.6f, 255);
	if (newParticle)
		particles->addFirst(newParticle);
}
