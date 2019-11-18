#include "LevelManager.h"





LevelManager::LevelManager(Map * map, Sound*sound)
	:map(map),sound(sound)
{
	reachedDestination = false;
	soundPlayed = false;
	cleared = false;
}

LevelManager::~LevelManager()
{
	cleared = false;
	soundPlayed = false;
	reachedDestination = false;
	isItTimeToReset = false;
	texture = NULL;
	sound = NULL;
}

void LevelManager::reset()
{
	cleared = false;
	soundPlayed = false;
	reachedDestination = false;
	isItTimeToReset = false;
	//Mix_FadeOutChannel(-1,500);
}

void LevelManager::setup(Texture * texture)
{
	this->texture = texture;
}

void LevelManager::display(const Camera & camera)
{
	if (reachedDestination) {

		if (!soundPlayed) {
			sound->play();
			soundPlayed = true;
		}
		//delay for 1.5second
		static const float TIMEDELAY = 2.5;
		static float timeCounter = TIMEDELAY;
		timeCounter -= 1.0f / FPS;
		//after that
		if (timeCounter <= 0)
		{
			if (map->getCurrentMapLevel() == NUM_MAP_LEVEL-1)
			{
				cleared = true;
				return;
			}
			map->next();
			isItTimeToReset = true;
			timeCounter = TIMEDELAY;//set it again incase nexttime it will be 0
		}
		texture->setAlpha((int32)(((TIMEDELAY - timeCounter) / TIMEDELAY) * 255.0f));
		texture->render(camera, 0, 0, camera.getSize().w + 100, camera.getSize().h + 100);
	}
}

void LevelManager::setNextLevel()
{
	reachedDestination = true;
}

bool LevelManager::isCleared()
{
	return cleared;
}

bool LevelManager::resetSystemNow() const
{
	return (isItTimeToReset&&!cleared);
}

int LevelManager::getMapLevel() const
{
	return map->getCurrentMapLevel();
}
