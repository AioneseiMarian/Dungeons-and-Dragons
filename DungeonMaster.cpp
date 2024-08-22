#include "DungeonMaster.h"
#include "Exception.h"
#include "Interactable.h"
#include "Logger.h"
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>

constexpr int length = 8;


using namespace std::string_literals;
using namespace std::string_view_literals;

void DungeonMaster::destroyInstance()
{
	if (dungeonMaster != nullptr)
	{
		delete dungeonMaster;
		dungeonMaster = nullptr;
	}
}

DungeonMaster *DungeonMaster::getInstance()
{
    if (dungeonMaster == nullptr)
    {
        dungeonMaster = new DungeonMaster();     //Camera de inceput a jocului
        dungeonMaster->valueModifierTable[0] = -5;
        for (int i = 1; i < 28; i += 2)                        //Initializarea tabelului de modificatori
        {
            dungeonMaster->valueModifierTable[i]
                = dungeonMaster->valueModifierTable[i + 1]
                = dungeonMaster->valueModifierTable[i - 1] + 1;
        }
        dungeonMaster->valueModifierTable[29] = dungeonMaster->valueModifierTable[28] + 1;
        if (dungeonMaster->valueModifierTable[29] != 10)
            throw Exception("Eroare initializare valueModifier array");

        dungeonMaster->genEnemyNamesPool();
        dungeonMaster->genRoomNamesPool();
		dungeonMaster->genRoomDescrPool();
        dungeonMaster->genInteracteblesPool();
        
        dungeonMaster->createDungeon();   //generates map

        
    }
    return dungeonMaster;
}

Room *DungeonMaster::generateRandomRoom()
{
    Room *room = new Room;
	room->name = static_cast<std::string> (getRandomRoomName());
	std::vector<std::string> objects;
	std::vector<IInteractable*> interactables;
	for (int i = 0; i < randInterPool; i++)         //Adds random interactables without doors and portals
	{
        if (rand() % 3 < 2)
            interactables.push_back(randomInteractables[i]);
	}
    
    for (auto it : interactables)
    {
        it->randomizeValues();
		objects.push_back(static_cast<std::string>(it->getName()));
    }
    


    room->objects = objects;
    room->interactables = interactables;
    room->description = getRandomRoomDescription();
    
    if (rand() % 2 == 0)
        room->generateRandomEnemy();
    
	// return value
    return room;
    
}

void DungeonMaster::createDungeon()
{
	for (int i = 0; i < nrRooms; i++)
	{
        rooms[i] = generateRandomRoom();
	}
    
    createDoors();
    currentRoom = rooms[0];
}


std::string DungeonMaster::getRandomEnemyName()
{
    int random;
	do
	{
        random = rand();
		if (randomEnemyNames[random% randomEnemyNames.size()].second == false)
		{
			randomEnemyNames[random % randomEnemyNames.size()].second = true;
			return randomEnemyNames[random% randomEnemyNames.size()].first;
		}
	} while (1);
}

std::string DungeonMaster::getRandomRoomName()
{
    int random;
	do
	{   random = rand();
		if (randomRoomNames[random % randomRoomNames.size()].second == false)
		{
			randomRoomNames[random % randomRoomNames.size()].second = true;
			return randomRoomNames[random % randomRoomNames.size()].first;
		}
	} while (1);
}

std::string DungeonMaster::getRandomRoomDescription()
{
    int random;
    do
    {
        random = rand();
		if (randomRoomDescription[random % randomRoomDescription.size()].second == false)
		{
			randomRoomDescription[random % randomRoomDescription.size()].second = true;
			return randomRoomDescription[random % randomRoomDescription.size()].first;
		}
	} while (1);
}

void DungeonMaster::win()
{
	std::cout << "\n\n\nYou won the game!" << std::endl;
    delete dungeonMaster;
    Logger::destroyInstance();
    exit(0);
}

void DungeonMaster::createDoors()
{
    for (int i = 0; i < nrRooms - 1; i++)
    {
        Teleporter *telep;
        if (rand() % 3 < 2)
        {
            std::string name = "Door to " + rooms[i + 1]->name;
            telep = new Teleporter(name, DOOR, 0, STRENGTH); //trash values for dc and ability
        }
        else
        {
            std::string name = "Portal to " + rooms[i + 1]->name;
            telep = new Teleporter(name, PORTAL, 0, STRENGTH);  //trash values
        }
        telep->randomizeValues();   //random values added
        telep->setDestinationRoom(rooms[i + 1]);
		rooms[i]->interactables.push_back(telep);
		rooms[i]->objects.push_back(telep->getName());
    }
    Teleporter *telep = new Teleporter("Portal to VICTORY", PORTAL, 0, STRENGTH);
	telep->randomizeValues();
	telep->setDestinationRoom(nullptr);
	rooms[nrRooms - 1]->interactables.push_back(telep);
	rooms[nrRooms - 1]->objects.push_back(telep->getName());
}

void DungeonMaster::genInteracteblesPool()
{
    //initialize with trash values
    randomInteractables[0] = new Booster{ "Book", BOOK, 0, STRENGTH, {0} };
    randomInteractables[1] = new Booster{ "Chest", CHEST, 0, STRENGTH, {0} };
	randomInteractables[2] = new Weapon{ "Axe", AXE, 0, STRENGTH };
    
}

void DungeonMaster::genRoomNamesPool()
{
    std::array<std::string, length> roomNames = { "Whispering Chamber"s,
                                                               "Fiery Forge"s,
                                                               "Frozen Crypt"s,
                                                               "Echoing Catacombs"s,
                                                               "Solar Sanctum"s,
                                                               "Phantom Parlor"s,
                                                               "Verdant Alcove"s,
                                                               "Thunderous Vault"s };
    for (int i = 0; i < length; i++)
        dungeonMaster->randomRoomNames[i] = { roomNames[i], false };
}   

void DungeonMaster::genEnemyNamesPool()
{
    std::array<std::string, length> names = { "Acererak"s,
                                                "Baphomet"s,
                                                "Demogorgon"s,
                                                "Fraz-Urb'luu"s,
                                                "Graz'zt"s,
                                                "Juiblex"s,
                                                "Orcus"s,
                                                "Zuggtmoy"s };
    for (int i = 0; i < length; i++)
        randomEnemyNames[i] = { names[i], false };
}

void DungeonMaster::genRoomDescrPool()
{
    std::array<std::string, length> descriptions;
    
	std::ifstream file("roomDesc.txt");
	if (!file.is_open())
		throw Exception("Eroare la deschiderea fisierului roomDesc.txt");

    for (int i = 0; i < length; i++)
    {
        std::string content;
        std::string line;

        while (std::getline(file, line)) {
            if (line.find("~") != std::string::npos) {
                // Found the delimiter (~), break the loop
                break;
            }

            content += line + '\n';  // Include newline characters
        }
		descriptions[i] = content;
    }

	for (int i = 0; i < length; i++)
		dungeonMaster->randomRoomDescription[i] = { descriptions[i], false };
}

void DungeonMaster::createPlayer()
{
	std::string name;
	std::cout << "\n\nEnter the player's name:\n";
	std::cin >> name;
	player.setName(name);
    
    std::cout << "Choose a class:\n";
	std::cout << "1) Fighter\n";
	std::cout << "2) Sorcerer\n";
	std::cout << "3) Bard\n";
	int choice;
	std::cin >> choice;
	while (choice < 1 || choice > 3)
	{
		std::cout << "Invalid choice! Try again!\n";
		std::cin >> choice;
	}
	player.setClass(static_cast<Class>(choice - 1));
	std::cout << "You chose " << player.getClass() << "\n\n";
    
    std::cout << "Choose a race:\n";
    std::cout << "1) Human\n";
	std::cout << "2) Gnome\n";
	std::cout << "3) Elf\n";
	std::cin >> choice;
	while (choice < 1 || choice > 3)
	{
		std::cout << "Invalid choice! Try again!\n";
		std::cin >> choice;
	}
	player.setRace(static_cast<Race>(choice - 1));
    std::cout << "You chose " << player.getRace() << "\n\n";

    std::cout << "Choose a background:\n";
	std::cout << "1) Thief\n";
    std::cout << "2) Soldier\n";
	std::cout << "3) Acolyte\n";
	std::cin >> choice;
	while (choice < 1 || choice > 3)
	{
		std::cout << "Invalid choice! Try again!\n";
		std::cin >> choice;
	}
	player.setBackground(static_cast<Background>(choice - 1));
    std::cout << "You chose " << player.getBackground() << "\n\n\n";
    
	Logger::getInstance()->logPlayerStats(player);
}

void DungeonMaster::playGame()
{
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Welcome to the Dungeon!\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
              << "You are a brave adventurer who has decided to explore the depths of a dungeon.\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
              << "You will encounter many dangers, but also many treasures.\n\n";
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
              << "Good luck!\n\n\n";
    
    createPlayer();
    
    std::cout << "Your stats are:\n";
	Logger::getInstance()->printPlayerStats();
	std::cout << "You enter the dungeon.\n";
    
    while (true)
    {
        currentRoom->printRoom();
        int answer;
        std::cin >> answer;
        while (answer < 1 || answer > currentRoom->interactables.size())
        {
            std::cout << "Invalid choice! Try again!\n";
            std::cin >> answer;
        }

        InteractableKey interactedKey = currentRoom->interactables[answer - 1]->getInteractableKey();
        bool worked = currentRoom->interactables[answer - 1]->interact(player);
        
        if (worked && interactedKey != DOOR      //altfel imi va sterge "interactionabilele" din camera urmatoare dupa ce ma voi muta in camera urmatoare 
                   && interactedKey != PORTAL)
        {
            currentRoom->interactables.erase(currentRoom->interactables.begin() + answer - 1);
			currentRoom->objects.erase(currentRoom->objects.begin() + answer - 1);
        }
    }
    
}

void DungeonMaster::fight(Player &player, Enemy &enemy)
{
    while (player.getHp() && enemy.getHp())
    {
        int answer;
        std::cout << "Choose an combat ability:\n";
        for (int i = 0; i < player.cAbilities.size(); i++)
        {
            std::cout << i + 1 << ") ";
            std::cout << player.cAbilities[i].cAbility << ' ' << player.cAbilities[i].difficultyClass
                << ' ' << player.cAbilities[i].abilityChecked << std::endl;
        }
        std::cin >> answer;
		while (answer < 1 || answer > player.cAbilities.size())
		{
			std::cout << "Invalid choice! Try again!\n";
			std::cin >> answer;
		}
        std::cout << "\nYou attack:\n";
        int diceValue = player.rollDice(player.cAbilities[answer - 1].abilityChecked);
		if (diceValue >= player.cAbilities[answer - 1].difficultyClass)
		{
            std::cout << "\nYou hit the enemy!\n";
            enemy.takeDamage(player.cAbilities[answer - 1].damage);
            if(enemy.getHp()<=0)
			{
				break;
			}

			std::cout << "The enemy has " << enemy.getHp() << " hp left!\n\n";
		}
		else
			std::cout << "You missed!\n\n";
		std::cout << "The enemy attacks!\n";
		CombatAbility enemyAbility = enemy.cAbilities[rand() % enemy.cAbilities.size()];
		diceValue = enemy.rollDice(enemyAbility.abilityChecked);
        
		if (diceValue >= enemyAbility.difficultyClass)
		{
			std::cout << "\nThe enemy hit you!\n";
			player.takeDamage(enemyAbility.damage);
            if (player.getHp()<=0)
            {
                break;
            }
			std::cout << "You have " << player.getHp() << " hp left!\n\n";
		}
		else
			std::cout << "The enemy missed!\n\n";
	}
    if (player.getHp() <= 0)
    {
        std::cout << "\n\tYou died!\nGame over!";
		exit(0);
    }
	else
	{
		std::cout << "\n\tYou killed the enemy!\n" << "All your hp has been restored!\n\n";
        player.hp = player.maxHp;
	}
}

std::string Room::getName() const
{
    return name;
}

void Room::printRoom()
{
    std::cout << description;
    std::cout << "\n\nYou are in " << name << ".\nYou can observe:\n\n";

    auto length = std::ssize(objects);
    for (std::size_t i = 0; i < length; i++)
    {
        std::cout << objects[i];
        if (i != length - 1)
            std::cout << ", ";
        else
            std::cout << ".\n\n\n";
    }

    std::cout << "The actions that you can make are:" << std::endl;
    auto len = interactables.size();
    for (std::size_t i = 0; i < len; i++)
    {
        std::cout << i + 1 << ") ";
        switch (interactables[i]->getInteractableKey())
        {
		case InteractableKey::DOOR:
            std::cout << "Open the door";
			break;
		case InteractableKey::CHEST:
            std::cout << "Open the chest";
            break;
        case InteractableKey::ENEMY:
            std::cout << "Fight " << interactables[i]->getName();
            break;
        case InteractableKey::AXE:
            std::cout << "Collect Axe";
            break;
        case InteractableKey::PORTAL:
            std::cout << "Open the portal";
            break;
        case InteractableKey::BOOK:
            std::cout << "Read book";
            break;
        default:
            exit(-1);
        }
        interactables[i]->printDifficulty();
        

    }
    std::cout << std::endl << "Choose a number!" << std::endl;
}

void Room::generateRandomEnemy()
{
    std::string name;
	name = DungeonMaster::getInstance()->getRandomEnemyName();
    std::string message = "Fight " + name;
    
    Enemy* enemy = new Enemy(message, name);

    Background randomBackground = static_cast<Background>(rand()
        % static_cast<int>(Background::MAX_BACKGROUND));
	enemy->setBackground(randomBackground);
    
    Race randomRace = static_cast<Race>(rand()
        % static_cast<int>(Race::MAX_RACE));
	enemy->setRace(randomRace);
    
	Class randomClass = static_cast<Class>(rand()
		% static_cast<int>(Class::MAX_CLASS));
	enemy->setClass(randomClass);

    objects.push_back(enemy->getName());
    interactables.push_back(enemy);
}



