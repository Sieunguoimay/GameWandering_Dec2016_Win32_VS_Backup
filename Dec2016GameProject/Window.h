#pragma once

#ifndef WINDOW_H
#define WINDOW_H
#include<SDL.h>
#include"GlobalHeader.h"
class Window
{
public:
	Window(std::string path = "NULL",Size*windowSize = NULL,SDL_Renderer**renderer = NULL);
	~Window();
	bool handleEvent(SDL_Event&mainEvent, SDL_Renderer*renderer);
	//this method to get the minimized state 
	//will helpful for drawing or not
	bool isMinimized() const;
	void setFullScreen(bool flag);
	Size*getWindowSize();
	bool isFullScreen()const;
private:
	bool minimized;
	bool fullScreen;
	SDL_Window*window;
	Size*windowSize;
};

#endif // !WINDOW_H
