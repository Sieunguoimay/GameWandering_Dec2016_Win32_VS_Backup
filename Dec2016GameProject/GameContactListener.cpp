#include "GameContactListener.h"



GameContactListener::GameContactListener()
	:b2ContactListener()
{
	//SDL_Log("Created contact listener.\n");
	buoyancyController = BuoyancyController::create();
}


GameContactListener::~GameContactListener()
{
	delete buoyancyController;
	buoyancyController = NULL;
	//SDL_Log("Destroyed contact listener.\n");

}

void GameContactListener::BeginContact(b2Contact * contact)
{
	buoyancyController->BeginContact(contact);

	b2Fixture*fixtureA = contact->GetFixtureA();
	b2Fixture*fixtureB = contact->GetFixtureB();
	void* userDataA = fixtureA->GetBody()->GetUserData();
	void* userDataB = fixtureB->GetBody()->GetUserData();
	
	if (userDataA&&userDataB) {
		if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID) {
			if (((ObjectData*)userDataB)->ID == SomeDefine::SURFACE_BRICK_ID) {
				((Mover*)((ObjectData*)userDataA)->pointer)->setOnGround(true);
				return;
			}
			if (((ObjectData*)userDataB)->ID == SomeDefine::GEARKILLER_ID) {

				((Mover*)((ObjectData*)userDataA)->pointer)->die();
			}
			if (
				((ObjectData*)userDataB)->ID == SomeDefine::BIRD_ID||
				((ObjectData*)userDataB)->ID == SomeDefine::FISH_ID
				)
				((Mover*)((ObjectData*)userDataA)->pointer)->die();


			if (((ObjectData*)userDataB)->ID == SomeDefine::LIGHTNING_KILLER) {
				//printf("Call die.\n");
				((Mover*)((ObjectData*)userDataA)->pointer)->die();
				//the phenomena is : even when colision does not occur
				//this scope till passed. why?>?????????????
				//this scope is passed means that : userdataA and B is real between LIGHTNING_KILLER & MOVER
			}
			if (((ObjectData*)userDataB)->ID == SomeDefine::DESTINATION_ID) {
				((LevelManager*)((ObjectData*)userDataB)->pointer)->setNextLevel();
			}
		}
		if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID) {
			if (((ObjectData*)userDataA)->ID == SomeDefine::SURFACE_BRICK_ID) {
				((Mover*)((ObjectData*)userDataB)->pointer)->setOnGround(true);
				return;
			}
			if (((ObjectData*)userDataA)->ID == SomeDefine::GEARKILLER_ID) {
				//kill mover
				((Mover*)((ObjectData*)userDataB)->pointer)->die();
			}

			if (
				((ObjectData*)userDataA)->ID == SomeDefine::BIRD_ID||
				((ObjectData*)userDataA)->ID == SomeDefine::FISH_ID
				)
				((Mover*)((ObjectData*)userDataB)->pointer)->die();

			if (((ObjectData*)userDataA)->ID == SomeDefine::LIGHTNING_KILLER) {
				//kill mover
				//printf("Call die.\n");
				((Mover*)((ObjectData*)userDataB)->pointer)->die();
			}
			if (((ObjectData*)userDataA)->ID == SomeDefine::DESTINATION_ID) {
				((LevelManager*)((ObjectData*)userDataA)->pointer)->setNextLevel();
			}
		}



		//

		if (((ObjectData*)userDataA)->ID == SomeDefine::MOVER_LIGHTNING) {
			if (((ObjectData*)userDataB)->ID == SomeDefine::BIRD_ID) {
				((Bird*)((ObjectData*)userDataB)->pointer)->die();
			}
			
		}
		if (((ObjectData*)userDataB)->ID == SomeDefine::MOVER_LIGHTNING) {
			if (((ObjectData*)userDataA)->ID == SomeDefine::BIRD_ID) {
				((Bird*)((ObjectData*)userDataA)->pointer)->die();
			}
		}


		//
		/*
		if (((ObjectData*)userDataA)->ID == SomeDefine::LIGHTNING_KILLER) {
			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID) {
				//((Mover*)((ObjectData*)userDataB)->pointer)->die();
				printf("Call die2.%d   %d  %d\n", ((ObjectData*)userDataA)->ID, ((ObjectData*)userDataB)->ID, SomeDefine::MOVERID);
			}

		}
		if (((ObjectData*)userDataB)->ID == SomeDefine::LIGHTNING_KILLER) {
			if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID) {
				//((Mover*)((ObjectData*)userDataA)->pointer)->die();
				printf("Call die2.%d   %d  %d\n", ((ObjectData*)userDataB)->ID, ((ObjectData*)userDataA)->ID, SomeDefine::MOVERID);
			}
		}
		*/

		//Valley 
		if (((ObjectData*)userDataA)->ID == SomeDefine::VALLEY_ID) {
			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID) {
				fixturePairs.insert(std::make_pair(fixtureA, fixtureB));
			}

		}
		if (((ObjectData*)userDataB)->ID == SomeDefine::VALLEY_ID) {
			if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID) {
				fixturePairs.insert(std::make_pair(fixtureB, fixtureA));
			}
		}

	}


	//check for monster and mover 
	if (userDataA&&userDataB) {
		if (((ObjectData*)userDataA)->ID == SomeDefine::MONSTER_TOMATO) 
		{
			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID) {
				((Mover*)((ObjectData*)userDataB)->pointer)->die();
			}
			else if (((ObjectData*)userDataB)->ID == SomeDefine::MOVER_LIGHTNING) {//lightning
				((MonsterTomato*)((ObjectData*)userDataA)->pointer)->touchMover();
			}
		}

		if (
			((ObjectData*)userDataB)->ID == SomeDefine::MONSTER_TOMATO
			)
		{
			if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID) {
				((Mover*)((ObjectData*)userDataA)->pointer)->die();
			}
			else if (((ObjectData*)userDataA)->ID == SomeDefine::MOVER_LIGHTNING) {//lightning
				((MonsterTomato*)((ObjectData*)userDataB)->pointer)->touchMover();
			}
		}

		
	}

	if (userDataA&&userDataB)
	{
		//check for laser photon
		if (((ObjectData*)userDataA)->ID == SomeDefine::PHOTON_LAZER_ID) {
			if (
				!(
				((ObjectData*)userDataB)->ID == SomeDefine::PHOTON_LAZER_ID
					||
					((ObjectData*)userDataB)->ID == SomeDefine::REFLECTOR_ID
					)
				)((Photon*)((ObjectData*)userDataA)->pointer)->collapse();

			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID)
				((Mover*)((ObjectData*)userDataB)->pointer)->die();
			if (((ObjectData*)userDataB)->ID == SomeDefine::REFLECTOR_ID)
			{
				((Relector*)((ObjectData*)userDataB)->pointer)->reflect(*((Photon*)((ObjectData*)userDataA)->pointer));
			}
		}
		if (((ObjectData*)userDataB)->ID == SomeDefine::PHOTON_LAZER_ID) {
			if (
				!(
				((ObjectData*)userDataA)->ID == SomeDefine::PHOTON_LAZER_ID
					||
					((ObjectData*)userDataA)->ID == SomeDefine::REFLECTOR_ID

					)
				)
				((Photon*)((ObjectData*)userDataB)->pointer)->collapse();
			if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID)
				((Mover*)((ObjectData*)userDataA)->pointer)->die();

			if (((ObjectData*)userDataA)->ID == SomeDefine::REFLECTOR_ID) {
				((Relector*)((ObjectData*)userDataA)->pointer)->reflect(*((Photon*)((ObjectData*)userDataB)->pointer));
			}
		}
	}
}

void GameContactListener::EndContact(b2Contact * contact)
{
	buoyancyController->EndContact(contact);

	b2Fixture*fixtureA = contact->GetFixtureA();
	b2Fixture*fixtureB = contact->GetFixtureB();
	void* userDataA = fixtureA->GetBody()->GetUserData();
	void* userDataB = fixtureB->GetBody()->GetUserData();


	if (userDataA&&userDataB) {
		if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID&&
			((ObjectData*)userDataB)->ID == SomeDefine::SURFACE_BRICK_ID
			) {
			((Mover*)((ObjectData*)userDataA)->pointer)->setOnGround(false);
		}

		if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID&&
			((ObjectData*)userDataA)->ID == SomeDefine::SURFACE_BRICK_ID) {
			((Mover*)((ObjectData*)userDataB)->pointer)->setOnGround(false);
		}

		//Valley 
		if (((ObjectData*)userDataA)->ID == SomeDefine::VALLEY_ID) {
			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID) {
				fixturePairs.erase(std::make_pair(fixtureA, fixtureB));
			}
		}
		if (((ObjectData*)userDataB)->ID == SomeDefine::VALLEY_ID) {
			if (((ObjectData*)userDataA)->ID == SomeDefine::MOVERID) {
				fixturePairs.erase(std::make_pair(fixtureB, fixtureA));
			}
		}
	}
}

void GameContactListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
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
			//printf("keep track mover.\n");
			if (((ObjectData*)userDataB)->ID == SomeDefine::MOVERID)
				((ThunderCloudValley*)((ObjectData*)userDataA)->pointer)->keepTrackOfObjectInsideArea(((Mover*)((ObjectData*)userDataB)->pointer)->getPosition());
		}

		++it;
	}
	
}

void GameContactListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{


}
void GameContactListener::step()
{
	buoyancyController->step();
}
