#pragma once
#ifndef SETTINGSCREEN_H
#define SETTINGSCREEN_H
#include"SoundContainer.h"
#include"SoundController.h"
#include"SDL_mixer.h"
#include"Window.h"
class SettingScreen
{
public:
	SettingScreen(Window*window,GameCursor*mouseCursor,std::vector<Texture*>*textureCollection, SoundContainer*soundCollection);
	~SettingScreen();
	void handleEvent(SDL_Event&mainEvent);
	void display();
	bool back()const;

private:
	std::vector<Texture*>*textureCollection;
	SoundController*chunkController;
	SoundController*musicController;
	Button*backButton;
	
	Button*fullScreenButtonOn;
	Button*fullScreenButtonOff;
	Button*fullScreenPointer;
	
	Size*cameraSize;
	GameCursor*mouseCursor;
	bool backNow;
	Window*window;
};

#endif // !SETTINGSCREEN_H
