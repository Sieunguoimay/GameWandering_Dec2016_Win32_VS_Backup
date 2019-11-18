#include "MountainBrick.h"




MountainBrick::MountainBrick(float x, float y, float w, float h, Texture * texture)
	:texture(texture)
{
	rectangle.set(x, y, w, h);
}

MountainBrick::~MountainBrick()
{
	texture = NULL;
}

void MountainBrick::display(const Camera & camera, float cameraRate)
{
	texture->render(camera, 
		rectangle.x - rectangle.w / 2 - camera.getPosition().x*cameraRate,
		rectangle.y - rectangle.h / 2 - camera.getPosition().y*cameraRate,
		rectangle.w, rectangle.h,0);
}

Rectangle& MountainBrick::getRectangle()
{
	return rectangle;
}
