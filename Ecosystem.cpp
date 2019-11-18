#include"Ecosystem.h"


Ecosystem::Ecosystem(b2World*world,std::vector<Texture*>*textureCollection,
	SoundContainer*soundCollection,Map*map,Mover*mover,LevelManager*levelManager)
	:world(world),textureCollection(textureCollection),
	soundCollection(soundCollection),map(map),mover(mover)
	
{
	ground = new Ground(world,textureCollection->at(0));

	bushes = new Bushes(textureCollection->at(1));

	trees = new Trees(textureCollection->at(4));

	gearKillers = new GearKillerContainer(world,textureCollection->at(5), textureCollection->at(6),mover);	

	monsters = new Monster_Tomatoes(world,mover,textureCollection,soundCollection);
	
	
	destination = new Destination(world,levelManager);
	destination->setup(textureCollection->at(19),textureCollection->at(20));
	lasers = new LaserContainer(world);
	lasers->setup(textureCollection->at(3), textureCollection->at(10),soundCollection->at(2));
	
	reflectors = new ReflectorContainer(world);
	reflectors->setup(textureCollection->at(7));
	
	powerPoles = new PowerPoleContainer(world,mover);
	powerPoles->setup(textureCollection->at(11), textureCollection->at(12), textureCollection->at(3),
		soundCollection);
	
	bridges = new BridgeContainer(world);
	bridges->setup(textureCollection->at(8), textureCollection->at(9));
	
	pools = new PoolContainer(world);
	pools->setup(textureCollection->at(13), textureCollection->at(14), textureCollection->at(15));
	

	putEverythingAtFirst();
}
Ecosystem::~Ecosystem(){
	
	delete ground;
	ground = NULL;

	delete powerPoles;
	powerPoles = NULL;
	
	delete bridges;
	bridges = NULL;
	
	delete pools;
	pools = NULL;
	

	delete reflectors;
	reflectors = NULL;
	delete lasers;
	lasers = NULL;
	delete destination;
	destination = NULL;
	delete monsters;
	monsters = NULL;
	delete gearKillers;	
	gearKillers=NULL;
	delete trees;
	trees = NULL;
	delete bushes;
	bushes = NULL;
	
	mover = NULL;
	world= NULL;
	textureCollection  = NULL;
	map = NULL;
	soundCollection = NULL;
}
void Ecosystem::display(const Camera&camera){
	updateLandscape(camera.getPosition().x);
	
	ground->display(camera);
	trees->display(camera);
	bushes->display(camera);
	
	monsters->display(camera);
	
	destination->display(camera);
	lasers->display(camera);
	reflectors->display(camera);
	
	powerPoles->display(camera);
	
	bridges->display(camera);
	pools->display(camera);
	gearKillers->display(camera);
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
		
	
	//bushes
	else if (ID >= '3' && ID <= '6') {
		bushes->plant(x, y - ground->getSize().h*0.5f, UtilityFunctions::charToInt(ID) - 3,leftOrRight);
	}
	//trees
	else if (ID == '7') {
		trees->plant(x, y - ground->getSize().h *0.5f, UtilityFunctions::charToInt(ID) - 7,leftOrRight);
	}
	
	//gear killer machines
	else if (ID == 'k') {
		gearKillers->addNewKiller(x, y + ground->getSize().h, 1,leftOrRight);

	}

	else if (ID == 'l') {
		gearKillers->addNewKiller(x, y + ground->getSize().h, 2,leftOrRight);
	}
	else if (ID == 'm') {
		gearKillers->addNewKiller(x, y + ground->getSize().h, 3,leftOrRight);
	}
	else if(ID=='8')
		monsters->putNewMonster(x,y,x);
	else if(ID=='n')
		destination->addNewDestination(x,y+ground->getSize().h/2);
	else if(ID=='9')
		lasers->addNewLaser(x,y);
	else if (ID == 'a'|| ID == 'b'|| ID == 'c'|| ID == 'd')
		reflectors->addNewReflector(x,y,(int)ID);



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

	else if (ID == 'o') {
		pools->addPoolMark(x, y, ground->getSize().w / 2, 3);
		ground->setPointerTo(pools->getNewPoolMark());
	}

	//need 2 marks
	else if (ID == 'i') {
	}
	else if (ID == 'j') {
	}



}

void Ecosystem::delEverything(char ID,bool leftOrRight){
	if(ID=='0'||ID=='1'||ID=='2')
		ground->delBrick(leftOrRight);
	if(ID=='k')
		gearKillers->del(leftOrRight);
	if(ID>='3'&&ID<='6')
		bushes->del(leftOrRight);
	if(ID=='7')
		trees->del(leftOrRight);
	
	else if(ID=='n')
		destination->del(leftOrRight);
	else if(ID=='9')
		lasers->del(leftOrRight);
	else if (ID == 'a'|| ID == 'b'|| ID == 'c'|| ID == 'd')
		reflectors->del(leftOrRight);

}












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
				delEverything(ID,false);

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
				delEverything(ID,true);
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
				if (!(ID == '0' || ID == '1' || ID == '2'))
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

