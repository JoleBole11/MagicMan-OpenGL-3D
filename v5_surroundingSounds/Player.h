#pragma once
#include "GameObject.h"
#include "Projectiles.h"
#include "Input.h"
#include "GameInstance.h"

class Player : public GameObject
{
protected:
	float health = 100;
	float fireCooldown = 0;
	ProjectileType selectedWeapon = Magic;
		
public:
	Player(const std::string& name) : GameObject(name) {}

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

	float getHealth() {
		return health;
	}
	void setHealth(float h) {
		health = h;
	}
};

