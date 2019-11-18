#pragma once

#ifndef SKIN_H
#define SKIN_H
#include"Texture.h"
#include"globalheader.h"
#include"Camera.h"
class Skin
{
public:
	Skin();
	~Skin();
	void setup(SDL_Renderer**renderer, Size*size,Vector2D*staticPosition);
	void render(const Camera&camera);
	void walk(float velocityX);
	Texture*getTexture(int orderIndex);
	Texture*getTailTexture();
	Texture*getTailTexture2();

private:
	void blink();
	void setupSprite(std::string path,SDL_Renderer**renderer,int numberSprites = 1);
	std::vector<Texture*>*skins;
	std::vector<int>*spriteIndices;

	Texture*tailTexture;
	Texture*tailTexture2;

	bool horizontalFlip;
	float secondPerLoop;
	int numLegSprites;

	int numberBodySprite;
	//pointers
	SDL_Renderer**renderer;//to draw tail by lines
	Size*size;
	Vector2D*staticPosition;
};

#endif // !SKIN_H
