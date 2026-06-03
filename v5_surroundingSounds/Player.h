#pragma once
#include "GameObject.h"
#include "Projectiles.h"
#include "Input.h"
#include "GameInstance.h"

class Player : public GameObject
{
protected:
	int health = 100;
	float fireCooldown = 0;
	bool isAlive = true;
	ProjectileType selectedWeapon = Magic;
		
public:
	Player(const std::string& name) : GameObject(name) {}

	void update(float delta_time);

	ProjectileType getSelectedWeapon() {
		return selectedWeapon;
	}
	void setSelectedWeapon(ProjectileType weapon) {
		selectedWeapon = weapon;
	}

	float getFireCooldown() {
		return fireCooldown;
	}
	void setFireCooldown(float cooldown) {
		fireCooldown = cooldown;
	}

	bool getIsAlive() {
		return isAlive;
	}

	int getHealth() { return health; }
	void takeDamage(float h);
};

