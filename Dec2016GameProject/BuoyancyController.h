#pragma once
#ifndef BUOYANCYCONTROLLER_H
#define BUOYANCYCONTROLLER_H
#include <set>
#include <vector>
#include"globalheader.h"
#include"GameHeader.h"
#include"Mover.h"
#include"Pool.h"
typedef std::pair<b2Fixture*, b2Fixture*> FixturePair;

class BuoyancyController
{
public:
	BuoyancyController();
	~BuoyancyController();
	void BeginContact(b2Contact*contact);
	void EndContact(b2Contact*contact);
	void step();
	static BuoyancyController*create();
private:
	std::set<FixturePair> fixturePairs;
	void processPolygonShape(b2Fixture*fixtureA, b2Fixture*fixtureB);
	void processCircleShape(b2Fixture*fixtureA, b2Fixture*fixtureB);
	static b2Vec2 ComputeCentroid(std::vector<b2Vec2> vs, float& area);
	bool inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p);
	b2Vec2 intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e);
	bool findIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices);
};

#endif // !BUOYANCYCONTROLLER_H
