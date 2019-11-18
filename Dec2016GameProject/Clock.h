#pragma once
#ifndef CLOCK_H
#define CLOCK_H
#include<SDL.h>

class Clock
{
public:
	Clock(float FPS = 0.0f);
	~Clock();
	void work();
private:
	float FPS;
	Uint32 timeStart;
};

#endif // !CLOCK_H
