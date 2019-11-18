#include "MainManager.h"



MainManager::MainManager()
{
	initSystem();
	mainLoop();
}


MainManager::~MainManager()
{
	delete mainGameProject;
	mainGameProject = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	delete window;
	window = NULL;
	delete clock;
	clock = NULL;


	delete world;
	world = NULL;
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void MainManager::initSystem()
{
	//initialize SDL system
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
	}//SDL_Log("Error to Init SDL: %s", SDL_GetError());

	IMG_InitFlags flag = IMG_INIT_PNG;
	if (!(IMG_Init(flag) && flag))
	{
	}//SDL_Log("Error to Init MIX_image: %s", IMG_GetError());

	MIX_InitFlags flags = MIX_INIT_MP3;
	if (Mix_Init(flags) && flags != flags)
	{
	}//SDL_Log("Error to Init MIX_image: %s", Mix_GetError());
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)//very important
	{
	}//SDL_Log("Mix_OpenAudio: %s\n", Mix_GetError());
	
	
	srand((unsigned int)time(0));
	//windowSize = new Size(500, 300);
	window = new Window("Last 2016 Game Project", new Size(1366, 768), &renderer);
	window->setFullScreen(true);
	SDL_ShowCursor(SDL_DISABLE);
	Mix_AllocateChannels(20);
	//SDL_Log("Allocate %d sound channels.\n", Mix_AllocateChannels(20));
	clock = new Clock(FPS);//30-FPS

	//initialize Box2D world
	world = new b2World(b2Vec2(0.0f, GRAVITY));
	//initialize game
	mainGameProject = new MainGameProject(&renderer, window, world);
	//mainGameProject = new GameExperiment(&renderer,windowSize,world);
	mainGameProject->setup();

	contactListener = new GameContactListener();
	world->SetContactListener(contactListener);
}

void MainManager::mainLoop()
{
	bool runMainLoop = true;
	SDL_Event mainEvent;
	while (runMainLoop) {
		while (SDL_PollEvent(&mainEvent)) {
			window->handleEvent(mainEvent, renderer);
			mainGameProject->handleEvent(mainEvent);
			if (mainGameProject->exitGame() || mainEvent.type == SDL_QUIT)
				runMainLoop = false;
		}
		drawEverything();
		clock->work();
		//keep the box2d world running
		world->Step(1.0f / FPS, 8,3);
		contactListener->step();
	}
	window->setFullScreen(false);
}

void MainManager::drawEverything()
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);
	//draw everything here
	mainGameProject->draw();
	SDL_RenderPresent(renderer);
}
