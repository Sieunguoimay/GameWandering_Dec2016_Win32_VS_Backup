#pragma once
#ifndef SOUND_H
#define SOUND_H
#include<SDL.h>
#include<SDL_mixer.h>
#include<string>
class Sound
{
public:
	Sound(std::string path);
	~Sound();
	void play(int channel = -1);
private:
	void loadFromFile(std::string path);
	Mix_Chunk*sound;
};
//int Sound::channel = 0;
#endif // !SOUND_H
