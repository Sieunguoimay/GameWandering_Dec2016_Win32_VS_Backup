#pragma once
#ifndef YOUWIND_H
#define YOUWIND_H
#include"globalheader.h"
#include"Explosion.h"
#include"Monster_Tomato.h"
#include"SoundContainer.h"
class FireWork;
class YouWin
{
public:
	YouWin(std::vector<Texture*>*textureCollection, SoundContainer*soundCollection, Size*cameraSize);
	~YouWin();
	void dislplay(const Camera&camera);
	bool done()const;
private:
	void addNewFireWork(float x, float y);
	LinkedList<FireWork*>*fireWork;
	float timeDisplay;
	
	std::vector<Texture*>*textureCollection;
	SoundContainer*soundCollection;
	Size*cameraSize;
};

class FireWork {
public:
	FireWork(float x, float y, Texture*texture, Texture * texture2, Sound*sound);
	~FireWork();
	void display(const Camera&camera);
	bool isDead()const;
private:
	LinkedList<Explosion*>*explosion;
	Texture*texture;
	Texture*texture2;
	Sound*sound;
	Vector2D position;
	Vector2D velocity;
	Vector2D acceleration;
	float damping;
	float size;
	bool dead;
};
#endif // !YOUWIND_H
