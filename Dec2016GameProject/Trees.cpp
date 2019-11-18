#include "Trees.h"



Trees::Trees()
{
}


Trees::~Trees()
{
}

void Trees::setup(Texture * texture)
{
	skins = texture;
	size->set(300.0f, 500.0f);//adjust yourself to fit the 1366x768
}
