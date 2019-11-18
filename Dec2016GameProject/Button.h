#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#include"globalheader.h"
#include"Texture.h"
#include"Sound.h"
enum ButtonState {
	NORMAL,
	ACTIVE,
	PUSH,
	DISABLED
};
class ButtonDefault {
public:
	ButtonDefault(float x, float y, float w, float h, Texture*texture);
	~ButtonDefault();
	virtual void handleEvent(SDL_Event&mainEvent);
	virtual void setButtonState(int buttonState);
	virtual void display();
	virtual bool checkInside(float x, float y)const;
	virtual bool isPressed()const;
	Rectangle*getRectangle();
	void setPosition(float x, float y);
	Size*getTextureSize();
protected:
	Rectangle*rectangle;
	Texture*texture;
	int buttonState;
};
class Button :public ButtonDefault
{
public:
	Button(float x, float y, float w, float h,Texture*texture, Sound*sound, Sound*sound2);
	~Button();
	void reset();
	void readMouseState(bool state);
	void handleEvent(SDL_Event&mainEvent);
	void setButtonState(int buttonState);
	void display();
	bool checkInside(float x, float y)const;
	bool isPressed()const;
private:
	bool dead;
	bool pressed;
	Sound*sound;
	Sound*sound2;
};

#endif // !BUTTON_H
