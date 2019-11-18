#include "Skin.h"



Skin::Skin()
{
	skins = new std::vector<Texture*>();
	spriteIndices = new std::vector<int>();
}


Skin::~Skin()
{
	for (size_t i = 0; i < skins->size(); i++)
	{
		delete skins->at(i);
		skins->at(i) = NULL;
	}
	delete skins;
	skins = NULL;
	delete spriteIndices;
	spriteIndices = NULL;
}

void Skin::setup(SDL_Renderer ** renderer, Size * size,Vector2D * staticPosition)
{
	horizontalFlip = true;
	numLegSprites = 9;//I know that because I created the image

	this->staticPosition = staticPosition;
	this->renderer = renderer;
	this->size = size;

	setupSprite("images/body.png", renderer);//0
	setupSprite("images/legs.png", renderer,numLegSprites);//1 - 9 sprites
	setupSprite("images/eye.png", renderer, 4);//2 - 4 sprites
	//add here..
	numberBodySprite = 3;
	setupSprite("images/bunnyear.png", renderer);//3 - 1 sprites
	setupSprite("images/bulb.png", renderer);//4 - 1 sprites
}

void Skin::render(const Camera&camera)
{
	blink();
	/*
	SDL_Rect rightRect = { (int)skins->at(0)->getSize()->w / 2,0,(int)skins->at(0)->getSize()->w / 2 ,(int)skins->at(0)->getSize()->h };
	SDL_Rect leftRect = { 0,0,(int)skins->at(0)->getSize()->w / 2 ,(int)skins->at(0)->getSize()->h };
	Vector2D bodySkinPos;
	bodySkinPos.x = dynamicPosition->x*UtilityFunctions::M2P();
	bodySkinPos.y = dynamicPosition->y*UtilityFunctions::M2P() - size->h/2;

	float diffHeight = 1.50f*(dynamicPosition->y - staticPosition->y)*UtilityFunctions::M2P();
	float diffWidth = 1.50f*(staticPosition->x - dynamicPosition->x)*UtilityFunctions::M2P();

	for (size_t i = 0; i <numberBodySprite; i++) skins->at(i)->render(camera,
		(diffWidth < 0 ? bodySkinPos.x : staticPosition->x*UtilityFunctions::M2P())
		- diffHeight / 4 - size->w / 2 - (int)(camera.getPosition().x),
			
		bodySkinPos.y + diffHeight / 1.5f - (int)(camera.getPosition().y),
			
		size->w + diffHeight / 2 - (diffWidth < 0 ? -diffWidth : diffWidth),
		size->h - diffHeight, spriteIndexes->at(i),0, horizontalFlip);
		*/

	for (int i = 0; i < numberBodySprite; i++)
	{
		skins->at(i)->render(camera,
			staticPosition->x - size->w / 2 - camera.getPosition().x,
			staticPosition->y - size->h / 2 - camera.getPosition().y,
			size->w,
			size->h,
			spriteIndices->at(i), 0, horizontalFlip
		);
	}
}

void Skin::walk(float velocityX)
{
	
	if (velocityX > 0.001f) {
		horizontalFlip = true;
	}
	else if (velocityX < -0.001f) { 
		horizontalFlip = false; 
	}
	//else it remain the same

	float framePerSecond = 10.0f*(velocityX>0?velocityX:-velocityX);
	static int signOfIndex = 1;
	secondPerLoop += 1.0f / FPS;

	if (secondPerLoop > (1.0f / framePerSecond)) {
		//Legs sprites update
		spriteIndices->at(SomeDefine::LEG_INDEX)+=signOfIndex;//+ or - to 1
		if (spriteIndices->at(SomeDefine::LEG_INDEX) == 0 ||
			spriteIndices->at(SomeDefine::LEG_INDEX) == numLegSprites - 1)
			signOfIndex *= -1;
		secondPerLoop = 0.0f;
	}
}
Texture * Skin::getTexture(int orderIndex)
{
	return this->skins->at(orderIndex);
}

Texture * Skin::getTailTexture()
{
	return this->skins->at(3);
}
Texture * Skin::getTailTexture2()
{
	return this->skins->at(4);
}
void Skin::blink()
{
	static bool blinked = false;
	static float duration = 0;
	duration -= 1.0f / FPS;
	if (duration <= 0.0f) {
		blinked = true;
		duration = (float)(rand() % 5 + 1);
	}

	if (blinked) {
		static float seconds = 0.0f;
		seconds += 1.0f / FPS;

		float fps = 10;
		if (seconds > 1.0f / fps) {
			if (spriteIndices->at(SomeDefine::EYE_INDEX) <= 0) {
				spriteIndices->at(SomeDefine::EYE_INDEX) = 4;
			}
			spriteIndices->at(SomeDefine::EYE_INDEX)--;
			seconds = 0.0f;
			if (spriteIndices->at(SomeDefine::EYE_INDEX) == 0)
				blinked = false;
		}
	}
}
void Skin::setupSprite(std::string path, SDL_Renderer ** renderer, int numberSprites)
{
	skins->push_back(new Texture(path, renderer, numberSprites));//0
	spriteIndices->push_back(0);//create a new integer variable store inside vector spriteIndexes
}
