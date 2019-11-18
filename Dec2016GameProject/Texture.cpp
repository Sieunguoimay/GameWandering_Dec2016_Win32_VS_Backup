#include "Texture.h"


Texture::Texture(std::string path, SDL_Renderer ** renderer,int numberSprite)
	: renderer(renderer)
{
	if (path != "NULL") {
		SDL_Surface*loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) {
			//SDL_Log("Error to load texture: %s", IMG_GetError());
		}
		else {
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xff, 0xff));
			texture = SDL_CreateTextureFromSurface(*this->renderer, loadedSurface);

			size = new Size((float)(loadedSurface->w/(float)numberSprite), (float)loadedSurface->h);
			SDL_FreeSurface(loadedSurface);
			//printf("Loading Texture Successed: %s\n",path.c_str());
		}
	}
	spriteRects = new std::vector<SDL_Rect*>();
	setupSprite(numberSprite);
}

Texture::~Texture()
{
	static int counter = 0;
	counter++;
	//SDL_Log("Delete Texture no. %d\n", counter);

	
	delete size;
	size = NULL;
	for (size_t i = 0; i < spriteRects->size(); i++)
	{
		delete spriteRects->at(i);
		spriteRects->at(i) = NULL;
	}
	delete spriteRects;
	spriteRects = NULL;

	SDL_DestroyTexture(texture);
	texture = NULL;
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::setColor(Color * color)
{
	SDL_SetTextureColorMod(texture, color->r, color->g, color->b);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(texture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::render(int x, int y, int w, int h, SDL_Rect * src)
{
	SDL_Rect renderQuad{ x,y,w,h };
	SDL_RenderCopyEx(*renderer, texture, src, &renderQuad,0,NULL,SDL_FLIP_NONE);
}

void Texture::render(int x, int y, int w, int h, int index)
{
	SDL_Rect renderQuad{ x,y,w,h };
	SDL_RenderCopy(*renderer, texture, spriteRects->at(index), &renderQuad);
}


void Texture::render(const Camera&camera,float x, float y, float w, float h, int spriteIndex, float angle,bool horizontalFlip)
{
	SDL_Rect renderQuad{ 
		(int)(x*(camera.getSize().w / WINDOWSIZE_W)),
		(int)(y*(camera.getSize().h / WINDOWSIZE_H)),
		(int)(w*(camera.getSize().w / WINDOWSIZE_W) + ERROR_DISPLAY),
		(int)(h*(camera.getSize().h / WINDOWSIZE_H) + ERROR_DISPLAY)};
	
	SDL_RenderCopyEx(*renderer, texture, spriteRects->at(spriteIndex), &renderQuad,
		(double)angle*180.0/3.141592,NULL,(horizontalFlip?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE));
}

void Texture::render(int x, int y,SDL_Rect*desRect)
{
	SDL_Rect renderQuad{ x,y,(int)size->w,(int)size->h };
	SDL_RenderCopy(*renderer, texture, NULL, (desRect==NULL)?&renderQuad:desRect);
}

void Texture::render(Vector2D * location, Size * size,float angle)
{
	SDL_Rect renderQuad{ (int)location->x- (int)size->w/2,(int)location->y- (int)size->h/2,(int)size->w,(int)size->h };
	SDL_RenderCopyEx(*renderer, texture, NULL, &renderQuad,(double)angle,NULL,SDL_FLIP_NONE);
}

void Texture::render(int x, int y, Size * size, float angle, SDL_RendererFlip rendererFlip)
//angle is in degreee.
{
	SDL_Rect renderQuad;
	if (size != NULL)
		renderQuad = { x-(int)size->w/2,y - (int)size->h/2 ,(int)size->w,(int)size->h };
	else 
		renderQuad = { x- (int)this->size->w/2,y - (int)this->size->h/2 ,(int)this->size->w,(int)this->size->h };

	SDL_RenderCopyEx(*renderer, texture, NULL, &renderQuad,
		(double)(angle*180.0f/3.141592f), NULL, rendererFlip);
}

void Texture::resize(float w, float h)
{
	size->w = w;
	size->h = h;
}

Size*Texture::getSize()
{
	return size;
}

void Texture::setupSprite(int numberSprites)
{
	for (int i = 0; i < numberSprites; i++) {
		SDL_Rect temp;
		temp.x = (int)(i*size->w);
		temp.y = 0;
		temp.w = (int)size->w;
		temp.h = (int)size->h;
		//copy value of temp into new dynamic SDL_Rect and put it in vector 
		spriteRects->push_back(new SDL_Rect(temp));
	}
}
