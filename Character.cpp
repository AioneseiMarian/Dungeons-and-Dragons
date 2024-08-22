#include "Character.h"
#include "Interactable.h"
#include "DungeonMaster.h"
#include "Exception.h"
#include "Logger.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string_view>


void Character::addCAbility(CombatAbility cAbil)
{
	bool cond = true;
	std::for_each(cAbilities.begin(), cAbilities.end(), [&cond, cAbil](CombatAbility c) {c.cAbility == cAbil.cAbility ? cond = false : 1; });
	if (cond)
		cAbilities.push_back(cAbil);
}

using namespace std::string_literals;
std::string Character::getRace()
{
	switch (race)
	{
	case Race::HUMAN:
		return "Human"s;
		break;
	case Race::GNOME:
		return "Gnome"s;
		break;
	case Race::ELF:
		return "Elf"s;
		break;
	default:
		return "???"s;
		break;
	}
}

Abilities::Abilities(int a)
{
	std::for_each(abilitiesArray.begin(), abilitiesArray.end(), [a](int& i) {i = a; });

}

Abilities Abilities::operator=(Abilities a)
{
	abilitiesArray = a.abilitiesArray;
	return *this;
}

Abilities Abilities::operator+=(Abilities a)
{
	for (int i = 0; i < MAX_ABILITY; ++i)
		abilitiesArray[i] += a.abilitiesArray[i] - 1;
	return *this;
}

int Character::rollDice(AbilityEnum ability) const
{
	int value = rand() % 20 + 1;
	std::cout << "Rolled " << value << " + " 
		<< DungeonMaster::getInstance()->getValueModifier(abilities.abilitiesArray[ability]) 
		<< " = " 
		<< value + DungeonMaster::getInstance()->getValueModifier(abilities.abilitiesArray[ability]) 
		<< std::endl;
	value += DungeonMaster::getInstance()->getValueModifier(abilities.abilitiesArray[ability]);
	if (value < 0)
		return 0;
	return value;
}

bool Character::takeDamage(int damage)
{
	hp -= damage;
	return hp <= 0;
}

using namespace std::string_literals;
std::string Character::getClass() const
{
	switch (clasa)
	{
	case Class::FIGHTER:
		return "Fighter"s;
		break;
	case Class::SORCERER:
		return "Sorcerer"s;
		break;
	case Class::BARD:
		return "Bard"s;
		break;
	default:
		throw Exception("Wrong Class");
		break;
	}
}

void Character::setName(const std::string& name)
{
	this->name = name;
}

std::string Character::getRace() const
{
	switch (race)
	{
	case Race::HUMAN:
		return "Human"s;
		break;
	case Race::GNOME:
		return "Gnome"s;
		break;
	case Race::ELF:
		return "Elf"s;
		break;
	default:
		throw Exception("Wrong Race");
		break;
	}
}

std::string Character::getBackground() const
{
	switch (background)
	{
	case Background::THIEF:
		return "Thief"s;
		break;
	case Background::SOLDIER:
		return "Soldier"s;
		break;
	case Background::ACOLYTE:
		return "Acolyte"s;
		break;
	default:
		throw Exception("Wrong Background");
		break;
	}
}

void Character::setClass(Class c)
{
	clasa = c;
	switch (c)
	{
	case Class::FIGHTER:
		cAbilities.push_back({ CombatAbilityKey::SWORD, 4, 5, STRENGTH });
		cAbilities.push_back({ CombatAbilityKey::PUNCH, 2, 3, STRENGTH });
		abilities.abilitiesArray[AbilityEnum::STRENGTH] += 4;
		abilities.abilitiesArray[AbilityEnum::CONSTITUTION] += 5;
		break;
	case Class::SORCERER:
		cAbilities.push_back({ CombatAbilityKey::FIREBALL, 4, 5, INTELLIGENCE });
		cAbilities.push_back({ CombatAbilityKey::PYROBLAST, 2, 3, INTELLIGENCE });
		abilities.abilitiesArray[AbilityEnum::INTELLIGENCE] += 4;
		abilities.abilitiesArray[AbilityEnum::CHARISMA] += 5;
		break;
	case Class::BARD:
		cAbilities.push_back({ CombatAbilityKey::BOW, 4, 5, DEXTERITY });
		cAbilities.push_back({ CombatAbilityKey::AXE, 2, 3, DEXTERITY });
		abilities.abilitiesArray[AbilityEnum::DEXTERITY] += 4;
		abilities.abilitiesArray[AbilityEnum::CHARISMA] += 5;
		break;
	default:
		throw Exception("Invalid class");
		break;
	}
}

int Character::getMaxHp() const
{
	return maxHp;
}

int Character::getHp() const
{
	return hp;
}

void Character::setRace(Race r)
{
	race = r;
	switch (r)
	{
	case Race::HUMAN:
		abilities.abilitiesArray[AbilityEnum::STRENGTH] += 1;
		abilities.abilitiesArray[AbilityEnum::DEXTERITY] += 1;
		abilities.abilitiesArray[AbilityEnum::CONSTITUTION] += 1;
		abilities.abilitiesArray[AbilityEnum::INTELLIGENCE] += 1;
		abilities.abilitiesArray[AbilityEnum::WISDOM] += 1;
		abilities.abilitiesArray[AbilityEnum::CHARISMA] += 1;
		break;
	case Race::GNOME:
		abilities.abilitiesArray[AbilityEnum::DEXTERITY] += 2;
		abilities.abilitiesArray[AbilityEnum::INTELLIGENCE] += 3;
		break;
	case Race::ELF:
		abilities.abilitiesArray[AbilityEnum::DEXTERITY] += 3;
		abilities.abilitiesArray[AbilityEnum::INTELLIGENCE] += 2;
		break;
	default:
		throw Exception("Invalid race");
		break;	
	}
}

void Character::setBackground(Background b)
{
	background = b;
	switch (b)
	{
	case Background::SOLDIER:
		abilities.abilitiesArray[AbilityEnum::STRENGTH] += 2;
		abilities.abilitiesArray[AbilityEnum::CONSTITUTION] += 2;
		break;
	case Background::THIEF:
		abilities.abilitiesArray[AbilityEnum::DEXTERITY] += 2;
		abilities.abilitiesArray[AbilityEnum::CHARISMA] += 2;
		break;
	case Background::ACOLYTE:
		abilities.abilitiesArray[AbilityEnum::WISDOM] += 2;
		abilities.abilitiesArray[AbilityEnum::INTELLIGENCE] += 2;
		break;
	default:
		throw Exception("Invalid background");
		break;
	}
}

void Character::setAbility(AbilityEnum a, int value)
{
	abilities.abilitiesArray[a] = value;
}

std::string Character::getName() const
{
	return name;
}

std::ostream &operator<<(std::ostream &os, const AbilityEnum &ability)
{
	switch (ability)
	{
	case AbilityEnum::STRENGTH:
		os << "Strength";
		break;
	case AbilityEnum::DEXTERITY:
		os << "Dexterity";
		break;
	case AbilityEnum::CONSTITUTION:
		os << "Constitution";
		break;
	case AbilityEnum::INTELLIGENCE:
		os << "Intelligence";
		break;
	case AbilityEnum::WISDOM:
		os << "Wisdom";
		break;
	case AbilityEnum::CHARISMA:
		os << "Charisma";
		break;
	default:
		throw Exception("Invalid ability");
		break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const CombatAbilityKey &combatAbility)
{
	switch (combatAbility)
	{
	case CombatAbilityKey::FIREBALL:
		os << "Fireball";
		break;
	case CombatAbilityKey::PYROBLAST:
		os << "Pyroblast";
		break;
	case CombatAbilityKey::SWORD:
		os << "Sword";
		break;
	case CombatAbilityKey::PUNCH:
		os << "Punch";
		break;
	case CombatAbilityKey::AXE:
		os << "Axe";
		break;
	case CombatAbilityKey::BOW:
		os << "Bow";
		break;
	default:
		throw Exception("Invalid ability");
		break;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const CombatAbility &combatAbility)
{
	os << combatAbility.cAbility << " Damage: " 
		<< combatAbility.damage << ", DC: " << combatAbility.difficultyClass 
		<< ", Ability: " << combatAbility.abilityChecked;
	return os;
}

std::ostream &operator<<(std::ostream &os, const Abilities &abilities)
{
	os << "Strength: " << abilities.abilitiesArray[AbilityEnum::STRENGTH] << std::endl;
	os << "Dexterity: " << abilities.abilitiesArray[AbilityEnum::DEXTERITY] << std::endl;
	os << "Constitution: " << abilities.abilitiesArray[AbilityEnum::CONSTITUTION] << std::endl;
	os << "Intelligence: " << abilities.abilitiesArray[AbilityEnum::INTELLIGENCE] << std::endl;
	os << "Wisdom: " << abilities.abilitiesArray[AbilityEnum::WISDOM] << std::endl;
	os << "Charisma: " << abilities.abilitiesArray[AbilityEnum::CHARISMA] << std::endl;
	return os;
}

void Player::addAbilitiesValues(Abilities ab)
{
	abilities += ab;
}

void Player::printStatistics()
{
	
	
	
}
