#include "Poster.h"



Poster::Poster( Texture*texture, Texture*texturePressAnyKey, GameCursor*mouseCursor, SoundContainer*soundCollection)
	:texture(texture),texturePressAnyKey(texturePressAnyKey),mouseCursor(mouseCursor),soundCollection(soundCollection)
{
	done = false;
	keyPressed = false;
	position = new Vector2D(0,0);
	velY = 0.0f;
	oscilationY = -texturePressAnyKey->getSize()->h;
	angle = 0.0f; 
}


Poster::~Poster()
{
	delete position;
	position = NULL;
	velY = 0.0f;
	keyPressed = false;
	done = false;
	texture = NULL;
	texturePressAnyKey = NULL;
	mouseCursor = NULL;
	soundCollection = NULL;
}

void Poster::handleEvent(SDL_Event & mainEvent)
{
	if (mainEvent.type == SDL_KEYDOWN || mainEvent.type == SDL_MOUSEBUTTONDOWN){
		if (!keyPressed) {
			soundCollection->at(13)->play();
			keyPressed = true;
		}
	}
	mouseCursor->handleEvent(mainEvent);
}

void Poster::display(const Camera&camera)
{
	float rateW = camera.getSize().w / texture->getSize()->w;
	float rateH = camera.getSize().h / texture->getSize()->h;
	if (rateW > rateH) {
		float w = rateH*texture->getSize()->w;
		float h = camera.getSize().h;
		float x = camera.getSize().w / 2 - w / 2;
		float y = position->y+ camera.getSize().h / 2 - h / 2;// -texture->getSize()->h / 2;

		texture->render(camera, x, y,w,h, 0);
	} else {
		float w = camera.getSize().w;
		float h = rateW*texture->getSize()->h;
		float x = camera.getSize().w / 2 - w / 2;
		float y = position->y + camera.getSize().h / 2 - h / 2;// -texture->getSize()->h / 2;

		texture->render(camera,x,y,w,h,0);
	}
	texturePressAnyKey->render(camera, camera.getSize().w / 2 - texturePressAnyKey->getSize()->w / 2, oscilationY, texturePressAnyKey->getSize()->w, texturePressAnyKey->getSize()->h);

	if (keyPressed) {
		float dest = -camera.getSize().h;
		velY = 0.1f*(dest - position->y);
		if (velY > -4.0f)
			velY = -4.0f;
		position->y += velY;
		oscilationY += velY;
		if (position->y <= dest)
			done = true;
	}
	else {
		angle += 0.02f;
		if (angle >= 2.0f*PI)
			angle = 0.0f;
		oscilationY = camera.getSize().h*0.9f + sinf(angle)*texturePressAnyKey->getSize()->h;
	}
	mouseCursor->display();
}

bool Poster::hasDone()
{
	return done;
}
