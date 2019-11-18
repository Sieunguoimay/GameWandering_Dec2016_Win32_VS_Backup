#include "Ecosystem.h"



Ecosystem::Ecosystem(b2World*world, Mover*mover,LevelManager**pointer,
	std::vector<Texture*>**textureColection, SoundContainer*soundCollection)
	:world(world),mover(mover),soundCollection(soundCollection)
{
	map = new Map();
	*pointer = new LevelManager(map,soundCollection->at(6));

	ground = new Ground(world);
	trees = new Trees();
	bushes = new Bushes();
	lasers = new LaserContainer(world);
	gearKillers = new GearKillerContainer(world);
	monster_tomatoes = new Monster_Tomatoes(world);

	reflectors = new ReflectorContainer(world);
	
	bridges = new BridgeContainer(world);
	powerPoles = new PowerPoleContainer(world,mover);
	pools = new PoolContainer(world);
	valleys = new ValleyContainer(world);
	
	destination = new Destination(world,*pointer);

	textureNumber = 24;
	*textureColection = new std::vector<Texture*>();
	this->textureCollection = *textureColection;
}


Ecosystem::~Ecosystem()
{
	for (size_t i = 0; i < textureCollection->size(); i++)
	{
		delete textureCollection->at(i);
		textureCollection->at(i) = NULL;//to be safe
	}
	//what happens with texture at number 21 is unknown......
	//textureCollection->at(21) = NULL;
	//delete textureColection->at(23);
	//for (size_t i = 21; i < ; i++)
	//{
	//	delete textureCollection->at(i);
	//	textureCollection->at(i) = NULL;//to be safe
	//}
	delete textureCollection;
	textureCollection = NULL;
	soundCollection = NULL;
	delete gearKillers;
	gearKillers = NULL;
	delete reflectors;
	reflectors = NULL;
	delete powerPoles;
	powerPoles = NULL;
	delete bridges;
	bridges = NULL;
	delete ground;
	ground = NULL;
	delete bushes;
	bushes = NULL;
	delete lasers;
	lasers = NULL;
	delete trees;
	trees = NULL;
	delete pools;
	pools = NULL;
	delete valleys;
	valleys = NULL;
	delete destination;
	destination = NULL;
	delete map;
	map = NULL;

}


void Ecosystem::setup(SDL_Renderer ** renderer)
{
	loadTexture(renderer);
	map->setup();
	
	bushes->setup(textureCollection->at(1));
	ground->setup(textureCollection->at(0));
	monster_tomatoes->setup(textureCollection->at(2),
		textureCollection->at(3),mover, soundCollection->at(0),soundCollection->at(9));
	trees->setup(textureCollection->at(4));
	gearKillers->setup(textureCollection->at(5), 
		textureCollection->at(6),mover);
	reflectors->setup(textureCollection->at(7));
	bridges->setup(textureCollection->at(8), textureCollection->at(9));
	lasers->setup(textureCollection->at(3), textureCollection->at(10),soundCollection->at(2));
	powerPoles->setup(textureCollection->at(11), textureCollection->at(12), textureCollection->at(3),
		soundCollection);
	pools->setup(textureCollection->at(13), textureCollection->at(14), textureCollection->at(15));
	valleys->setup(textureCollection->at(16), textureCollection->at(17), 
		textureCollection->at(18),soundCollection->at(1));
	destination->setup(textureCollection->at(19), textureCollection->at(20));
}

void Ecosystem::reset()
{
	map->reset();
	
	delete ground;
	ground = new Ground(world);
	ground->setup(textureCollection->at(0));

	//these following objects bound things which need at least 2 map data to be created
	//and hence we need to make sure that there is no more mark left when the game reset(mover dies)
	pools->reset();
	bridges->reset();
	valleys->reset();
	monster_tomatoes->reset();
	gearKillers->reset();
	putEverythingAtFirst();
}

void Ecosystem::display(const Camera&camera)
{
	updateLandscape(camera.getPosition().x);
	
	valleys->display(camera);

	ground->display(camera);
	
	bushes->display(camera);
	trees->display(camera);

	lasers->display(camera);
	monster_tomatoes->display(camera);
	reflectors->display(camera);

	powerPoles->display(camera);
	destination->display(camera);

}

void Ecosystem::display2(const Camera & camera)
{
	bridges->display(camera);
	gearKillers->display(camera);
	pools->display(camera);
}

Map * Ecosystem::getMapPointer()
{
	return map;
}


void Ecosystem::putEverything(float x, float y, char ID, bool leftOrRight)
{
	//ground
	if (ID == '0')
		ground->putNewBrick(x, y, 1, leftOrRight);//on the right
	else if (ID == '1')
		ground->putNewBrick(x, y, 2, leftOrRight);//on the right
	else if (ID == '2')
		ground->putNewBrick(x, y, 3, leftOrRight);//on the right

	//Bellow is the story of create pointer to all the items taken care of by the specificly current brick - managed inside - by ground
	//- ground -> set pointer to item at the head->next Node or the newest brick - that's all.
	//all of the pointers will be store inside that brick in order to when the brick die, in its destructor
	//it will send a message to all the pointers that it die. 
	// with the message recieved. all of them will autodestroyed inside their manager eg. Bush managed by bushes with a linkedlist<Bush*>
	//return;
	//bushes
	else 
	if (ID >= '3' && ID <= '6') {
		bushes->plant(x, y - ground->getSize().h*0.5f, UtilityFunctions::charToInt(ID) - 3);
		ground->setPointerTo(bushes->getNewBush());
	}

	//trees
	else if (ID == '7') {
		trees->plant(x, y - ground->getSize().h *0.5f, UtilityFunctions::charToInt(ID) - 7);
		ground->setPointerTo(trees->getNewBush());
	}

	//monsters
	else if (ID == '8') {
		if (leftOrRight)
		{
			if (!monster_tomatoes->checkID(x))
				monster_tomatoes->putNewMonster(x, y, x);
		}
		else
		{
			if (!monster_tomatoes->checkID(x))
				monster_tomatoes->putNewMonster(x, y, x);
		}
	}

	//Lasers
	else if (ID == '9') {
		lasers->addNewLaser(x, y);
		ground->setPointerTo(lasers->getNewLaser());
	}
	//reflector - mirror
	else if (ID == 'a'|| ID == 'b'|| ID == 'c'|| ID == 'd') {
		reflectors->addNewReflector(x, y, (int)ID);
		ground->setPointerTo(reflectors->getNewReflector());
	}
	else if (ID == 'e') {
			powerPoles->putNewPowerPole(x, y + ground->getSize().h / 2, x);
		ground->setPointerTo(powerPoles->getNewPowerPole());
	}

	//need 2 marks
	else if (ID == 'f') {
		bridges->putBridgeMark(x, y);
		ground->setPointerTo(bridges->getNewTreeRoot());
	}


	//need 2 marks
	else if (ID == 'g') {
		pools->addPoolMark(x, y,ground->getSize().w/2,1);
		ground->setPointerTo(pools->getNewPoolMark());
	}

	else if (ID == 'h') {
		pools->addPoolMark(x, y, ground->getSize().w / 2, 2);
		ground->setPointerTo(pools->getNewPoolMark());
	}

	//need 2 marks
	else if (ID == 'i') {
		valleys->addValleyMark(x, y,1);
		ground->setPointerTo(valleys->getNewMark());
	}
	else if (ID == 'j') {
		valleys->addValleyMark(x, y, 2);
		ground->setPointerTo(valleys->getNewMark());
	}

	//gear killer machines
	else if (ID == 'k') {
		//create a new killer machine according to map
		gearKillers->addNewKiller(x, y + ground->getSize().h, 1);
		//set the pointer to this new Killer machine for the newest create brick before this
		ground->setPointerTo(gearKillers->getNewKiller());
		//what if there is no brick previous this??
	}

	//gear killer machines
	else if (ID == 'l') {
		//create a new killer machine according to map
		gearKillers->addNewKiller(x, y + ground->getSize().h, 2);
		//set the pointer to this new Killer machine for the newest create brick before this
		ground->setPointerTo(gearKillers->getNewKiller());
		//what if there is no brick previous this??
	}
	//gear killer machines
	else if (ID == 'm') {
		//create a new killer machine according to map
		gearKillers->addNewKiller(x, y + ground->getSize().h, 3);
		//set the pointer to this new Killer machine for the newest create brick before this
		ground->setPointerTo(gearKillers->getNewKiller());
		//what if there is no brick previous this??
	}
		else if (ID == 'o') {
		pools->addPoolMark(x, y, ground->getSize().w / 2, 3);
		ground->setPointerTo(pools->getNewPoolMark());
	}


	//destination
	else if (ID == 'n') {
		destination->addNewDestination(x, y+ground->getSize().h/2);
		ground->setPointerTo(destination->getNewDestinationBody());
	}

}
////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////// Don't look down =)
void Ecosystem::updateLandscape(float cameraPosX)
{
	float x = 0.0f;
	float y = 0.0f;
	int numberBrickAdded = 0;
	
	if ((cameraPosX > (map->getFirstIndex()+map->getNumberBricks()/3+1)*ground->getSize().w) 
		&&(map->getLastIndex() < map->getSize())){
		//add new brick at first - on the right
		for (size_t i = 0; i < map->getMapDataOnTheRight()->size(); i++){
			if (map->getMapDataOnTheRight()->at(i) >= '0'){
				//&& // - 0 is underground brick, 1 is surface brick
				//map->getMapDataOnTheRight()->at(i) <= '9') {
				char ID = map->getMapDataOnTheRight()->at(i);
				x = (float)map->getLastIndex()*ground->getSize().w;

				//devide into 2 groups as discussing bellow
				//group 2: tree, brush,
				if (!(ID == '0' || ID == '1' || ID == '2'))
					numberBrickAdded--;
				if (conditionToPutBush(ID)) {
					float y = (15.0f - (float)numberBrickAdded)*ground->getSize().h;
					putEverything(x, y, ID, true);
				}
				else {//group 1: reflector...
					float y = (15.0f - (float)i)*ground->getSize().h;
					putEverything(x, y, ID, true);
				}
			}
			numberBrickAdded++;
		}

		
		//delete the end terminal bricks
		for (size_t i = 0; i < map->getMapDataOnTheLeft()->size(); i++){
			if (map->getMapDataOnTheLeft()->at(i) >= '0'){
				//&&map->getMapDataOnTheLeft()->at(i) <= '9') {
				char ID = map->getMapDataOnTheLeft()->at(i);
				//del bricks
				if (ID < '2')
					ground->delBrick(false);
			}
		}
		map->increaseIndex();
	}
	else if ((cameraPosX < (map->getFirstIndex() + map->getNumberBricks() / 3)*ground->getSize().w)
		&&map->getFirstIndex()>0) {
		map->decreaseIndex();
		//add new brick at last - on the left
		for (size_t i = 0; i < map->getMapDataOnTheLeft()->size(); i++){
			if (map->getMapDataOnTheLeft()->at(i) >= '0'){
				//&& // - 0 is underground brick, 1 is surface brick
				//map->getMapDataOnTheLeft()->at(i) <= '9') {
				char ID = map->getMapDataOnTheLeft()->at(i);
				x = (float)map->getFirstIndex()*ground->getSize().w;

				//devide into 2 groups as discussing bellow
				//group 2: tree, brush,
				if (!(ID == '0' || ID == '1' || ID == '2'))
					numberBrickAdded--;

				if (conditionToPutBush(ID)) {
					float y = (15.0f - (float)numberBrickAdded)*ground->getSize().h;
					putEverything(x, y, ID, false);
				}
				else {//group 1: reflector...
					float y = (15.0f - (float)i)*ground->getSize().h;
					putEverything(x, y, ID, false);
				}
			}
			numberBrickAdded++;
		}

		//delete the begin terminal bricks
		for (size_t i = 0; i < map->getMapDataOnTheRight()->size(); i++){
			if (map->getMapDataOnTheRight()->at(i) >= '0'){
				//&&map->getMapDataOnTheRight()->at(i) <= '9') {
				char ID = map->getMapDataOnTheRight()->at(i);
				//del bricks
				if (ID < '2') 
					ground->delBrick(true);
			}
		}
	}
}


void Ecosystem::putEverythingAtFirst()
{
	for (int i = 0; i < map->getNumberBricks(); i++) {
		int numberBrickAdded = 0;
		//add new brick at first - on the right
		for (size_t j = 0; j < map->getMapData(i)->size(); j++) {
			if (map->getMapData(i)->at(j) >= '0') {//&& // - 0 is underground brick, 1 is surface brick
				char ID = map->getMapData(i)->at(j);
				float x = (float)i*ground->getSize().w;

				//devide into 2 groups as discussing bellow
				//group 2: tree, brush,
				if (!(ID == '0' || ID == '1' || ID == '2'||ID==' '))
					numberBrickAdded--;
	
				if (conditionToPutBush(ID)) {
					float y = (15.0f - (float)numberBrickAdded)*ground->getSize().h;
					putEverything(x, y, ID, true);
				}
				else {//group 1: reflector...
					float y = (15.0f - (float)j)*ground->getSize().h;
					putEverything(x, y, ID, true);
				}
			}
			numberBrickAdded++;
		}
	}
}
bool Ecosystem::conditionToPutBush(char & ID)
{
	return (ID == '3' || ID == '4' || ID == '5' || ID == '6' || ID == '7' || ID == '8');
}
//at the moment, there are 2 groups:
//group 1 uses the y position of exactly j index in the map->getMapData->at(i)
//includes: brick, laser, gear killer machine,reflector and monster
//group 2 uses the y position of the last brick added 
// includes trees, brushes, ... 7 2 3 4 5-> brushes and trees



void Ecosystem::loadTexture(SDL_Renderer**renderer)
{

	textureCollection->push_back(new Texture("images/brick2.png", renderer, 3));
	textureCollection->push_back(new Texture("images/bushes.png", renderer, 4));
	textureCollection->push_back(new Texture("images/monster_tomato.png", renderer, 5));
	textureCollection->push_back(new Texture("images/bulb.png", renderer, 1));
	textureCollection->push_back(new Texture("images/tree.png", renderer, 1));
	textureCollection->push_back(new Texture("images/gear.png", renderer, 1));
	textureCollection->push_back(new Texture("images/stick.png", renderer, 1));
	textureCollection->push_back(new Texture("images/mirror.png", renderer, 1));
	textureCollection->push_back(new Texture("images/wood.png", renderer, 1));
	textureCollection->push_back(new Texture("images/root.png", renderer, 1));
	textureCollection->push_back(new Texture("images/laser.png", renderer, 1));//10
	textureCollection->push_back(new Texture("images/fly.png", renderer, 1));//11
	textureCollection->push_back(new Texture("images/powerpole.png", renderer, 1));//12

	textureCollection->push_back(new Texture("images/rectangle.png", renderer, 1));//13 pool
	textureCollection->push_back(new Texture("images/fish.png", renderer, 1));//14
	textureCollection->push_back(new Texture("images/floatingwood.png", renderer, 1));//15

	textureCollection->push_back(new Texture("images/cloud.png", renderer, 1));//16 - clouds
	textureCollection->push_back(new Texture("images/rectangle.png", renderer, 1));//17  rainDrop
	textureCollection->push_back(new Texture("images/rectangle.png", renderer, 1));//18  thunder lightning
	textureCollection->push_back(new Texture("images/destination.png", renderer, 1));//19  destination texture
	textureCollection->push_back(new Texture("images/rectangle.png", renderer, 1));//20  nextlevel texture


	textureCollection->push_back(new Texture("images/mapbutton.png", renderer, 4));//21 - for menu
	textureCollection->push_back(new Texture("images/levelnumber.png", renderer, NUM_MAP_LEVEL));//22 - for menu
	textureCollection->push_back(new Texture("images/backgroundmenu.png", renderer, 1));//23- for menu

	textureCollection->push_back(new Texture("images/exitbutton.png", renderer, 4));//24- for menu
	textureCollection->push_back(new Texture("images/mousecursor.png", renderer, 2));//25- for menu

	textureCollection->push_back(new Texture("images/homebutton.png", renderer, 3));//26- for menu
	textureCollection->push_back(new Texture("images/resumebutton.png", renderer, 3));//27- for menu
	textureCollection->push_back(new Texture("images/settingbutton.png", renderer, 3));//28- for menu
	
	textureCollection->push_back(new Texture("images/backbutton.png", renderer, 3));//29- for menu

	textureCollection->push_back(new Texture("images/chunkbutton.png", renderer, 4));//30- for menu
	textureCollection->push_back(new Texture("images/musicbutton.png", renderer, 4));//31- for menu
	textureCollection->push_back(new Texture("images/framesoundcontroller.png", renderer, 1));//32- for menu
	textureCollection->push_back(new Texture("images/movingbuttonsoundcontroller.png", renderer, 3));//33- for menu
	
	textureCollection->push_back(new Texture("images/mainposter.png", renderer, 1));//34- for menu
	textureCollection->push_back(new Texture("images/pressanykey.png", renderer, 1));//35- for menu


	//background
	textureCollection->push_back(new Texture("images/rectangle.png", renderer));//36
	textureCollection->push_back(new Texture("images/background_midlayer.png", renderer));//37
	textureCollection->push_back(new Texture("images/rectangle.png", renderer));//38 mountain brick
	textureCollection->push_back(new Texture("images/foreshader.png", renderer));//39 mountain brick

	textureCollection->push_back(new Texture("images/onfullscreenbutton.png", renderer,3));//40 mountain brick
	textureCollection->push_back(new Texture("images/offfullscreenbutton.png", renderer, 3));//41 mountain brick
	textureCollection->push_back(new Texture("images/youwin.png", renderer,1));//42 mountain brick


	textureCollection->push_back(new Texture("images/exitbutton.png", renderer, 4));//43- for menu

	textureCollection->push_back(new Texture("images/exitbutton.png", renderer, 4));//44- for menu

	textureCollection->push_back(new Texture("images/exitbutton.png", renderer, 4));//45- for menu

	textureCollection->push_back(new Texture("images/exitbutton.png", renderer, 4));//46- for menu

}
