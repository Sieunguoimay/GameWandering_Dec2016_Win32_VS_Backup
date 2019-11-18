#pragma once
#ifndef SOMEDEFINE_H
#define SOMEDEFINE_H

#define SQRT_2 1.41421356f
#define FPS 30.0f
#define WINDOWSIZE_W 1366.0f 
#define WINDOWSIZE_H 768.0f
#define ERROR_DISPLAY 2.0f 
#define GRAVITY 10.0f
//Game's Object IDs
#define MOVER 10
#define ECOSYSTEM 11
#define CAMERA 12
#define NUM_MAP_LEVEL 4
//#define MAX_SOUND_CHANNELS 16
struct SomeDefine
{
	//list your body's ID here:

	static const char CLUSTERID = 1;
	static const char SENSOR_ID_ON = 2;
	static const char SENSOR_ID_OFF = 3;
	static const char SURFACE_BRICK_ID = 4;
	static const char UNDERGROUND_BRICK_ID = 5;

	//index for sprites of Skin
	//belong to mover
	static const char BODY_INDEX = 0;
	static const char LEG_INDEX = 1;
	static const char EYE_INDEX = 2;
	static const char BULB_INDEX = 3;

	static const char MONSTER_TOMATO = 8;
	static const char MOVER_LIGHTNING = 9;


	static const char GEARKILLER_ID = 13;
	static const char PHOTON_LAZER_ID = 14;
	static const char REFLECTOR_ID = 15;
	static const char BIRD_ID = 16;

	static const char WATER_POOL_ID = 17;
	static const char FISH_ID = 18;
	static const char VALLEY_ID = 19;
	static const char LIGHTNING_KILLER = 20;

	static const char MOVERID = 21;
	static const char DESTINATION_ID = 22;

};
#endif // !SOMEDEFINE_H

#ifndef OBJECTDATA_H
#define OBJECTDATA_H

struct ObjectData{
	ObjectData(char ID, void*pointer) 
		:ID(ID)
	{
		this->pointer = pointer;
	}
	char ID;
	void*pointer;
};
#endif // !OBJECTDATA_H
#ifndef MAPDATA_H
#define MAPDATA_H
struct MapData {
	int type;
	std::vector<int> data;//vertical up or down relative to origin
};
#endif // !MAPDATA_H
