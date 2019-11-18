#include "Button.h"



Button::Button(float x, float y, float w, float h, Texture*texture, Sound*sound, Sound*sound2)
	:ButtonDefault(x,y,w,h,texture),sound(sound),sound2(sound2)
{
	buttonState = 0;//0123
	dead = false;
}


Button::~Button()
{
	dead = false;
	delete rectangle;
	rectangle = NULL;
	sound = NULL;
	sound2 = NULL;
	texture = NULL;
}

void Button::reset()
{
	dead = false;
	pressed = false;
	buttonState = NORMAL;
}

void Button::readMouseState(bool state)
{
	if (state) {
		buttonState = PUSH;
		pressed = true;
	}
	else {
		if (buttonState == NORMAL)
			sound2->play();
		buttonState = ACTIVE;
	}
}

void Button::handleEvent(SDL_Event & mainEvent)
{
	if (mainEvent.type == SDL_MOUSEBUTTONDOWN) {
		buttonState = PUSH;
		pressed = true;
	}else if (mainEvent.type == SDL_MOUSEBUTTONUP) {
		buttonState = ACTIVE;
		pressed = false;
	}
	else if (mainEvent.type == SDL_MOUSEMOTION) {
		if (pressed)
			buttonState = PUSH;
		else
		{
			if (buttonState == NORMAL)
				sound2->play();
			buttonState = ACTIVE;
		}
	}
}

void Button::setButtonState(int buttonState)
{
	this->buttonState = buttonState;
}

void Button::display()
{
	texture->render((int)(rectangle->x - rectangle->w / 2),(int)( rectangle->y - rectangle->h / 2),
		(int)rectangle->w,(int)rectangle->h,buttonState);
	if (pressed) {
		if (!dead)
			sound->play();
		dead = true;
	}
}

bool Button::checkInside(float mouseX, float mouseY) const
{
	if (mouseX < rectangle->x - rectangle->w/2)
		return false;

	if (mouseX > rectangle->x+rectangle->w/2)
		return false;

	if (mouseY < rectangle->y - rectangle->h/2)
		return false;

	if (mouseY > rectangle->y + rectangle->h/2)
		return false;
	return true;
}

bool Button::isPressed() const
{
	return dead;
}

ButtonDefault::ButtonDefault(float x, float y, float w, float h, Texture * texture)
	:texture(texture)
{
	rectangle = new Rectangle(x, y, w, h);
}

ButtonDefault::~ButtonDefault()
{
}

void ButtonDefault::handleEvent(SDL_Event & mainEvent)
{
}

void ButtonDefault::setButtonState(int buttonState)
{
}

void ButtonDefault::display()
{
	texture->render((int)(rectangle->x - rectangle->w / 2.0f), (int)(rectangle->y - rectangle->h / 2.0f), 
		(int)rectangle->w, (int)rectangle->h, ButtonState::DISABLED);

}

bool ButtonDefault::checkInside(float x, float y) const
{
	return false;
}

bool ButtonDefault::isPressed() const
{
	return false;
}

Rectangle * ButtonDefault::getRectangle()
{
	return rectangle;
}

void ButtonDefault::setPosition(float x, float y)
{
	rectangle->x = x;
	rectangle->y = y;
}

Size * ButtonDefault::getTextureSize()
{
	return texture->getSize();
}
