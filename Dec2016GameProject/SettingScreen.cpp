#include "SettingScreen.h"



SettingScreen::SettingScreen(Window*window,GameCursor*mouseCursor,std::vector<Texture*>*textureCollection, SoundContainer*soundCollection)
	:cameraSize(window->getWindowSize()),window(window),mouseCursor(mouseCursor), textureCollection(textureCollection)
{
	chunkController = new SoundController((float)Mix_Volume(-1,-1), mouseCursor, textureCollection->at(30), 
		textureCollection->at(33), textureCollection->at(32),textureCollection->at(38),soundCollection);
	chunkController->setPosition(cameraSize->w*0.3f, cameraSize->h *0.45f);
	
	musicController = new SoundController((float)Mix_VolumeMusic(-1), mouseCursor, textureCollection->at(31), 
		textureCollection->at(33), textureCollection->at(32), textureCollection->at(38),soundCollection);
	musicController->setPosition(cameraSize->w*0.3f, cameraSize->h *0.65f);
	
	backButton = new Button(cameraSize->w*0.3f, cameraSize->h*0.25f, 80.0f,
		80.0f / textureCollection->at(29)->getSize()->w*textureCollection->at(29)->getSize()->h,
		textureCollection->at(29), soundCollection->at(4), soundCollection->at(7));

	fullScreenButtonOn = new Button(backButton->getRectangle()->x+2.0f*backButton->getRectangle()->w,
		cameraSize->h*0.25f, 80.0f, 80.0f,
		textureCollection->at(40), soundCollection->at(4), soundCollection->at(7));

	fullScreenButtonOff = new Button(backButton->getRectangle()->x + 2.0f*backButton->getRectangle()->w, 
		cameraSize->h*0.25f, 80.0f, 80.0f,
		textureCollection->at(41), soundCollection->at(4), soundCollection->at(7));
	fullScreenPointer = fullScreenButtonOff;
	backNow = false;
}


SettingScreen::~SettingScreen()
{
	delete chunkController;
	chunkController = NULL;
	delete musicController;
	musicController = NULL;
	delete backButton;
	backButton = NULL;
	delete fullScreenButtonOn;
	fullScreenButtonOn = NULL;
	delete fullScreenButtonOff;
	fullScreenButtonOff = NULL;
	fullScreenPointer = NULL;
	backNow = false;
}

void SettingScreen::handleEvent(SDL_Event&mainEvent)
{
	if (mainEvent.type == SDL_KEYDOWN&&mainEvent.key.keysym.sym == SDLK_ESCAPE)
		backNow = true;
	bool tempState = false;
	if (backButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)) {//if mouse over and
		backButton->readMouseState(mouseCursor->isButtonPressed());//pressed or not
		tempState = true;
	}else backButton->setButtonState(ButtonState::NORMAL);//mouse out side

	if (fullScreenPointer->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)) {//if mouse over and
		fullScreenPointer->readMouseState(mouseCursor->isButtonPressed());//pressed or not
		tempState = true;
	}
	else fullScreenPointer->setButtonState(ButtonState::NORMAL);//mouse out side


	if (chunkController->handleEvent(mainEvent))
		tempState = true;
	if (musicController->handleEvent(mainEvent))
		tempState = true;
	mouseCursor->coverButton(tempState);

	//update
	Mix_Volume(-1, (int)chunkController->getVolume());
	Mix_VolumeMusic((int)musicController->getVolume());
	if (fullScreenPointer->isPressed())
	{
		window->setFullScreen(!window->isFullScreen());
		fullScreenPointer->reset();
		fullScreenPointer = (fullScreenPointer == fullScreenButtonOn ? fullScreenButtonOff : fullScreenButtonOn);
	}
}

void SettingScreen::display()
{
	int x = 0;
	textureCollection->at(38)->setColor(215,154, 67);
	textureCollection->at(38)->render(x, x, (int)cameraSize->w, (int)cameraSize->h, 0);
	textureCollection->at(38)->setColor(0xff, 0xff, 0xff);

	chunkController->display();
	musicController->display();
	backButton->display();
	fullScreenPointer->display();

	textureCollection->at(39)->setColor(156, 106, 34);
	textureCollection->at(39)->render(x, x, (int)cameraSize->w, (int)cameraSize->h, 0);
	textureCollection->at(39)->setColor(0xff, 0xff, 0xff);

}

bool SettingScreen::back() const
{
	return (backNow||backButton->isPressed());
}
