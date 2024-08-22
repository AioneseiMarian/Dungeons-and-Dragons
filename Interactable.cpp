#include "Interactable.h"
#include "DungeonMaster.h"
#include "Exception.h"

using namespace std::string_literals;

void Booster::randomizeValues()
{
	difficultyClass = rand() % 15 + 1;    //random difficulty class
	checkedAbility = static_cast <AbilityEnum> (rand() % MAX_ABILITY);  //random ability to check
	
	Abilities random;
	random.abilitiesArray[rand() % MAX_ABILITY] = rand() % 3 + 1;    //chooses a random ability to boost with a random value
	abilityBooster = random;
}

Abilities Booster::getAbilities() const
{
	return abilityBooster;
}

bool Booster::interact(Player &player)
{
	int rolled = player.rollDice(checkedAbility);
	if (rolled >= difficultyClass)
	{
		std::cout << "You got:\n";
		for (int i = 0; i < MAX_ABILITY; ++i)
			if (abilityBooster.abilitiesArray[i])
				std::cout << abilityBooster.abilitiesArray[i] << " points in "
				<< static_cast <AbilityEnum> (i) << "\n";
		player.addAbilitiesValues(abilityBooster);
		std::cout << "\n\n";
		return true;
	}
	std::cout << "You failed\n\n\n";
	return false;
}

bool Teleporter::interact(Player& player)
{
	int rolled = player.rollDice(checkedAbility);
	if (rolled >= difficultyClass)
	{
		if (destinationRoom != nullptr)
		{
			DungeonMaster::getInstance()->currentRoom = destinationRoom;
			return true;
		}
		else
			DungeonMaster::getInstance()->win();
	}
	else
	{
		std::cout << "You failed\n\n\n";
		return false;
	}
}

void Teleporter::setDestinationRoom(Room *room)
{
	destinationRoom = room;
}

InteractableKey AInteractable::getInteractableKey() const
{
	return interactableKey;
}

std::string AInteractable::getName() const
{
	return getMessage();
}

std::string AInteractable::getMessage() const
{
	return message;
}

int AInteractableObj::getDifficulty()
{
	return difficultyClass;
}

void AInteractableObj::printDifficulty()
{
	std::cout << ' ' << difficultyClass << ' ' << checkedAbility << std::endl;
}

void AInteractableObj::randomizeValues()
{
	difficultyClass = rand() % 15 + 1;    //random difficulty class
	checkedAbility = static_cast <AbilityEnum> (rand() % MAX_ABILITY);  //random ability to check
}

AbilityEnum AInteractableObj::getAbilityNeeded()
{
	return checkedAbility;
}

AbilityEnum Enemy::getAbilityNeeded()
{
	return MAX_ABILITY;    //shouldn't be used;
}

bool Enemy::interact(Player &player)
{
	DungeonMaster::getInstance()->fight(player, *this);
	return true;
}

int Enemy::getDifficulty()
{
	return 0;   //shouldn't be used;
}

void Enemy::randomizeValues()
{
	Abilities random;
	random.abilitiesArray[rand() % MAX_ABILITY] = rand() % 3 + 1;    //chooses two random abilities to boost with a random value
	random.abilitiesArray[rand() % MAX_ABILITY] = rand() % 3 + 1;    
	abilities = random;
	
	hp = rand() % 4 + 4;	//random hp
	setClass(static_cast <Class> (rand() % static_cast <int> (Class::MAX_CLASS)));	//random class
	setRace(static_cast <Race> (rand() % static_cast <int> (Race::MAX_RACE)));
	setBackground(static_cast <Background> (rand() % static_cast <int> (Background::MAX_BACKGROUND)));
}

std::string Enemy::getMessage() const
{
	return "Fight "s + name;
}

std::string Enemy::getName() const
{
	return name;
}

void Enemy::printDifficulty()
{
	std::cout << "he has " << hp << " hp and is a " << getRace() << " " << getClass() << ", former " << getBackground() << std::endl;
}

bool Weapon::interact(Player &player)
{
	int rolled = player.rollDice(checkedAbility);
	if (rolled >= difficultyClass)
	{
		CombatAbility combatAbility;
		combatAbility.damage = rand() % 6 + 1;
		combatAbility.abilityChecked = checkedAbility;
		combatAbility.difficultyClass = difficultyClass;
		switch (interactableKey)
		{
		case AXE:
			combatAbility.cAbility = CombatAbilityKey::AXE;		//can add different weapons in future
			break;
		default:
			throw Exception("Invalid weapon type");
			break;
		}

		player.addCAbility(combatAbility);
		std::cout << "You got a " << getName() << " that does " << combatAbility.damage << " damage" << std::endl;

		return true;
	}
	else
	{
		std::cout << "You failed to get the " << getName() << "\n\n\n";
		return false;
	}
	
}
