#include "FingerManager.h"



FingerManager::FingerManager()
{
	fingers = new Finger[4];
}


FingerManager::~FingerManager()
{
	delete []fingers;
}

void FingerManager::handleEvent(SDL_Event & mainEvent)
{
	int fingerId = 0;
	if (mainEvent.type == SDL_FINGERDOWN|| mainEvent.type == SDL_FINGERMOTION) {
		fingerId = mainEvent.tfinger.fingerId;
		if (fingerId < 4) {
			fingers[fingerId].set(mainEvent.tfinger.x, mainEvent.tfinger.y,true);
		}
	}
	if (mainEvent.type == SDL_FINGERUP) {
		fingerId = mainEvent.tfinger.fingerId;
		if (fingerId < 4) {
			fingers[fingerId].set(mainEvent.tfinger.x, mainEvent.tfinger.y, false);
		}
	}
}

Finger * FingerManager::getFinger()
{
	return fingers;
}
