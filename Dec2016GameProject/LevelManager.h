#pragma once
#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H	
#include"globalheader.h"
#include"Map.h"
#include"Texture.h"
#include"Camera.h"
#include"Sound.h"
class LevelManager
{
public:
	LevelManager(Map*map, Sound*sound);
	~LevelManager();
	void reset();
	void setup(Texture*texture);
	void display(const Camera&camera);

	void setNextLevel();
	bool isCleared();
	bool resetSystemNow()const;
	int getMapLevel()const;
private:
	bool isItTimeToReset;
	bool reachedDestination;
	bool soundPlayed;
	bool cleared;
	Map*map;
	Texture*texture;
	Sound*sound;
};

#endif // !LEVELMANAGER_H
