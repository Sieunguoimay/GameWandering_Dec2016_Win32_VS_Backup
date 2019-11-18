#pragma once
#ifndef PATH_H
#define PATH_H
#include"GlobalHeader.h"
class Path
{
public:
	friend class Bird;
	Path();
	~Path();
	void setup(Vector2D&position);
	LinkedList<Vector2D*>*getPaths();
private:
	void setupInCircle();
	LinkedList<Vector2D*>*paths;
	Vector2D*position;
	float radius;
};

#endif // !PATH_H
