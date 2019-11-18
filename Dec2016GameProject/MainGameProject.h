#pragma once

#ifndef MAINGAMEPROJECT_H
#define MAINGAMEPROJECT_H
#include"GlobalHeader.h"
#include"Camera.h"
#include"Texture.h"
#include"Shape.h"
#include"Mover.h"
#include"Ecosystem.h"
#include"Background.h"
#include"LevelManager.h"
#include"MenuManager.h"
#include"GameCursor.h"
#include"PauseScreen.h"
#include"SoundContainer.h"
#include"Poster.h"
#include"YouWin.h"
class MainGameProject
{
public:
	MainGameProject(SDL_Renderer**renderer,Window*window,b2World*world);
	~MainGameProject();
	virtual void handleEvent(SDL_Event&mainEvent);
	virtual void setup();
	virtual void draw();
	bool exitGame()const;
private:
	void reset();
	void home();
	bool gameCleared;
	//=================>
	//every objects should be store in a single linkedlist is beter :))
	//=================>
	LevelManager*levelManager;
	Background*background;
	Shape*shape;
	LinkedList<ObjectData*>*objectsContainer;
	LinkedList<Mover*>*moverContainer;
	LinkedList<Ecosystem*>*ecosystemContainer;
	LinkedList<YouWin*>*youWin;
	///////////////////////////////////////
	LinkedList<MenuManager*>*menuManager;
	LinkedList<PauseScreen*>*pauseScreen;
	LinkedList<Poster*>*poster;
	GameCursor*mouseCursor;
	std::vector<Texture*>*textureColection;
	SoundContainer*soundCollection;
protected:
	//pointer to the environment
	b2World*world;
	SDL_Renderer**renderer;
	Window*window;
	Camera*camera;
};

#endif // !MAINGAMEPROJECT_H
