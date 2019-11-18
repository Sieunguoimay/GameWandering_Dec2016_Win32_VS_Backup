#include "MainGameProject.h"



MainGameProject::MainGameProject(SDL_Renderer**renderer, Window*window, b2World*world)
	:window(window),renderer(renderer),world(world)
{
	
	camera = new Camera(window->getWindowSize());
	shape = new Shape(world);
	background = new Background(camera);
	moverContainer = new LinkedList<Mover*>();
	ecosystemContainer = new LinkedList<Ecosystem*>();
	menuManager = new LinkedList<MenuManager*>();
	pauseScreen = new LinkedList<PauseScreen*>();
	poster = new LinkedList<Poster*>();
	soundCollection = new SoundContainer();
	youWin = new LinkedList<YouWin*>();
}


MainGameProject::~MainGameProject()
{
	delete background;
	background = NULL;
	delete camera;
	camera = NULL;
	delete shape;
	shape = NULL;
	delete levelManager;
	levelManager = NULL;

	for (Node<Mover*>*iter = moverContainer->getFirstNode(); iter != moverContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete moverContainer;
	moverContainer = NULL;

	for (Node<Ecosystem*>*iter = ecosystemContainer->getFirstNode(); iter != ecosystemContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete ecosystemContainer;
	ecosystemContainer = NULL;

	for (Node<MenuManager*>*iter = menuManager->getFirstNode(); iter != menuManager->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete menuManager;
	menuManager = NULL;

	for (Node<PauseScreen*>*iter = pauseScreen->getFirstNode(); iter != pauseScreen->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete pauseScreen;
	pauseScreen = NULL;
	for (Node<Poster*>*iter = poster->getFirstNode(); iter != poster->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete poster;
	poster = NULL;
	for (Node<YouWin*>*iter = youWin->getFirstNode(); iter != youWin->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete youWin;
	youWin = NULL;
	delete soundCollection;
	soundCollection = NULL;
	gameCleared = false;
}
//because b2Vec2 is just vector of float
//hence when it's in Box2D world, it's meter unit
//but in pixels world it's just float

//the x,y,w,h we must convert when exchange between 2 worlds because
//It's actualy in the real - world
void MainGameProject::handleEvent(SDL_Event & mainEvent)
{
	mainEvent.tfinger;
		//shape->addRectangle(mainEvent.motion.x, mainEvent.motion.y, 20, 20, true);
		//shape->addCircle(mainEvent.motion.x, mainEvent.motion.y, 40, true);
	for (Node<Poster*>*iter = poster->getFirstNode(); iter != poster->tail; iter = iter->next)
	{
		iter->data->handleEvent(mainEvent);
		return;
	}
	for (Node<MenuManager*>*iter = menuManager->getFirstNode(); iter != menuManager->tail; iter = iter->next)
	{
		iter->data->handleEvent(mainEvent);
	}
	for (Node<PauseScreen*>*iter = pauseScreen->getFirstNode(); iter != pauseScreen->tail; iter = iter->next)
		iter->data->handleEvent(mainEvent);
	
		if (menuManager->isEmpty())
		{
			if (pauseScreen->isEmpty())
				for (Node<Mover*>*iter = moverContainer->getFirstNode(); iter != moverContainer->tail; iter = iter->next) {
					iter->data->handleEvent(mainEvent);
				}
			if (mainEvent.type == SDL_KEYDOWN) {
				if (mainEvent.key.keysym.sym == SDLK_ESCAPE) {
					if (pauseScreen->isEmpty())
						pauseScreen->addFirst(new PauseScreen(mouseCursor, window, textureColection, soundCollection));
				}
			}
		}

}

void MainGameProject::setup()
{
	gameCleared = false;
	camera->setup();
	
	Mover*newMover = new Mover(world);
	newMover->setup(renderer,soundCollection);
	moverContainer->addFirst(newMover);

	Ecosystem*newEcosystem = new Ecosystem(world,newMover,
		&levelManager,&textureColection, soundCollection);
	newEcosystem->setup(renderer);
	ecosystemContainer->addFirst(newEcosystem);
	
	background->setup(textureColection);

	/////////////////////////////////////////////////////
	mouseCursor = new GameCursor(textureColection->at(25));
	mouseCursor->setPosition(window->getWindowSize()->w / 2.0f, window->getWindowSize()->h / 2.0f);
	MenuManager*newMenuManager = new MenuManager(ecosystemContainer->getFirstNode()->data->getMapPointer(), window,
		mouseCursor,
		textureColection,soundCollection); 
	newMenuManager->setup();
	menuManager->addFirst(newMenuManager);

	Poster*newPoster = new Poster(textureColection->at(34), textureColection->at(35),mouseCursor,soundCollection);
	poster->addFirst(newPoster);
}

void MainGameProject::draw()
{
	static bool resetSystem = false;
	//shape->display(renderer);//need to be deleted as soon as possible
	for (Node<MenuManager*>*iter = menuManager->getFirstNode(); iter != menuManager->tail; iter = iter->next)
	{
		iter->data->display();
		if (iter->data->hasSelected()) {
			delete iter->data;
			iter->data = NULL;
			iter = menuManager->delNode(iter);
			resetSystem = true;
		}

		for (Node<Poster*>*iter = poster->getFirstNode(); iter != poster->tail; iter = iter->next)
		{
			iter->data->display(*camera);
			if (iter->data->hasDone()) {
				delete iter->data;
				iter->data = NULL;
				iter = poster->delNode(iter);
			}
		}

		return;
	}
	
	camera->update();//when 3will it update?? Mover??
	background->display();
	for (Node<Ecosystem*>*iter = ecosystemContainer->getFirstNode(); iter != ecosystemContainer->tail; iter = iter->next)
		iter->data->display(*camera);
	for (Node<Mover*>*iter = moverContainer->getFirstNode(); iter != moverContainer->tail; iter = iter->next)
	{
		iter->data->display(*camera);
		if (iter->data->hasExploded())
			resetSystem = true;
	}
	//////////////////////////////
	//display 2
	for (Node<Ecosystem*>*iter = ecosystemContainer->getFirstNode(); iter != ecosystemContainer->tail; iter = iter->next)
		iter->data->display2(*camera);
	background->display2();

	///////////////////////////////
	for (Node<PauseScreen*>*iter = pauseScreen->getFirstNode(); iter != pauseScreen->tail; iter = iter->next) {
		iter->data->display();
		if (iter->data->backHome()) 
			home();
		
		if (iter->data->selected()) {
			delete iter->data;
			iter->data = NULL;
			iter = pauseScreen->delNode(iter);
		}
			
	}
	///////////////////////////////////////////////////////////
	levelManager->display(*camera);
	if (levelManager->isCleared())
	{
		if (gameCleared == false)
			youWin->addFirst(new YouWin(textureColection, soundCollection, window->getWindowSize()));
		gameCleared = true;
	}	//home();
	for (Node<YouWin*>*iter = youWin->getFirstNode(); iter != youWin->tail; iter = iter->next)
	{
		iter->data->dislplay(*camera);
		if (iter->data->done()) {
			delete iter->data;
			iter->data = NULL;
			iter = youWin->delNode(iter);
			home();
		}
	}
	if (resetSystem||levelManager->resetSystemNow()) {
		reset();
		resetSystem = false;
		soundCollection->at(5)->play();
	}
	
}

bool MainGameProject::exitGame() const
{
	for (Node<MenuManager*>*iter = menuManager->getFirstNode(); iter != menuManager->tail; iter = iter->next)
		return iter->data->exitGameNow();
	return false;
}

void MainGameProject::reset()
{
	
	for (Node<Mover*>*iter = moverContainer->getFirstNode(); iter != moverContainer->tail; iter = iter->next)
		iter->data->reset();
	for (Node<Ecosystem*>*iter = ecosystemContainer->getFirstNode(); iter != ecosystemContainer->tail; iter = iter->next)
		iter->data->reset();

	background->reset(levelManager->getMapLevel());
	camera->reset();
	levelManager->reset();
	gameCleared = false;
}

void MainGameProject::home()
{
	MenuManager*newMenuManager = new MenuManager(ecosystemContainer->getFirstNode()->data->getMapPointer(), window,
		mouseCursor, textureColection, soundCollection);
	newMenuManager->setup();
	menuManager->addFirst(newMenuManager);
}

	