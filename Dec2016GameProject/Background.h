#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H
#include"globalheader.h"
#include"Camera.h"
#include"Layer.h"
#include"Mountain.h"
class Background
{
public:
	Background(Camera*camera);
	~Background();
	void setup(std::vector<Texture*>*textureCollection);
	void reset(int currentLevel);
	void display();
	void display2();
private:
	Layer* addMidLayer();
	//includes 3 layers:
	
	//static layers very far, with a very low coefficient
	//mid layer is a linkedlist of layers with the same coefficient
	//fore layer is similar to mid layer with a higher coefficient 
	std::vector<Texture*>*textureCollection;
	Mountain*mountain;
	Mountain*mountain2;

	Color*colorOverlay;
	Layer*skyground;
	LinkedList<Layer*>*midlayer;
	Camera*camera;
};

#endif // !BACKGROUND_H
