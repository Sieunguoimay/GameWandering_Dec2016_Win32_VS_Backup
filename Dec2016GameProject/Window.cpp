#include "Window.h"



Window::Window(std::string path,Size*windowSize,SDL_Renderer**renderer)
	:windowSize(windowSize)
{
	this->window = SDL_CreateWindow(path.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		(int)windowSize->w, (int)windowSize->h, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
	*renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0x00);
	SDL_RenderClear(*renderer);
}


Window::~Window()
{
	this->minimized = false;
	this->fullScreen = false;
	SDL_DestroyWindow(window);
	window = NULL;
}

bool Window::handleEvent(SDL_Event & mainEvent, SDL_Renderer * renderer)
{
	if (mainEvent.type == SDL_WINDOWEVENT) {
		switch (mainEvent.window.event)
		{
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			windowSize->w = (float)mainEvent.window.data1;
			windowSize->h = (float)mainEvent.window.data2;
			SDL_RenderPresent(renderer);//by the time window changes size
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_RenderPresent(renderer);
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			this->minimized = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			this->minimized = false;
			break;
		case SDL_WINDOWEVENT_RESTORED:
			this->minimized = false;
			break;
		default:
			break;
		}
	}
	else if (mainEvent.type == SDL_KEYDOWN ) {
		/*if (mainEvent.key.keysym.sym == SDLK_RETURN) {
			if (fullScreen) {
				SDL_SetWindowFullscreen(this->window, SDL_FALSE);
				fullScreen = false;
			}
			else {
				SDL_SetWindowFullscreen(this->window, SDL_TRUE);
				fullScreen = true;
				minimized = false;
			}
		}
		else if (mainEvent.key.keysym.sym == SDLK_ESCAPE) {
			return false;
		}*/
	}
	else if (mainEvent.type == SDL_QUIT)
		return false;
	return true;
}

bool Window::isMinimized() const
{
	return this->minimized;
}

void Window::setFullScreen(bool flag)
{
	SDL_SetWindowFullscreen(this->window, flag);
	if (flag) {
		fullScreen = true;
		minimized = false;
	}
	else {
		fullScreen = false;
	}
}

Size * Window::getWindowSize()
{
	return windowSize;
}

bool Window::isFullScreen() const
{
	return fullScreen;
}


