#pragma once

#ifndef MAINMANAGER_H
#define MAINMANAGER_H
#include<ctime>
#include"Clock.h"
#include"Window.h"
#include"MainGameProject.h"
#include"GameContactListener.h"
class MainManager
{
public:
	MainManager();
	~MainManager();
private:
	void initSystem();
	void mainLoop();
	void drawEverything();

	MainGameProject*mainGameProject;
	GameContactListener*contactListener;
	b2World*world;
	SDL_Renderer*renderer;
	Window*window;
	Clock*clock;
};

#endif // !MAINMANAGER_H
