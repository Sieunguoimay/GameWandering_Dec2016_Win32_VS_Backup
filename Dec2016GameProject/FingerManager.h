#pragma once
#ifndef FINGERMANAGER_H
#define FINGERMANAGER_H
#include"globalheader.h"

struct Finger {
	Finger()
	{
		position.set(0.0f, 0.0f);
		bool pressed = false;
	}
	void set(float x, float y, bool pressed) {
		this->position.set(x, y);
		this->pressed = pressed;
	}
	Vector2D position;
	bool pressed;
};

class FingerManager
{
public:
	FingerManager();
	~FingerManager();
	void handleEvent(SDL_Event&mainEvent);
	Finger*getFinger();
private:
	Finger*fingers;
};

#endif // FINGERMANAGER_H
