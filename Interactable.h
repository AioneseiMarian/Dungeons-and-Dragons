#pragma once
#include <iostream>
#include "Character.h"
class Room;

enum InteractableKey
{
	DOOR,
	PORTAL,
	BOOK,
	CHEST,
	AXE,
	ENEMY,
	MAX_INTERACTABLE
};

class IInteractable	
{
public:
	virtual InteractableKey getInteractableKey() const = 0;
	virtual std::string getMessage() const = 0;
	virtual std::string getName() const = 0;
	virtual void randomizeValues() = 0;
	virtual bool interact(Player& player) = 0;
	virtual void printDifficulty() = 0;
	virtual int getDifficulty() = 0;
	virtual AbilityEnum getAbilityNeeded() = 0;
	
	virtual ~IInteractable() = default;
};

class AInteractable : public IInteractable
{
protected:
	std::string message;
	InteractableKey interactableKey;
	AInteractable(const std::string &mess, InteractableKey t)
		:interactableKey{ t }
		, message{ mess }
	{}
public:

	InteractableKey getInteractableKey() const;
	virtual std::string getName() const override;
	virtual std::string getMessage() const override;
	
	virtual ~AInteractable() = default;
};

class Enemy : public AInteractable, public Character
{
protected:
public:
	Enemy() = default;
	Enemy(const std::string& mess, const std::string& name)
		: AInteractable{ mess, InteractableKey::ENEMY }
		, Character{ name }
	{
	}
	
	virtual AbilityEnum getAbilityNeeded() override;
	virtual int getDifficulty() override;
	virtual bool interact(Player &player) override;
	virtual void randomizeValues() override;
	virtual std::string getMessage() const override;
	virtual std::string getName() const override;
	void printDifficulty() override;
	virtual ~Enemy() = default;
	friend class DungeonMaster;
};


class AInteractableObj : public AInteractable
{
protected:
	int difficultyClass;
	AbilityEnum checkedAbility;

	AInteractableObj(const std::string& mess, InteractableKey t, int difficulty, AbilityEnum usedAbility )
		:AInteractable{ mess, t }
		,difficultyClass{ difficulty }
		,checkedAbility{ usedAbility }
	{}
public:
	//virtual void interact() const = 0;
	virtual int getDifficulty() override;
	virtual void printDifficulty() override;
	virtual void randomizeValues() override;
	virtual AbilityEnum getAbilityNeeded() override;
	virtual ~AInteractableObj() = default;
};

class Weapon : public AInteractableObj
{
public:
	Weapon(const std::string& mess, InteractableKey t, int difficulty, AbilityEnum usedAbility)
		:AInteractableObj{mess, t, difficulty, usedAbility }
	{}
	virtual bool interact(Player &player) override;


	~Weapon() = default;
};

class Teleporter : public AInteractableObj
{
private:
	Room *destinationRoom;
public:
	Teleporter(const std::string& mess, InteractableKey t, int difficulty, AbilityEnum usedAbility)
		:AInteractableObj{mess, t, difficulty, usedAbility }
	{}
	virtual bool interact(Player& player) override;
	void setDestinationRoom(Room* room);
	~Teleporter() = default;
};

class Booster : public AInteractableObj
{
private:
	Abilities abilityBooster;
public:
	Booster(const std::string& mess, InteractableKey t, int difficulty, AbilityEnum usedAbility, Abilities abilityBoost)
		:AInteractableObj{mess, t, difficulty, usedAbility }
		, abilityBooster{ abilityBoost }
	{}
	virtual void randomizeValues() override;
	virtual bool interact(Player &player) override;
	Abilities getAbilities() const;
	~Booster() = default;
};

