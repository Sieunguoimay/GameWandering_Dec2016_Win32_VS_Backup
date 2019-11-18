#pragma once
#include<string>
#include<SDL.h>
#include<SDL_image.h>
#include"GlobalHeader.h"
#include"Camera.h"
#ifndef TEXTURE_H
#define TEXTURE_H
class Texture
{
public:
	Texture(std::string path = "NULL",SDL_Renderer**renderer = NULL,int numberSprite = 1);
	~Texture();
	//set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setColor(Color*color);
	//Set blending
	void setBlendMode(SDL_BlendMode blending);
	//Set alpha modulation
	void setAlpha(Uint8 alpha);
	
	void render(int x, int y, int w, int h, SDL_Rect*src);
	void render(int x, int y, int w, int h,int index);
	void render(const Camera&camera,float x, float y ,float w, float h,int spriteIndex = 0,float angle = 0.0f, bool horizontalFlip = false);
	void render(int x, int y, SDL_Rect*desRect = NULL);
	void render(Vector2D*location, Size*size, float angle = 0.0f);
	void render(int x, int y, Size*size,float angle = 0.0f,SDL_RendererFlip rendererFlip = SDL_FLIP_NONE);

	void resize(float w, float h);
	Size*getSize();

private:
	void setupSprite(int numberSprites);
	std::vector<SDL_Rect*>*spriteRects;
	SDL_Texture*texture;
	Size*size;
	//some pointers
	SDL_Renderer**renderer;
};

#endif // !TEXTURE_H
