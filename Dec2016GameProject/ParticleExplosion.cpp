#include "ParticleExplosion.h"



ParticleExplosion::ParticleExplosion(float x, float y, Texture*texture, Color*color)
	:texture(texture),color(color)
{
	position.set(x, y);
	calculateVel();
	size = (float)(rand()%10+5);
	damping = 0.95f;
	lifeSpan = 255*2;
}


ParticleExplosion::~ParticleExplosion()
{
	position.set(0.0f, 0.0f);
	velocity.set(0.0f, 0.0f);
	acceleration.set(0.0f, 0.0f);
	texture = NULL;
	color = NULL;
	size = 0.0f;
	damping = 0.0f;
	lifeSpan = 0;
}

void ParticleExplosion::display(const Camera & camera)
{
	//update
	Vector2D gravity(0.0f, 0.05f);
	acceleration.x += gravity.x;
	acceleration.y += gravity.y;

	velocity.x += acceleration.x;
	velocity.y += acceleration.y;
	
	velocity.x *= damping;
	velocity.y *= damping;

	position.x += velocity.x;
	position.y += velocity.y;
	
	lifeSpan -= 16;
	if (lifeSpan < 0)
		lifeSpan = 0;
	//display
	texture->setColor(color);
	if (lifeSpan < 255)
		texture->setAlpha(lifeSpan);
	texture->render(camera,
		position.x - size*0.5f - camera.getPosition().x,
		position.y - size*0.5f - camera.getPosition().y, size, size, 0, 0.0f, false);
	texture->setColor(0xff,0xff,0xff);
	texture->setAlpha(255);

}

bool ParticleExplosion::done() const
{
	return (lifeSpan <= 0);
}

void ParticleExplosion::adjust(float damping, int lifespan)
{
	this->damping = damping;
	this->lifeSpan = lifespan;
}

void ParticleExplosion::calculateVel()
{
	float angle = (float)(rand() % 360 + 1) / 360.0f*(2.0f*PI);
	float speed = (float)(rand() % 16+9);

	velocity.x = speed*cosf(angle);
	velocity.y = speed*sinf(angle);
}
