#pragma once
#ifndef BRIDGE_H
#define BRIDGE_H
#include"globalheader.h"
#include"Texture.h"
#include"SendMessage.h"
class TreeRoot :public SendMessage
{
public:
	TreeRoot(b2Body*body, float x)
		:body(body),x(x){
		state = false;
	}
	b2Body*body;
	float x;
	bool state;
};
typedef std::pair<Node<TreeRoot*>*, Node<TreeRoot*>*>TreeRootPair;
class Bridge
{
public:
	Bridge(b2World*world,Texture*texture, TreeRootPair*treeRootPair,int*pointerToNumNodes, float radius = 40);
	~Bridge();
	void setup();
	void display(const Camera&camera);
	bool isDead()const;
private:
	void checkStaticBodys();
	void connect2Body(b2Body*body1,b2Body*body2);
	void putNewWood(float x, float y);
	LinkedList<b2Body*>*woods;
	float radius;
	ObjectData*data;
	bool dead;

	TreeRootPair*treeRootPair;
	int*pointerToNumNodes;
	b2World*world;
	Texture*texture;
};

#endif // !BRIDGE_H
