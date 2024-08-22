#pragma once
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string_view>
#include <array>
class DungeonMaster;
class Booster;
class AInteractableObj;
class Logger;
class Player;

enum AbilityEnum
{
    STRENGTH,
    DEXTERITY,
    INTELLIGENCE,
    CHARISMA,
    CONSTITUTION,
    WISDOM,
    MAX_ABILITY
};

std::ostream &operator<<(std::ostream &os, const AbilityEnum &ability);

enum class Background
{
    THIEF,
    SOLDIER,
    ACOLYTE,
    MAX_BACKGROUND
};

enum class Race
{
    HUMAN,
    GNOME,
    ELF,
    MAX_RACE
};

enum class Class
{
    FIGHTER,    
    SORCERER,
    BARD,
	MAX_CLASS
};

enum class CombatAbilityKey
{
    FIREBALL,
    PYROBLAST,
    SWORD,
    PUNCH,
    AXE,
    BOW,
	MAX_CABILITY
};

std::ostream &operator<<(std::ostream &os, const CombatAbilityKey &ability);

struct CombatAbility 
{
public:
    CombatAbilityKey cAbility;
	int damage;
	int difficultyClass;
	AbilityEnum abilityChecked;
	CombatAbility(CombatAbilityKey c, int d, int dc, AbilityEnum a) 
        : cAbility(c)
        , damage(d)
        , difficultyClass(dc)
        , abilityChecked(a) {}
	CombatAbility(CombatAbility& c)
		: cAbility(c.cAbility)
		, damage(c.damage)
		, difficultyClass(c.difficultyClass)
		, abilityChecked(c.abilityChecked) {}
    
    CombatAbility()
        : cAbility(CombatAbilityKey::FIREBALL),
        damage(0),
        difficultyClass(0),
        abilityChecked(AbilityEnum::STRENGTH)
    {
    }
    CombatAbility(const CombatAbility &other)
        : cAbility(other.cAbility),
        damage(other.damage),
        difficultyClass(other.difficultyClass),
        abilityChecked(other.abilityChecked)
    {
    }
    CombatAbility(CombatAbility &&other) noexcept
        : cAbility(std::move(other.cAbility)),
        damage(std::move(other.damage)),
        difficultyClass(std::move(other.difficultyClass)),
        abilityChecked(std::move(other.abilityChecked))
    {
    }
    CombatAbility &operator=(const CombatAbility &other) {
        if (this != &other) {
            cAbility = other.cAbility;
            damage = other.damage;
            difficultyClass = other.difficultyClass;
            abilityChecked = other.abilityChecked;
        }
        return *this;
    }
    CombatAbility &operator=(CombatAbility &&other) noexcept {
        if (this != &other) {
            cAbility = std::move(other.cAbility);
            damage = std::move(other.damage);
            difficultyClass = std::move(other.difficultyClass);
            abilityChecked = std::move(other.abilityChecked);
        }
        return *this;
    }
    
};

std::ostream &operator<<(std::ostream &os, const CombatAbility &ability);

struct Abilities
{
public:
    std::array<int, MAX_ABILITY> abilitiesArray;
    Abilities(int a = 1);
    Abilities operator=(Abilities a);
    Abilities operator+=(Abilities a);
};
std::ostream &operator<<(std::ostream &os, const Abilities &abilities);

template <typename T, std::size_t N>
std::array <T, N> operator+=(std::array <T, N>& a, const std::array <T, N>& b)
{
    for (int i = 0; i < N; ++i)
        a[i] += b[i];
    return a;
}


class Character 
{
protected:
	const int maxHp = 15;
	std::string name;
    int hp;
    Abilities abilities;
    Race race;
    Class clasa;
    Background background;
    std::vector <CombatAbility> cAbilities;
    
    Character(std::string name)
        : hp{ maxHp }
        , name{ name }
    {};

public:

    void addCAbility(CombatAbility cAbil);
    std::string getRace();
    int rollDice(AbilityEnum ability) const;
	bool takeDamage(int damage);
    
    void setName(const std::string& name);
    int getMaxHp() const;
	int getHp() const;
    //implement getters for class,race,background
    std::string getClass() const;
    std::string getRace() const;
    std::string getBackground() const;
    //implement setters
    void setClass(Class c);
    void setRace(Race r);
    void setBackground(Background b);
    void setAbility(AbilityEnum a, int value);
	std::string getName() const;

    virtual ~Character() = default;
    friend DungeonMaster;
};

class Player : public Character
{
private:
    void addAbilitiesValues(Abilities ab);
public:
    Player() : Character("Player") {};
    Player(std::string n) : Character(n) {}
    void printStatistics();
    
	virtual ~Player() = default;
    
    friend Logger;
    friend Booster;
    friend DungeonMaster;
};









