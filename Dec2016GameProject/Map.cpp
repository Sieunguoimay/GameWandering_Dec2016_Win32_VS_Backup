#include "Map.h"



Map::Map()
{
	mapContainer = new LinkedList<Matrix*>();
}


Map::~Map()
{
	writeCurrentMapIndexToFile();
	for (Node<Matrix*>*iter = mapContainer->getFirstNode(); iter != mapContainer->tail; iter = iter->next)
	{
		for (size_t i = 0; i < iter->data->size(); i++)
		{
			delete iter->data->at(i);
			iter->data->at(i) = NULL;
		}
		delete iter->data;
		iter->data = NULL;
	}
	delete mapContainer;
	currentMap = NULL;
}	

void Map::setup()
{
	maxSavedMapLevel = loadCurrentMapIndex();
	firstIndex = 0;
	numberBrickBeingDisplayed = 50;
	currentMapLevel = 0;

	mapContainer->addLast(loadDataFromFile("map.txt"));
	mapContainer->addLast(loadDataFromFile("map_2.txt"));
	mapContainer->addLast(loadDataFromFile("map_3.txt"));
	mapContainer->addLast(loadDataFromFile("map_4.txt"));
	setMapLevel(0);
}

void Map::reset()
{
	firstIndex = 0;
	numberBrickBeingDisplayed = 50;
}

const int Map::getSize() const
{
	return currentMap->data->size();
}

const std::vector<char>* Map::getMapData(const int index) const
{
	// TODO: insert return statement here
	return currentMap->data->at(index);
}

const std::vector<char>* Map::getMapDataOnTheRight() const
{
	return getMapData( firstIndex+numberBrickBeingDisplayed);
}

const std::vector<char>* Map::getMapDataOnTheLeft() const
{
	return getMapData(firstIndex);
}


int Map::getFirstIndex() const
{
	return this->firstIndex;
}

int Map::getLastIndex()
{
	return firstIndex+numberBrickBeingDisplayed;
}


int Map::getNumberBricks() const
{
	return this->numberBrickBeingDisplayed;
}

void Map::increaseIndex()
{
	firstIndex++;
}

void Map::decreaseIndex()
{
	firstIndex--;
}

int Map::next()
{
	if (currentMap->next != mapContainer->tail)
	{
		currentMap = currentMap->next;//jump to next map data
		currentMapLevel++;

		if (currentMapLevel > maxSavedMapLevel)
			maxSavedMapLevel = currentMapLevel;
	}
	return currentMapLevel;
}

void Map::setMapLevel(int index)
{
	currentMapLevel = index;
	currentMap = mapContainer->getFirstNode();
	for (int i = 0; i < currentMapLevel; i++)
		if (currentMap->next != mapContainer->tail)
			currentMap = currentMap->next;//jump to next map data
}

int Map::getMapLevelSaved()
{
	return maxSavedMapLevel;
}

int Map::getCurrentMapLevel() const
{
	return currentMapLevel;
}

void Map::writeCurrentMapIndexToFile()
{
	SDL_RWops*io = SDL_RWFromFile("data.txt", "wb");
	if (io != NULL) {
		if (SDL_RWwrite(io, &maxSavedMapLevel, sizeof(maxSavedMapLevel), 1) > 0) {
			//SDL_Log("Game saved. %d\n", maxSavedMapLevel);
		}
		SDL_RWclose(io);
	}
}

int Map::loadCurrentMapIndex()
{
	SDL_RWops *io = SDL_RWFromFile("data.txt", "rb");
	int data = 0;
	if (io != NULL) {
		if (SDL_RWread(io, &data, sizeof(data), 1) > 0) {
			//SDL_Log("Current Level, %d!\n", data);
		}
		SDL_RWclose(io);
	}
	return data;
}

std::vector<std::vector<char>*>*Map::loadDataFromFile(std::string path)
{
	std::vector<std::vector<char>*>*tempData = new std::vector<std::vector<char>*>();
	std::vector<char>*temp = new std::vector<char>();
	char iterator;
	//int type;

	SDL_RWops*file = SDL_RWFromFile(path.c_str(), "rb");

	if (file != NULL) {
		while (file->read(file, &iterator, sizeof(iterator), 1) > 0) {
			if (iterator == '.')
				break;//completed.
			if (iterator == '\n') {
				tempData->push_back(temp);
				temp = new std::vector<char>();
			}
			else{
				temp->push_back(iterator);
			}
		}
		file->close(file);
	}
	//else printf("NULL.\n");
	delete temp;
	temp = NULL;
	//SDL_Log("Load Map Data completed.\n");
	/*
	for (size_t i = 0; i < tempData->size(); i++)
	{
		for (size_t j = 0; j < tempData->at(i)->size(); j++)
			SDL_Log("%c", tempData->at(i)->at(j));
		SDL_Log("\n");
	}
	*/
	return tempData;
}
