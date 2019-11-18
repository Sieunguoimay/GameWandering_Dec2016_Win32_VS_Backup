#pragma once
#ifndef SOUNDCLOUD_H
#define SOUNDCLOUD_H
#include"Sound.h"
#include"globalheader.h"
class SoundContainer
{
public:
	SoundContainer();
	~SoundContainer();
	Sound*at(int index);
	void playMusicLoopForever();
	//to play music we need connect to the music source
	//but to stop it, we can do anywhere
private:
	void loadSoundFromFile();
	std::vector<Sound*>*soundCollection;
	Mix_Music*music;
};

#endif // !SOUNDCLOUD_H
