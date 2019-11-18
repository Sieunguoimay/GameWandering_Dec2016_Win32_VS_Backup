#include "GameCursor.h"



GameCursor::GameCursor(Texture*texture)
	:texture(texture)
{
	position = new Vector2D();
	state = false;
}


GameCursor::~GameCursor()
{
	delete position;
	position = NULL;
	state = false;
}

void GameCursor::handleEvent(SDL_Event & mainEvent)
{
	position->set((float)mainEvent.motion.x, (float)mainEvent.motion.y);
	if (mainEvent.type == SDL_MOUSEBUTTONDOWN)
	{
		state = true;
		buttonPressed = true;
	}
	if (mainEvent.type == SDL_MOUSEBUTTONUP)
	{
		state = false;
		buttonPressed = false;
	}
}

void GameCursor::display()
{
	texture->render(
		(int)position->x, (int)position->y,
		40, (int)(40.0f / texture->getSize()->w*texture->getSize()->h), state);
}

Vector2D * GameCursor::getPosition()
{
	return position;
}

bool GameCursor::isButtonPressed()
{
	return buttonPressed;
}

void GameCursor::coverButton(bool state)
{
	this->state = state;
}

void GameCursor::setPosition(float x, float y)
{
	this->position->set(x, y);
}
