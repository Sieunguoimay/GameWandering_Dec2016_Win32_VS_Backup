#pragma once
#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H
#include"Button.h"
#include"GameCursor.h"
#include"SettingScreen.h"
class PauseScreen
{
public:
	PauseScreen(GameCursor*mouseCursor, Window*window, std::vector<Texture*>*textureContainer, SoundContainer*soundCollection);
	~PauseScreen();
	void handleEvent(SDL_Event&mainEvent);
	void display();
	bool selected()const;
	bool resume()const;
	bool backHome()const;
	bool setting()const;
private:
	Vector2D*position;
	Vector2D*velocity;
	Vector2D*acceleraion;

	std::vector<Texture*>*textureCollection;
	SoundContainer*soundCollection;
	Button*homeButton;
	Button*resumeButton;
	Button*settingButton;
	LinkedList<SettingScreen*>*settingScreen;

	Texture*background;
	Size*cameraSize;
	Window*window;
	GameCursor*mouseCursor;
};

#endif // !PAUSESCREEN_H
