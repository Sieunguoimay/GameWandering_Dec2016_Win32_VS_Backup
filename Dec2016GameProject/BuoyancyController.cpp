#include "BuoyancyController.h"



BuoyancyController::BuoyancyController()
{
	//SDL_Log("Created Buoyancy Controller.\n");
}


BuoyancyController::~BuoyancyController()
{
	//SDL_Log("Destroyed Buoyancy Controller.\n");
}

void BuoyancyController::BeginContact(b2Contact * contact)
{
	b2Fixture*fixtureA = contact->GetFixtureA();
	b2Fixture*fixtureB = contact->GetFixtureB();
	void*userDataA = fixtureA->GetBody()->GetUserData();
	void*userDataB = fixtureB->GetBody()->GetUserData();

	if (userDataA&&userDataB) {
		//the first partner is fluid and the second one is dynamic body
		if (((ObjectData*)userDataA)->ID == SomeDefine::WATER_POOL_ID&&
			((ObjectData*)userDataB)->ID != SomeDefine::FISH_ID&&
			fixtureB->GetBody()->GetType() == b2_dynamicBody)
		{
			fixturePairs.insert(std::make_pair(fixtureA, fixtureB));

		}
		else if (((ObjectData*)userDataB)->ID == SomeDefine::WATER_POOL_ID&&
			((ObjectData*)userDataA)->ID != SomeDefine::FISH_ID&&
			fixtureA->GetBody()->GetType() == b2_dynamicBody)
		{
			fixturePairs.insert(std::make_pair(fixtureB, fixtureA));
		}
	}
}

void BuoyancyController::EndContact(b2Contact * contact)
{

	b2Fixture*fixtureA = contact->GetFixtureA();
	b2Fixture*fixtureB = contact->GetFixtureB();
	void*userDataA = fixtureA->GetBody()->GetUserData();
	void*userDataB = fixtureB->GetBody()->GetUserData();
	if (userDataA&&userDataB) {
		if (((ObjectData*)userDataA)->ID == SomeDefine::WATER_POOL_ID&&
			((ObjectData*)userDataB)->ID != SomeDefine::FISH_ID&&
			fixtureB->GetBody()->GetType() == b2_dynamicBody)
		{
			fixturePairs.erase(std::make_pair(fixtureA, fixtureB));
		}
		else if (
			((ObjectData*)userDataB)->ID == SomeDefine::WATER_POOL_ID &&
			((ObjectData*)userDataA)->ID != SomeDefine::FISH_ID&&
			fixtureA->GetBody()->GetType() == b2_dynamicBody)
		{
			fixturePairs.erase(std::make_pair(fixtureB, fixtureA));
		}
	}
}

void BuoyancyController::step()
{
	//go through all buoyancy fixture pairs and apply necessary forces
	std::set<FixturePair>::iterator it = fixturePairs.begin();
	std::set<FixturePair>::iterator end = fixturePairs.end();
	while (it != end) {

		//fixtureA is the fluid
		b2Fixture* fixtureA = it->first;
		b2Fixture* fixtureB = it->second;
		void*userDataA = fixtureA->GetBody()->GetUserData();
		void*userDataB = fixtureB->GetBody()->GetUserData();
		if (userDataA&&userDataB) {

			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID)
				((Pool*)((ObjectData*)userDataA)->pointer)->findOutMoverUnderWater(((Mover*)((ObjectData*)userDataB)->pointer)->getPosition());
		
		}

		processPolygonShape(fixtureA, fixtureB);

		processCircleShape(fixtureA, fixtureB);

		++it;
	}
}

BuoyancyController * BuoyancyController::create()
{
	return new BuoyancyController();
}






void BuoyancyController::processPolygonShape(b2Fixture*fixtureA, b2Fixture*fixtureB)
{

	float density = fixtureA->GetDensity();
	std::vector<b2Vec2> intersectionPoints;
	if (findIntersectionOfFixtures(fixtureA, fixtureB, intersectionPoints)) {

		//find centroid
		float area = 0.0f;
		b2Vec2 centroid = ComputeCentroid(intersectionPoints, area);
		//apply buoyancy force
		float displacedMass = density * area;
		b2Vec2 gravity(0.0f, GRAVITY);
		b2Vec2 buoyancyForce = displacedMass * -gravity;
		fixtureB->GetBody()->ApplyForce(buoyancyForce, centroid, true);

		/*
		//simple drag
		//find relative velocity between object and fluid
		b2Vec2 velDir = fixtureB->GetBody()->GetLinearVelocityFromWorldPoint( centroid ) -
		fixtureA->GetBody()->GetLinearVelocityFromWorldPoint( centroid );
		float vel = velDir.Normalize();

		float dragMod = 1;//adjust as desired
		float dragMag = fixtureA->GetDensity() * vel * vel;
		b2Vec2 dragForce = dragMod * dragMag * -velDir;
		fixtureB->GetBody()->ApplyForce( dragForce, centroid );
		float angularDrag = area * -fixtureB->GetBody()->GetAngularVelocity();
		fixtureB->GetBody()->ApplyTorque( angularDrag );
		//*/

		//apply complex drag
		float dragMod = 0.25f;//adjust as desired
		float liftMod = 0.25f;//adjust as desired
		float maxDrag = 2000;//adjust as desired
		float maxLift = 500;//adjust as desired
		for (size_t i = 0; i < intersectionPoints.size(); i++) {
			b2Vec2 v0 = intersectionPoints[i];
			b2Vec2 v1 = intersectionPoints[(i + 1) % intersectionPoints.size()];
			b2Vec2 midPoint = 0.5f * (v0 + v1);

			//find relative velocity between object and fluid at edge midpoint
			b2Vec2 velDir = fixtureB->GetBody()->GetLinearVelocityFromWorldPoint(midPoint) -
				fixtureA->GetBody()->GetLinearVelocityFromWorldPoint(midPoint);
			float vel = velDir.Normalize();

			b2Vec2 edge = v1 - v0;
			float edgeLength = edge.Normalize();
			b2Vec2 normal = b2Cross(-1, edge);
			float dragDot = b2Dot(normal, velDir);
			if (dragDot < 0)
				continue;//normal points backwards - this is not a leading edge

						 //apply drag
			float dragMag = dragDot * dragMod * edgeLength * density * vel * vel;
			dragMag = b2Min(dragMag, maxDrag);
			b2Vec2 dragForce = dragMag * -velDir;
			fixtureB->GetBody()->ApplyForce(dragForce, midPoint, true);

			//apply lift
			float liftDot = b2Dot(edge, velDir);
			float liftMag = dragDot * liftDot * liftMod * edgeLength * density * vel * vel;
			liftMag = b2Min(liftMag, maxLift);
			b2Vec2 liftDir = b2Cross(1, velDir);
			b2Vec2 liftForce = liftMag * liftDir;
			fixtureB->GetBody()->ApplyForce(liftForce, midPoint, true);
		}
		
	}

}

void BuoyancyController::processCircleShape(b2Fixture * fixtureA, b2Fixture * fixtureB)
{
	if (fixtureB->GetShape()->GetType() == b2Shape::Type::e_circle) {
		float h = fabsf(
			((b2PolygonShape*)fixtureA->GetShape())->GetVertex(0).y
			-((b2PolygonShape*)fixtureA->GetShape())->GetVertex(2).y)/2.0f;

		float fluidSurfaceY = fixtureA->GetBody()->GetPosition().y - h;
		float posY = fixtureB->GetBody()->GetPosition().y;
		float distance = fluidSurfaceY - posY;//radius of 
		float radius = ((b2CircleShape*)fixtureB->GetShape())->m_radius;

		if (posY + radius > fluidSurfaceY
			&&
			posY - radius < fluidSurfaceY+2.0f*h) {
			float totalArea = PI*radius*radius;
			float triangleArea = 0.0f;
			float density = fixtureA->GetDensity();

			float area = totalArea;
			b2Vec2 centroid = fixtureB->GetBody()->GetWorldCenter();
			if (distance > -radius) {
				float alphaArea = acosf(distance / radius);
				area = (alphaArea / PI)*totalArea - triangleArea;
				centroid.y += (distance + radius) / 2.0f;
			}
			//apply buoyancy force
			float displacedMass = density * area;
			b2Vec2 gravity(0, GRAVITY);
			b2Vec2 buoyancyForce = displacedMass * -gravity;

			fixtureB->GetBody()->ApplyForce(buoyancyForce, centroid, true);

			
			//simple drag
			//find relative velocity between object and fluid
			b2Vec2 velDir = 
				fixtureB->GetBody()->GetLinearVelocityFromWorldPoint(centroid) -
				fixtureA->GetBody()->GetLinearVelocityFromWorldPoint(centroid);
			float vel = velDir.Normalize();

			float dragMod = 1;//adjust as desired
			float dragMag = density * vel * vel;

			b2Vec2 dragForce = dragMod * dragMag * -velDir;
			dragForce *= UtilityFunctions::P2M();
			fixtureB->GetBody()->ApplyForce(dragForce, centroid, true);
			float angularDrag = area * -fixtureB->GetBody()->GetAngularVelocity();
			fixtureB->GetBody()->ApplyTorque(angularDrag, true);
		
		}
	}
}

//////////////////////////////////////////////////////////////
b2Vec2 BuoyancyController::ComputeCentroid(std::vector<b2Vec2> vs, float& area)
{
	int count = (int)vs.size();
	b2Assert(count >= 3);

	b2Vec2 c; c.Set(0.0f, 0.0f);
	area = 0.0f;

	// pRef is the reference point for forming triangles.
	// It's location doesn't change the result (except for rounding error).
	b2Vec2 pRef(0.0f, 0.0f);

	const float32 inv3 = 1.0f / 3.0f;

	for (int32 i = 0; i < count; ++i)
	{
		// Triangle vertices.
		b2Vec2 p1 = pRef;
		b2Vec2 p2 = vs[i];
		b2Vec2 p3 = i + 1 < count ? vs[i + 1] : vs[0];

		b2Vec2 e1 = p2 - p1;
		b2Vec2 e2 = p3 - p1;

		float32 D = b2Cross(e1, e2);

		float32 triangleArea = 0.5f * D;
		area += triangleArea;

		// Area weighted centroid
		c += triangleArea * inv3 * (p1 + p2 + p3);
	}

	// Centroid
	if (area > b2_epsilon)
		c *= 1.0f / area;
	else
		area = 0;
	return c;
}

bool BuoyancyController::inside(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 p) {
	return (cp2.x - cp1.x)*(p.y - cp1.y) > (cp2.y - cp1.y)*(p.x - cp1.x);
}

b2Vec2 BuoyancyController::intersection(b2Vec2 cp1, b2Vec2 cp2, b2Vec2 s, b2Vec2 e) {
	b2Vec2 dc(cp1.x - cp2.x, cp1.y - cp2.y);
	b2Vec2 dp(s.x - e.x, s.y - e.y);
	float n1 = cp1.x * cp2.y - cp1.y * cp2.x;
	float n2 = s.x * e.y - s.y * e.x;
	float n3 = 1.0f / (dc.x * dp.y - dc.y * dp.x);
	return b2Vec2((n1*dp.x - n2*dc.x) * n3, (n1*dp.y - n2*dc.y) * n3);
}

//http://rosettacode.org/wiki/Sutherland-Hodgman_polygon_clipping#JavaScript
bool BuoyancyController::findIntersectionOfFixtures(b2Fixture* fA, b2Fixture* fB, std::vector<b2Vec2>& outputVertices)
{
	//currently this only handles polygon vs polygon
	if (fA->GetShape()->GetType() != b2Shape::e_polygon ||
		fB->GetShape()->GetType() != b2Shape::e_polygon)
		return false;

	b2PolygonShape* polyA = (b2PolygonShape*)fA->GetShape();
	b2PolygonShape* polyB = (b2PolygonShape*)fB->GetShape();

	//fill 'subject polygon' from fixtureA polygon
	for (int i = 0; i < polyA->GetVertexCount(); i++)
		outputVertices.push_back(fA->GetBody()->GetWorldPoint(polyA->GetVertex(i)));

	//fill 'clip polygon' from fixtureB polygon
	std::vector<b2Vec2> clipPolygon;
	for (int i = 0; i < polyB->GetVertexCount(); i++)
		clipPolygon.push_back(fB->GetBody()->GetWorldPoint(polyB->GetVertex(i)));

	b2Vec2 cp1 = clipPolygon[clipPolygon.size() - 1];
	for (size_t j = 0; j < clipPolygon.size(); j++) {
		b2Vec2 cp2 = clipPolygon[j];
		if (outputVertices.empty())
			return false;
		std::vector<b2Vec2> inputList = outputVertices;
		outputVertices.clear();
		b2Vec2 s = inputList[inputList.size() - 1]; //last on the input list
		for (size_t i = 0; i < inputList.size(); i++) {
			b2Vec2 e = inputList[i];
			if (inside(cp1, cp2, e)) {
				if (!inside(cp1, cp2, s)) {
					outputVertices.push_back(intersection(cp1, cp2, s, e));
				}
				outputVertices.push_back(e);
			}
			else if (inside(cp1, cp2, s)) {
				outputVertices.push_back(intersection(cp1, cp2, s, e));
			}
			s = e;
		}
		cp1 = cp2;
	}

	return !outputVertices.empty();
}
