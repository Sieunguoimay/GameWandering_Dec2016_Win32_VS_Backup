#pragma once
#ifndef POSTER_H
#define POSTER_H
#include"globalheader.h"
#include"Texture.h"
#include"Camera.h"
#include"GameCursor.h"
#include"SoundContainer.h"
class Poster
{
public:
	Poster(Texture*texture,Texture*texturePressAnyKey, GameCursor*mouseCursor, SoundContainer*soundCollection);
	~Poster();
	void handleEvent(SDL_Event&mainEvent);
	void display(const Camera&camera);
	bool hasDone();
private:
	SoundContainer*soundCollection;
	float oscilationY;
	float angle;

	Texture*texture;
	Texture*texturePressAnyKey;
	bool keyPressed;
	bool done;
	Vector2D* position;
	float velY;
	GameCursor*mouseCursor;
};

#endif // !POSTER_H
