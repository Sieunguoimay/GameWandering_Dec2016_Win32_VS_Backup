#pragma once
#include"MainManager.h"

int main(int argv, char**argc) {
	//SDL_Log("Hello last game Project of 2016.\n");
	MainManager*mainManager = new MainManager();
	delete mainManager;
	return 0;
}