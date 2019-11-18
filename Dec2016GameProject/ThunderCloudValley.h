#pragma once
#ifndef THUNDERCLOUDVALLEY_H
#define THUNDERCLOUDVALLEY_H
#include"Cloud.h"
#include"SendMessage.h"
class ValleyMark:public SendMessage
{
public:
	ValleyMark(Vector2D*position):position(position){
		state = false;
	}
	~ValleyMark() {
		delete position;
		state = false;
	}
	bool state;
	Vector2D*position;
};
typedef std::pair<Node<ValleyMark*>*, Node<ValleyMark*>*> ValleyMarkPair;
class WanderingCloudValley {
public:
	WanderingCloudValley(ValleyMarkPair*velleyMarkPair, int*pointerToNumNodes);
	~WanderingCloudValley();
	virtual void setup(Texture*cloudTexture, Texture*rainTexture);
	virtual void display(const Camera&camera);
	bool isDead()const;
	virtual void destroyValley();
protected:

	LinkedList<Cloud*>*clouds;
	ValleyMarkPair*valleyMarkPair;
	int*pointerToNumNodes;
	Rectangle*area;

	Texture*cloudTexture;
	Texture*rainTexture;
};
class ThunderCloudValley:public WanderingCloudValley
{
public:
	ThunderCloudValley(b2World*world, ValleyMarkPair*velleyMarkPair, int*pointerToNumNodes);
	~ThunderCloudValley();

	void setup(Texture*cloudTexture,Texture*rainTexture, Texture*thunderTexture,Sound*thunderSound);
	void display(const Camera&camera);
	void keepTrackOfObjectInsideArea(Vector2D*position);
	void destroyValley();
private:
	//for transfering position of object into for cloud class
	Vector2D*deliver;
	bool isThereMessage;

	b2World*world;
	b2Body*body;
	ObjectData*data;
};

#endif // !THUNDERCLOUDVALLEY_H
