#pragma once
//Implementare scenariu de tip Singleton
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <string_view>
#include "Character.h"
#include "Interactable.h"

class AInteractableObj;
class IInteractable;
class DungeonMaster;

constexpr int nrRooms = 5;
constexpr int randInterPool = 3;


class Room
{
private:
	std::string name;
	std::string description;
	std::vector<std::string> objects;
	std::vector<IInteractable*> interactables;
public:
	Room() = default;
	
	std::string getName() const;
	void printRoom();
	void generateRandomEnemy();
	friend DungeonMaster;
};

class DungeonMaster
{
private:
	Player player;
	static DungeonMaster *dungeonMaster;
	Room *currentRoom;
	std::array <Room *, nrRooms> rooms;

	using sBoolPair = std::pair <std::string, bool>;
	std::array <sBoolPair, 8> randomEnemyNames;
	std::array <sBoolPair, 8> randomRoomNames;
	std::array <sBoolPair, 8> randomRoomDescription;
	std::array <IInteractable*, randInterPool> randomInteractables;  //without door and portal because they are added separatly
	std::array <int, 30> valueModifierTable;

	Room *generateRandomRoom();
	void createDungeon();
	std::string getRandomEnemyName();
	std::string getRandomRoomName();
	std::string getRandomRoomDescription();
	
	void win();
	void createDoors();
	void genInteracteblesPool();
	void genRoomNamesPool();
	void genEnemyNamesPool();
	void genRoomDescrPool();
	DungeonMaster() = default;
	DungeonMaster(DungeonMaster&) = default;
	void operator=(const DungeonMaster &) = delete;

public:
	static void destroyInstance();
	static DungeonMaster *getInstance();
	int getValueModifier(int index) { return valueModifierTable[index]; }
	void createPlayer();
	void playGame();
	void fight(Player &player, Enemy &enemy);
	friend void Room::generateRandomEnemy();
	friend void Enemy::randomizeValues();
	friend bool Teleporter::interact(Player&);
};
inline DungeonMaster *DungeonMaster::dungeonMaster = nullptr;