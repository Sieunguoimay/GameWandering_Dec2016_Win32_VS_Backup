#include "SoundContainer.h"



SoundContainer::SoundContainer()
{
	soundCollection = new std::vector<Sound*>();
	loadSoundFromFile();
}


SoundContainer::~SoundContainer()
{
	for (size_t i = 0; i < soundCollection->size(); i++)
	{
		delete soundCollection->at(i);
		soundCollection->at(i) = NULL;
	}
	delete soundCollection;
	soundCollection = NULL;
}

Sound * SoundContainer::at(int index)
{
	return soundCollection->at(index);
}

void SoundContainer::playMusicLoopForever()
{
	Mix_PlayMusic(music, -1);
}


void SoundContainer::loadSoundFromFile()
{
	music = Mix_LoadMUS("sounds/mainmusic.mp3");
	soundCollection->push_back(new Sound("sounds/explosion.wav"));//0
	soundCollection->push_back(new Sound("sounds/lightning.wav"));//1
	soundCollection->push_back(new Sound("sounds/laser.wav"));//2
	soundCollection->push_back(new Sound("sounds/shoot.wav"));//3
	soundCollection->push_back(new Sound("sounds/buttonclick.wav"));//4
	soundCollection->push_back(new Sound("sounds/reset.wav"));//5
	soundCollection->push_back(new Sound("sounds/fairybell.wav"));//6
	soundCollection->push_back(new Sound("sounds/buttonhover.wav"));//7
	soundCollection->push_back(new Sound("sounds/jump.wav"));//8
	soundCollection->push_back(new Sound("sounds/monster.wav"));//9
	soundCollection->push_back(new Sound("sounds/beepop.wav"));//10
	soundCollection->push_back(new Sound("sounds/beezz.wav"));//11
	soundCollection->push_back(new Sound("sounds/beewhoosh.wav"));//12
	soundCollection->push_back(new Sound("sounds/hideposter.wav"));//13
}
