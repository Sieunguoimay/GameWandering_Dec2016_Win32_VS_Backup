#pragma once
#ifndef MAP_H
#define MAP_H
#include"globalheader.h"
typedef std::vector<std::vector<char>*> Matrix;
class Map
{
public:
	Map();
	~Map();
	void setup();
	void reset();
	const int getSize()const;
	const std::vector<char>*getMapData(const int index)const;
	const std::vector<char>*getMapDataOnTheRight()const;
	const std::vector<char>*getMapDataOnTheLeft()const;


	int getNumberBricks()const;
	int getFirstIndex()const;
	int getLastIndex();
	void increaseIndex();
	void decreaseIndex();
	int next();
	void setMapLevel(int index);
	int getMapLevelSaved();
	int getCurrentMapLevel()const;
protected:
	void writeCurrentMapIndexToFile();
	int loadCurrentMapIndex();
	std::vector<std::vector<char>*>*loadDataFromFile(std::string path);
	LinkedList<Matrix*>*mapContainer;
	Node<Matrix*>*currentMap;
	int firstIndex;
	int numberBrickBeingDisplayed;
	int currentMapLevel;
	int maxSavedMapLevel;
};

#endif // !MAP_H
