#pragma once
#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include"Map.h"
#include"Camera.h"
#include"Button.h"
#include"GameCursor.h"
#include"SoundContainer.h"
#include"SettingScreen.h"
class MenuManager
{
public:
	MenuManager(Map*map, Window*window, GameCursor*mouseCursor,std::vector<Texture*>*textureColection,SoundContainer*soundCollecition);
	~MenuManager();
	void setup();
	void display();
	void handleEvent(SDL_Event&mainEvent);
	bool hasSelected()const;
	bool exitGameNow()const;
private:
	std::vector<ButtonDefault*>*buttons;
	std::vector<Texture*>*textureCollection;

	Button*exitButton;
	Button*settingButton;
	LinkedList<SettingScreen*>*settingScreen;
	Map*map;
	Size*cameraSize;
	GameCursor*mouseCursor;

	Texture*buttonTexture;
	Texture*levelNumTexture;
	Texture*background;
	Window*window;
	SoundContainer*soundCollection;
	bool selected;
};

#endif // !MENUMANAGER_H
