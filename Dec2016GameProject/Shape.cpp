#include "Shape.h"



Shape::Shape( b2World*world)
	:world(world)
{
}


Shape::~Shape()
{
}

void Shape::display(SDL_Renderer**renderer)
{
	b2Body*temp = world->GetBodyList();
	b2Vec2 points[4];
	int pointNum = 4;
	int shapeType = 0;
	SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);

	while (temp) {
		shapeType = temp->GetFixtureList()->GetShape()->GetType();
		if (shapeType == 0) {
			void* userData = temp->GetUserData();
			if (userData) {
				if (((ObjectData*)userData)->ID == SomeDefine::SENSOR_ID_ON) {
					SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0x00, 0xff);
					drawCircle(renderer, temp->GetWorldCenter(), temp->GetFixtureList()->GetShape()->m_radius, temp->GetAngle());
				}
				else {
					SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);
					drawCircle(renderer, temp->GetWorldCenter(), temp->GetFixtureList()->GetShape()->m_radius, temp->GetAngle());
				}
			}
			else
			drawCircle(renderer, temp->GetWorldCenter(), temp->GetFixtureList()->GetShape()->m_radius, temp->GetAngle());
		}
		else if (shapeType == 2) {
			pointNum = ((b2PolygonShape*)temp->GetFixtureList()->GetShape())->GetVertexCount();
			for (int i = 0; i < pointNum; i++) {

				points[i] = ((b2PolygonShape*)temp->GetFixtureList()->GetShape())->GetVertex(i);
				rotationTranslate(points[i], temp->GetWorldCenter(), temp->GetAngle());
			}
			drawRectangle(renderer, points, temp->GetPosition(), temp->GetAngle());
		}
		temp = temp->GetNext();
	}
}

b2Body*Shape::addRectangle(float x, float y, int w, int h, bool dynamic,bool fixedRotation,float density,bool isSensor)
{
	b2BodyDef bd;//position, type
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());
	
	if (dynamic)
		bd.type = b2_dynamicBody;
	if (fixedRotation)
		bd.fixedRotation = true;

	b2Body*newBody = world->CreateBody(&bd);//each create a new body for the world

	b2PolygonShape ps;
	ps.SetAsBox(w*UtilityFunctions::P2M() / 2, h*UtilityFunctions::P2M() / 2);
	

	b2FixtureDef fd;
	fd.shape = &ps;
	fd.density = density;
	if (isSensor)
		fd.isSensor = true;
	fd.friction = 0.3f;
	fd.restitution = 0.2f;

	newBody->CreateFixture(&fd);
	return newBody;
}

b2Body* Shape::addCircle(float x, float y, float radius, bool dyn, float density, bool fixedRotate, bool isSensor)
{
	b2BodyDef bd;//position, type
	bd.position.Set(x*UtilityFunctions::P2M(), y*UtilityFunctions::P2M());
	if (dyn)
		bd.type = b2_dynamicBody;
	if (fixedRotate)
		bd.fixedRotation = true;
	bd.gravityScale = 3;//who is heavier
	b2Body*newBody = world->CreateBody(&bd);//each create a new body for the world

	b2CircleShape cs;
	cs.m_radius = radius*UtilityFunctions::P2M()/2;

	b2FixtureDef fd;
	fd.shape = &cs;
	if (isSensor)
		fd.isSensor = true;
	fd.density = density;
	fd.friction = 0.05f;
	fd.restitution = 0.1f;
	
	newBody->CreateFixture(&fd);
	return newBody;
}

void Shape::drawRectangle(SDL_Renderer**renderer,b2Vec2 * points, b2Vec2 center, float angle)
{
	SDL_SetRenderDrawColor(*renderer, 0xff, 0xff, 0xff, 0xff);
	for (int i = 0; i < 4; i++)
		SDL_RenderDrawLine(*renderer,
			(int)((points[i].x + center.x)*UtilityFunctions::M2P()),
			(int)((points[i].y + center.y)*UtilityFunctions::M2P()),
			(int)((points[((i == 3) ? 0 : i + 1)].x + center.x)*UtilityFunctions::M2P()),
			(int)((points[((i == 3) ? 0 : i + 1)].y+ center.y)*UtilityFunctions::M2P()));
}

void Shape::drawCircle(SDL_Renderer**renderer,b2Vec2 center, float radius, float angle)
{
	Vector2D pos;
	const int precision = 10;
	for (int i = 0; i <= 360; i += 360/precision) {
		
		pos.x = radius*cosf(i*3.141592f / 180.0f+angle) + center.x;
		pos.y = radius*sinf(i*3.141592f / 180.0f+angle) + center.y;

		SDL_RenderDrawLine(*renderer,
			(int)(pos.x*UtilityFunctions::M2P()), 
			(int)(pos.y*UtilityFunctions::M2P()),
			(int)((radius*cosf((i+ 360 / precision)*3.141592f / 180.0f + angle)
				+ center.x)*UtilityFunctions::M2P()),
			(int)((radius*sinf((i+ 360 / precision)*3.141592f / 180.0f + angle)
				+ center.y)*UtilityFunctions::M2P()));
	}
}

void Shape::rotationTranslate(b2Vec2&vector, b2Vec2 center, float angle)
{
	Vector2D tempCenter(0, 0);
	
	Vector2D*tempVector = new Vector2D(vector.x, vector.y);
	Vector2D* newVector = UtilityFunctions::polarToCartesian(tempCenter, tempVector->magnitude(),tempVector->getAngle()+ angle);
	vector.Set(newVector->x, newVector->y);

	delete tempVector;
	delete newVector;
}

void Shape::connectRevolution(b2Body * bodyA, b2Body * bodyB)
{
	b2RevoluteJointDef rjd;
	rjd.Initialize(bodyA, bodyB, bodyA->GetWorldCenter());
	
	world->CreateJoint(&rjd);
}




void Shape::connectDistance(b2Body * bodyA, b2Body * bodyB,float frequency)
{
	b2DistanceJointDef djd;

	djd.bodyA = bodyA;
	djd.bodyB = bodyB;
	djd.length = (bodyA->GetPosition() - bodyB->GetPosition()).Length();
	//djd.localAnchorA = bodyA->GetPosition();
	//djd.localAnchorB = bodyB->GetPosition();
	djd.collideConnected = false;//two body can deep into each other
	djd.dampingRatio = 0.2f;
	djd.frequencyHz = frequency;
	world->CreateJoint(&djd);//joint will be automaticly destroyed as the two body connected be destroyed
}
