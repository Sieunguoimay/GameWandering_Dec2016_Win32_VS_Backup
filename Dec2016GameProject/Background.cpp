#include "Background.h"



Background::Background(Camera * camera)
	:camera(camera)
{
	//textureCollection = new std::vector<Texture*>();
	skyground = new Layer();
	midlayer = new LinkedList<Layer*>();

	mountain = new Mountain(camera);
	mountain2 = new Mountain(camera);
	colorOverlay = new Color();
}


Background::~Background()
{
	delete skyground;
	skyground = NULL;
	for (Node<Layer*>*iter = midlayer->getFirstNode(); iter != midlayer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete midlayer;
	midlayer = NULL;
	delete colorOverlay;
	colorOverlay = NULL;
}

void Background::setup(std::vector<Texture*>*textureCollection)
{
	this->textureCollection = textureCollection;
	skyground->setup(
		new Vector2D(camera->getSize().w/2, camera->getSize().h/2+20),
		new Size(camera->getSize().w*1.1f, camera->getSize().h*1.1f),
		0.01f, textureCollection->at(36));
	midlayer->addFirst(addMidLayer());


	mountain->setup(textureCollection->at(38), 0.5f, new Color(52, 56, 75));
	mountain2->setup(textureCollection->at(38), 0.25f, new Color(77, 81, 99));

}

void Background::reset(int currentLevel)
{
	delete skyground;
	skyground = new Layer();
	skyground->setup(
		new Vector2D(camera->getSize().w / 2, camera->getSize().h / 2),
		new Size(camera->getSize().w*1.1f, camera->getSize().h*1.1f),
		0.01f, textureCollection->at(36));

	delete mountain;
	delete mountain2;
	mountain = new Mountain(camera);
	mountain2 = new Mountain(camera);



	if (currentLevel == 3) {
		mountain->setup(textureCollection->at(38), 0.5f, new Color(52, 56, 75));
		mountain2->setup(textureCollection->at(38), 0.25f, new Color(77, 81, 99));
		textureCollection->at(36)->setColor(128, 137, 150);//good for level 4
		//textureCollection->at(39)->setColor(0, 0, 0);//good for level 4
		colorOverlay->set(0, 0, 0);
	}
	else if (currentLevel == 2) {
		mountain->setup(textureCollection->at(38), 0.5f, new Color(119,56,111));
		mountain2->setup(textureCollection->at(38), 0.25f, new Color(142,80,134));
		textureCollection->at(36)->setColor(200,150,200);//good for level 4
	
		//textureCollection->at(39)->setColor();//good for level 4
		colorOverlay->set(119, 56, 111);
	}
	else if (currentLevel == 1) {
		mountain->setup(textureCollection->at(38), 0.5f, new Color(81,75,62));
		mountain2->setup(textureCollection->at(38), 0.25f, new Color(113,103,82));
		textureCollection->at(36)->setColor(133, 170, 185);//good for level 4
		
		//textureCollection->at(39)->setColor(81, 75, 62);//good for level 4
		colorOverlay->set(81, 75, 62);
	}
	else if (currentLevel == 0) {
		mountain->setup(textureCollection->at(38), 0.5f, new Color(86,126,142));
		mountain2->setup(textureCollection->at(38), 0.25f, new Color(105,147,158));
		textureCollection->at(36)->setColor(133, 170, 185);//good for level 4
		
		//textureCollection->at(39)->setColor(255,255,255);//good for level 4
		colorOverlay->set(255,255,255);
	}
}

void Background::display()
{
//	this->skyground->display(*camera);
	int x = 0;
	textureCollection->at(36)->render(x, x, (int)camera->getSize().w, (int)camera->getSize().h, 0);
	for (Node<Layer*>*iter = midlayer->getFirstNode(); iter != midlayer->tail; iter = iter->next)
		iter->data->display(*camera);

	mountain2->display(*camera);
	mountain->display(*camera);
}

void Background::display2()
{
	int x = 0;
	textureCollection->at(39)->setColor(colorOverlay);
	textureCollection->at(39)->render(x,x, (int)camera->getSize().w, (int)camera->getSize().h, 0);
	textureCollection->at(39)->setColor(0xff, 0xff, 0xff);
}

Layer* Background::addMidLayer()
{
	Layer*newLayer = new Layer();
	float randCoefficient = 0.2f;// (float)(rand() % 10 + 1)*0.1f;
	float w = textureCollection->at(37)->getSize()->w*randCoefficient;
	float h = textureCollection->at(37)->getSize()->h*randCoefficient;
	float x = camera->getSize().w/2+rand()%((int)camera->getSize().w/2);
	float y = camera->getSize().h *0.3f+ (camera->getSize().h / 3)*randCoefficient;
	newLayer->setup(new Vector2D(x,y), new Size(w,h), randCoefficient, textureCollection->at(37));

	return newLayer;
}

