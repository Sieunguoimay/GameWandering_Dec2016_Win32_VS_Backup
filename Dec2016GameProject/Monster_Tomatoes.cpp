#include "Monster_Tomatoes.h"




Monster_Tomatoes::Monster_Tomatoes(b2World * world)
	:world(world)
{
	monsterContainer = new LinkedList<ObjectData*>();
	textureContainer = new std::vector<Texture*>();
	explosionContainer = new LinkedList<Explosion*>();
	target = new Vector2D();
}


Monster_Tomatoes::~Monster_Tomatoes()
{
	for (Node<ObjectData*>*iter = monsterContainer->getFirstNode(); iter != monsterContainer->tail; iter = iter->next) {
		delete ((MonsterTomato*)(iter->data->pointer));
		iter->data->pointer = NULL;
		delete iter->data;
		iter->data = NULL;
	}
	delete monsterContainer;
	monsterContainer = NULL;
	soundExplosion = NULL;
	soundSwallow = NULL;

	for (Node<ObjectData*>*iter = monsterContainer->getFirstNode(); iter != monsterContainer->tail; iter = iter->next)
	{
		delete iter->data;
		iter->data = NULL;
	}
	delete explosionContainer;
	explosionContainer = NULL;

	delete textureContainer;//that's why we don't delete each element
	textureContainer = NULL;
	delete target;
	target = NULL;
	delete size;
	size = NULL;
}

void Monster_Tomatoes::reset()
{
	for (Node<ObjectData*>*iter = monsterContainer->getFirstNode(); iter != monsterContainer->tail; iter = iter->next) {
		delete ((MonsterTomato*)(iter->data->pointer));
		iter->data->pointer = NULL;
		delete iter->data;
		iter->data = NULL;
		iter = monsterContainer->delNode(iter);
	}
}

void Monster_Tomatoes::setup(Texture * texture1, Texture*texture2, Mover*mover, Sound*soundExplosion, Sound*soundSwallow)
{
	this->soundSwallow = soundSwallow;
	this->soundExplosion = soundExplosion;
	this->mover = mover;
	textureContainer->push_back(texture1);//store it to use inside only
	textureContainer->push_back(texture2);
	size = new Size(80.0f, 80.0f);
}

void Monster_Tomatoes::display(const Camera & camera)
{
	//monster container is a linked
	for (Node<ObjectData*>*iter = monsterContainer->getFirstNode(); iter != monsterContainer->tail; iter = iter->next) {
		
		((MonsterTomato*)(iter->data->pointer))->seek(mover);
		((MonsterTomato*)(iter->data->pointer))->display(camera);

		//when will a monster is dead?
		//-when it recieves 3 messages from brick, from player
		// and from itself - does it fall off the screen or not
		if (((MonsterTomato*)(iter->data->pointer))->isDead()) {
			//in the case receive message from non-mover, then don't create a explosion. ok
			if (!(
				((MonsterTomato*)(iter->data->pointer))->isItTimeToDestroy()
				||
				((MonsterTomato*)(iter->data->pointer))->isOutOfScreen()))
				putNewExplosion(((MonsterTomato*)(iter->data->pointer))->getPosition());
			

			delete ((MonsterTomato*)(iter->data->pointer));
			iter->data->pointer = NULL;
			delete iter->data;
			iter->data = NULL;
			iter = monsterContainer->delNode(iter);
		}
	}

	for (Node<Explosion*>*iter = explosionContainer->getFirstNode(); iter != explosionContainer->tail; iter = iter->next) {
		iter->data->display(camera);
		if (iter->data->isDead()) {
			delete iter->data;
			iter->data = NULL;
			iter = explosionContainer->delNode(iter);
		}
	}
}

void Monster_Tomatoes::putNewMonster(float x, float y, float ID)
{
	MonsterTomato*newMonster = new MonsterTomato(world);
	newMonster->setup(
		x - size->w / 2, y - size->h, size,
		textureContainer->at(0), textureContainer->at(1),
		soundExplosion,soundSwallow);
	if (newMonster) {//to be safe
		ObjectData*newMonsterData = new ObjectData((char)ID, newMonster);
		if (newMonsterData)//to be safe
			monsterContainer->addFirst(newMonsterData);
	}
}

bool Monster_Tomatoes::checkID(float ID)
{
	for (Node<ObjectData*>*iter = monsterContainer->getFirstNode(); iter != monsterContainer->tail; iter = iter->next)
		if (iter->data->ID == (char)ID)
			return true;
	return false;
}

MonsterTomato * Monster_Tomatoes::getNewMonster()
{
	return (MonsterTomato *)(monsterContainer->getFirstNode()->data->pointer);
}

void Monster_Tomatoes::putNewExplosion(Vector2D*position)
{
	Explosion*newExplosion = new Explosion();
	newExplosion->setup(new Vector2D(position),//copy it to new data location 
		textureContainer->at(1), soundExplosion);//remember the order of each element =(
	explosionContainer->addFirst(newExplosion);	
}

