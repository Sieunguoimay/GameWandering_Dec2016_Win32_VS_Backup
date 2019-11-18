#include "GearKillerMachine.h"


//every pointers passed into the class is responsible
GearKillerMachine::GearKillerMachine(b2World * world, Mover * moverPointer, Vector2D * position, Texture * gearTexture, Texture * stickTexture, int numberGear)
	:world(world), moverPointer(moverPointer), position(position), gearTexture(gearTexture), stickTexture(stickTexture),numberGear(numberGear)
{
	gearKillerContainer = new LinkedList<GearKiller*>();
	setup();
}

GearKillerMachine::~GearKillerMachine()
{
	world->DestroyBody(staticBodyOrigin);
	staticBodyOrigin = NULL;
	delete position;
	position = NULL;

	for (Node<GearKiller*>*iter = gearKillerContainer->getFirstNode(); iter != gearKillerContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete gearKillerContainer;
	gearKillerContainer = NULL;
}

void GearKillerMachine::setup()
{
	speedRate = 0.25f;
	radius = 40.0f;
	//create a base to hold gearkillers
	//a rectangular shape, static body
	//at position
	
	b2BodyDef bd;
	bd.position.Set(position->x*UtilityFunctions::P2M(), position->y*UtilityFunctions::P2M());
	bd.type = b2_staticBody;
	staticBodyOrigin= world->CreateBody(&bd);

	b2PolygonShape ps;
	ps.SetAsBox(20.0f*UtilityFunctions::P2M(), 20.0f * UtilityFunctions::P2M());//any size is ok - cause we only need the staticbody
	
	b2FixtureDef fd;
	fd.shape = &ps;
	fd.isSensor = true;
	staticBodyOrigin->CreateFixture(&fd);
	//finish creating gear's body
	for (int i = 0; i < numberGear; i++)
		intallNewGearKiller();
}

void GearKillerMachine::display(const Camera & camera)
{
	//we need to send the information to mover from here in case contact listener sending amessage to this class
	//---//
	stickTexture->render(camera,
		position->x - 10.0f - camera.getPosition().x,
		position->y - 10.0f - camera.getPosition().y, 20.0f, 20.0f);

	for (Node<GearKiller*>*iter = gearKillerContainer->getFirstNode(); iter != gearKillerContainer->tail; iter = iter->next)
		iter->data->display(camera);
}
Vector2D * GearKillerMachine::getPosition()
{
	return this->position;
}
//each time this function is called, one new GearKiller will be put into the container
//it must be put in a specific rule
void GearKillerMachine::intallNewGearKiller()
{
	//to adjust the speed (radius/s) of motor
	int sign = 0;
	if (numberGear == 1)
		sign = 1;
	else sign = ((rand() % 10) > 5 ? 1 : -1);
	
	speedRate *= (float)(1.25*sign);
	//to adjust the radius of new Gear tobe Created
	radius -= 2;
	GearKiller*newGearKiller = NULL;
	
	if (gearKillerContainer->getSize() == 0)//incase the first GearKiller to be added
	{
		newGearKiller = new GearKiller(world, moverPointer, new Vector2D(position), gearTexture, stickTexture,radius);
		newGearKiller->plugin(staticBodyOrigin,speedRate*3.141592f);
	}
	else {
		newGearKiller = new GearKiller(world, moverPointer,
			new Vector2D(gearKillerContainer->getFirstNode()->data->getDynamicPosition()),//new new new
			gearTexture, stickTexture, radius);
		newGearKiller->plugin(gearKillerContainer->getFirstNode()->data->getStickBody(),speedRate*3.141592f);
	}
	gearKillerContainer->addFirst(newGearKiller);
}
