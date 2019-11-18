#include "MenuManager.h"



MenuManager::MenuManager(Map*map, Window*window, GameCursor*mouseCursor, std::vector<Texture*>*textureCollection, SoundContainer*soundCollection)
	:map(map),cameraSize(window->getWindowSize()),window(window),mouseCursor(mouseCursor),soundCollection(soundCollection),
	buttonTexture(textureCollection->at(21)),levelNumTexture(textureCollection->at(22)),
	background(textureCollection->at(23)),textureCollection(textureCollection)
{
	buttons = new std::vector<ButtonDefault*>();

	exitButton = new Button(cameraSize->w*0.9f, cameraSize->h*0.15f, 60.0f, 60.0f,
		textureCollection->at(24),soundCollection->at(4), soundCollection->at(7));
	settingButton = new Button(exitButton->getRectangle()->x, 
		exitButton->getRectangle()->y+ exitButton->getRectangle()->w*1.5f,
		exitButton->getRectangle()->w, exitButton->getRectangle()->h, 
		textureCollection->at(28), soundCollection->at(4), soundCollection->at(7));
	settingScreen = new LinkedList<SettingScreen*>();
	soundCollection->playMusicLoopForever();
	mouseCursor->setPosition(cameraSize->w *0.5f, cameraSize->h *0.5f);
}


MenuManager::~MenuManager()
{
	Mix_HaltMusic();
	for (size_t i = 0; i < buttons->size(); i++)
	{
		delete buttons->at(i);
		buttons->at(i) = NULL;
	}
	delete buttons;
	buttons = NULL;
	delete exitButton;
	exitButton = NULL;
	delete settingButton;
	settingButton = NULL;
	for (Node<SettingScreen*>*iter = settingScreen->getFirstNode(); iter != settingScreen->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete settingScreen;
	settingScreen = NULL;
	soundCollection = NULL;

}

void MenuManager::setup()
{
	selected = false;
	float buttonSize = 100.0f*cameraSize->w/WINDOWSIZE_W;
	int numberButton = NUM_MAP_LEVEL;
	float buttonDistance = cameraSize->w / numberButton / 2;
	float x = cameraSize->w / numberButton+buttonDistance/2;
	float y = cameraSize->h * 2.0f/ 3.0f;
	for (int i = 0; i < numberButton; i++) {
		if (i < map->getMapLevelSaved()+1)
			buttons->push_back(new Button(x + i*buttonDistance, y, 
				buttonSize, buttonSize, 
				buttonTexture,soundCollection->at(4), soundCollection->at(7)));
		else 
			buttons->push_back(new ButtonDefault(x + i*buttonDistance, y, 
				buttonSize, buttonSize, buttonTexture));
	}
}

void MenuManager::display()
{
	for (Node<SettingScreen*>*iter = settingScreen->getFirstNode(); iter != settingScreen->tail; iter = iter->next)
	{
		iter->data->display();
		mouseCursor->display();
		if (iter->data->back())
		{
			delete iter->data;
			iter->data = NULL;
			iter = settingScreen->delNode(iter);
			settingButton->reset();
		}
		return;
	}
	int x = 0;
	background->render(x, x,(int)cameraSize->w, (int)cameraSize->h,0);
	for (size_t i = 0; i < buttons->size(); i++) {
		buttons->at(i)->display();
		if (buttons->at(i)->isPressed())
		{
			map->setMapLevel(i);
			selected = true;
		}
		levelNumTexture->render(
			(int)(buttons->at(i)->getRectangle()->x - buttons->at(i)->getRectangle()->w / 4), 
			(int)(buttons->at(i)->getRectangle()->y + buttons->at(i)->getRectangle()->h*0.5), 
			(int)(buttons->at(i)->getRectangle()->w/2),
			(int)( buttons->at(i)->getRectangle()->h/2),i);
	}
	exitButton->display();
	settingButton->display();
	mouseCursor->display();
}

void MenuManager::handleEvent(SDL_Event & mainEvent)
{
	for (Node<SettingScreen*>*iter = settingScreen->getFirstNode(); iter != settingScreen->tail; iter = iter->next)
	{
		iter->data->handleEvent(mainEvent);
		mouseCursor->handleEvent(mainEvent);
		return;
	}
	bool tempState = false;
	for (size_t i = 0; i < buttons->size(); i++)
		if (buttons->at(i)->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y))
			buttons->at(i)->handleEvent(mainEvent);
		else buttons->at(i)->setButtonState(ButtonState::NORMAL);
	////
	if (exitButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)){
		exitButton->handleEvent(mainEvent);
		tempState = true;
	}else exitButton->setButtonState(ButtonState::NORMAL);

	if (settingButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)){
		settingButton->handleEvent(mainEvent);
		tempState = true;
	}else settingButton->setButtonState(ButtonState::NORMAL);
	////
	mouseCursor->coverButton(tempState);
	mouseCursor->handleEvent(mainEvent);


	//setting button pressed
	if (settingButton->isPressed()) {
		SettingScreen*newSettingScreen = new SettingScreen(window, mouseCursor, textureCollection,soundCollection);
		if (newSettingScreen)
			settingScreen->addFirst(newSettingScreen);
	}
}

bool MenuManager::hasSelected() const
{
	return selected;
}

bool MenuManager::exitGameNow() const
{
	return exitButton->isPressed();
}
