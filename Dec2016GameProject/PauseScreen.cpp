#include "PauseScreen.h"



PauseScreen::PauseScreen(GameCursor*mouseCursor, Window*window, std::vector<Texture*>*textureContainer, SoundContainer*soundCollection)
	:mouseCursor(mouseCursor),window(window),cameraSize(window->getWindowSize()),textureCollection(textureContainer),soundCollection(soundCollection)
{
	//for the appearance effect
	position = new Vector2D(-cameraSize->w/2, 0);
	velocity = new Vector2D(50, 0);

	float x = position->x+cameraSize->w/2;
	float y = position->y+cameraSize->h/2;
	float w = cameraSize->w / 10;
	float h = cameraSize->w / 10;

	homeButton = new Button(x-w*1.5f, y, w, h, textureContainer->at(26),soundCollection->at(4), soundCollection->at(7));
	resumeButton = new Button(x, y, w, h, textureContainer->at(27), soundCollection->at(4), soundCollection->at(7));
	settingButton = new Button(x + w*1.5f, y, w, h, textureContainer->at(28), soundCollection->at(4), soundCollection->at(7));
	background = textureContainer->at(38);
	settingScreen = new LinkedList<SettingScreen*>();
}


PauseScreen::~PauseScreen()
{
	for (Node<SettingScreen*>*iter = settingScreen->getFirstNode(); iter != settingScreen->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete settingScreen;
	settingScreen = NULL;
	delete homeButton;
	homeButton = NULL;
	delete resumeButton;
	resumeButton = NULL;
	delete settingButton;
	settingButton = NULL;

	delete position;
	position = NULL;
	delete velocity;
	velocity = NULL;
	delete acceleraion;
	acceleraion = NULL;
}

void PauseScreen::handleEvent(SDL_Event & mainEvent)
{
	for (Node<SettingScreen*>*iter = settingScreen->getFirstNode(); iter != settingScreen->tail; iter = iter->next)
		iter->data->handleEvent(mainEvent);

	if (settingScreen->isEmpty()) {
		bool tempState = false;
		if (homeButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)){
			homeButton->handleEvent(mainEvent);
			tempState = true;
		} else homeButton->setButtonState(ButtonState::NORMAL);

		if (resumeButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)){
			resumeButton->handleEvent(mainEvent);
			tempState = true;
		} else resumeButton->setButtonState(ButtonState::NORMAL);

		if (settingButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)){
			settingButton->handleEvent(mainEvent);
			tempState = true;
		}else settingButton->setButtonState(ButtonState::NORMAL);
		mouseCursor->coverButton(tempState);
	}
	mouseCursor->handleEvent(mainEvent);
}

void PauseScreen::display()
{
	background->setAlpha(100);
	background->setColor(255, 255, 255);
	background->render((int)(position->x), (int)position->y, (int)cameraSize->w, (int)cameraSize->h,0);
	background->setAlpha(255);

	if (position->x < 0) {
		if (velocity->x > 5.0f)
			velocity->set(-position->x*0.1f);
		homeButton->setPosition(
			homeButton->getRectangle()->x + velocity->x,
			homeButton->getRectangle()->y + velocity->y);
		resumeButton->setPosition(
			resumeButton->getRectangle()->x + velocity->x,
			resumeButton->getRectangle()->y + velocity->y);
		settingButton->setPosition(
			settingButton->getRectangle()->x + velocity->x,
			settingButton->getRectangle()->y + velocity->y);
		position->add(velocity);
	}

	if (settingButton->isPressed()) {
		if (settingScreen->isEmpty())
			settingScreen->addFirst(new SettingScreen(window, mouseCursor, textureCollection,soundCollection));
		settingButton->reset();
	}
	for (Node<SettingScreen*>*iter = settingScreen->getFirstNode(); iter != settingScreen->tail; iter = iter->next)
	{
		iter->data->display();
		if (iter->data->back()) {
			delete iter->data;
			iter->data = NULL;
			iter = settingScreen->delNode(iter);
		}
	}
	if (settingScreen->isEmpty()) {
		homeButton->display();
		resumeButton->display();
		settingButton->display();
	}
	mouseCursor->display();
}

bool PauseScreen::selected() const
{
	return (homeButton->isPressed()||resumeButton->isPressed());
}

bool PauseScreen::resume()const
{
	return resumeButton->isPressed();
}

bool PauseScreen::backHome()const
{
	return homeButton->isPressed();
}

bool PauseScreen::setting()const
{
	return settingButton->isPressed();
}
