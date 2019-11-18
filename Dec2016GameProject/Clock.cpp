#include "Clock.h"





Clock::Clock(float FPS)
	:FPS(FPS)
{
}

Clock::~Clock()
{
	timeStart = 0;
	FPS = 0;
}

void Clock::work()
{
	if ((Uint32)(1000 / FPS) > (SDL_GetTicks() - timeStart))
		SDL_Delay((Uint32)(1000 / FPS) - (SDL_GetTicks() - timeStart));
	
	timeStart = SDL_GetTicks();
}
