#pragma once
#ifndef GAMECURSOR_H
#define GAMECURSOR_H
#include"globalheader.h"
#include"Texture.h"
class GameCursor
{
public:
	GameCursor(Texture*texture);
	~GameCursor();
	void handleEvent(SDL_Event&mainEvent);
	void display();
	void coverButton(bool state);
	void setPosition(float x, float y);
	Vector2D*getPosition();
	bool isButtonPressed();
private:
	Texture*texture;
	Vector2D*position;
	bool state;
	bool buttonPressed;

};

#endif // !GAMECURSOR_H
