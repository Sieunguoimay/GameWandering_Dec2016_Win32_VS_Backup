#include "SoundController.h"



SoundController::SoundController(float volume,GameCursor*mouseCursor, Texture*buttonTexture, 
	Texture*movingButtonTexture, Texture*frameTexture, Texture*dynamicTexture, SoundContainer*soundCollection)
	:volume(volume),mouseCursor(mouseCursor),dynamicTexture(dynamicTexture)
{
	button = new Button(0, 0, 0, 0, buttonTexture,soundCollection->at(4),soundCollection->at(7));
	movingButton = new Button(0, 0, 0,0, movingButtonTexture, soundCollection->at(4), soundCollection->at(7));
	frameButton = new Button(0, 0, 0, 0, frameTexture, soundCollection->at(4), soundCollection->at(7));
	position = new Vector2D();
}


SoundController::~SoundController()
{
	delete position;
	position = NULL;
	delete button;
	button = NULL;
	delete movingButton;
	movingButton = NULL;
	delete frameButton;
	frameButton = NULL;
	dynamicTexture = NULL;
	mouseCursor = NULL;
}

bool SoundController::handleEvent(SDL_Event&mainEvent)
{
	float head = 
		frameButton->getRectangle()->x 
		- frameButton->getRectangle()->w / 2.0f 
		+ movingButton->getRectangle()->w / 2;
	float tail = head + frameButton->getRectangle()->w - movingButton->getRectangle()->w;

	bool tempState = false;
	if (button->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)) {
		button->handleEvent(mainEvent);
		tempState = true;
	}
	else button->setButtonState(ButtonState::NORMAL);
	
	if (frameButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)) {
		if (mainEvent.type == SDL_MOUSEBUTTONDOWN&&mainEvent.button.button == SDL_BUTTON_LEFT) {
			if (mouseCursor->getPosition()->x > head&&mouseCursor->getPosition()->x < tail)
			{
				movingButton->getRectangle()->x = mouseCursor->getPosition()->x;
				volume = UtilityFunctions::map(
					movingButton->getRectangle()->x - head,
					0, tail - head,
					0, SDL_MIX_MAXVOLUME);
			}
		}
	}
	else frameButton->setButtonState(ButtonState::NORMAL);


	if (movingButton->checkInside(mouseCursor->getPosition()->x, mouseCursor->getPosition()->y)) {
		tempState = true;
		movingButton->handleEvent(mainEvent);
	}
	else movingButton->setButtonState(ButtonState::NORMAL);
	if (mainEvent.type == SDL_MOUSEBUTTONUP)
		movingButton->reset();

	//////	
	if (movingButton->isPressed()) {
		if (mouseCursor->getPosition()->x <= head)
			movingButton->getRectangle()->x = head;
		else if (mouseCursor->getPosition()->x >= tail)
			movingButton->getRectangle()->x = tail;
		else 
			movingButton->getRectangle()->x = mouseCursor->getPosition()->x;
		volume = UtilityFunctions::map(movingButton->getRectangle()->x - head,
			0, tail - head,
			0,SDL_MIX_MAXVOLUME);
	}
	return tempState;
}

void SoundController::display()
{
	dynamicTexture->setColor(255, 150, 0);
	dynamicTexture->render(
		(int)(frameButton->getRectangle()->x - frameButton->getRectangle()->w *0.5f
			+ movingButton->getRectangle()->w *0.5f),
		(int)(frameButton->getRectangle()->y - frameButton->getRectangle()->h*0.25f),
		(int)UtilityFunctions::map(volume,
			0, SDL_MIX_MAXVOLUME, 
			0, frameButton->getRectangle()->w - movingButton->getRectangle()->w),
		(int)(frameButton->getRectangle()->h*0.5f), 0);
	dynamicTexture->setColor(0xff,0xff,0xff);
	
	frameButton->display();
	button->display();
	movingButton->display();


}

void SoundController::setPosition(float x, float y)
{
	position->set(x, y);
	setup(position->x, position->y,80.0f);
}

Vector2D * SoundController::getPosition()
{
	return position;
}

float SoundController::getVolume() const
{
	return volume;
}

void SoundController::setup(float x, float y, float size)
{
	button->getRectangle()->set(x, y, size/button->getTextureSize()->h*button->getTextureSize()->w, size);

	frameButton->getRectangle()->set(x + size*2.5f, y, 
		size/ frameButton->getTextureSize()->h*frameButton->getTextureSize()->w, size);
	
	float movingButtionHeight = size*1.5f;
	float movingButtionWidth =
		movingButtionHeight / movingButton->getTextureSize()->h*movingButton->getTextureSize()->w;

	movingButton->getRectangle()->set(
		frameButton->getRectangle()->x - frameButton->getRectangle()->w/2
		+ movingButtionWidth*0.5f+
		UtilityFunctions::map(volume,0,SDL_MIX_MAXVOLUME, 0, frameButton->getRectangle()->w - movingButtionWidth),
		y+size/2-movingButtionHeight/3,
		movingButtionWidth, movingButtionHeight);
}
