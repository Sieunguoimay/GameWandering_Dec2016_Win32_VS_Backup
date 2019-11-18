#pragma once
#ifndef BRIDGECONTAINER_H
#define BRIDGECONTAINER_H
#include"Bridge.h"

class BridgeContainer
{
public:
	BridgeContainer(b2World*world);
	~BridgeContainer();
	void reset();
	void setup(Texture*texture1, Texture*texture2);
	void display(const Camera&camera);
	void putBridgeMark(float x, float y);

	TreeRoot*getNewTreeRoot();
	void del(bool leftOrRight);
private:
	void buildNewBridge();
	float radius;
	LinkedList<Bridge*>*bridges;
	LinkedList<TreeRoot*>*bridgeMarks;
protected:
	b2World*world;
	Texture*texture1;
	Texture*texture2;
};

#endif // !BRIDGECONTAINER_H
