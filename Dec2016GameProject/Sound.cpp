#include "Sound.h"



Sound::Sound(std::string path)
{
	loadFromFile(path);
}


Sound::~Sound()
{
	//delete sound;
	Mix_FreeChunk(sound);
	sound = NULL;//to be safe
}

void Sound::play(int channel)
{
	if (Mix_PlayChannel(channel, sound, 0) == -1) {
		//SDL_Log("Mix_PlayChannel: %s\n", Mix_GetError());
		// may be critical error, or maybe just no channels were free.
		// you could allocated another channel in that case...
	}
}

void Sound::loadFromFile(std::string path)
{
	sound = Mix_LoadWAV(path.c_str());
	//SDL_Log("Loaded sound: %s\n", path);
	//if(sound==NULL)
		//SDL_Log("Error to Load sound: %s\n", Mix_GetError());
}
