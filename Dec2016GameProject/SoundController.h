#pragma once
#ifndef SOUNDCONTROLLER_H
#define SOUNDCONTROLLER_H
#include"Button.h"
#include"GameCursor.h"
#include"SoundContainer.h"
class SoundController
{
public:
	SoundController(float volume,GameCursor*mouseCursor, 
		Texture*buttonTexture, Texture*movingButtonTexture, Texture*frameTexture,
		Texture*dynamicTexture,SoundContainer*soundCollection);
	~SoundController();
	bool handleEvent(SDL_Event&mainEvent);
	void display();
	void setPosition(float x, float y);
	Vector2D*getPosition();
	float getVolume()const;
private:
	void setup(float x, float y, float size);
	Vector2D*position;
	Button*button;
	Button*movingButton;
	Button*frameButton;
	Texture*dynamicTexture;
	float volume;
	bool switchOnOff;
	GameCursor*mouseCursor;
};

#endif // !SOUNDCONTROLLER_H
