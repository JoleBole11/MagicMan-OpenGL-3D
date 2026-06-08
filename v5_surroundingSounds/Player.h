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
	float magicCooldown = 0.5f;
	float freezeCooldown = 0.75f;
	float fireballCooldown = 1.75f;
	bool isAlive = true;
	ProjectileTypes selectedWeapon = Magic;
		
public:
	Player(const std::string& name) : GameObject(name) {}

	void update(float delta_time);

	ProjectileTypes getSelectedWeapon();
	void setSelectedWeapon(ProjectileTypes weapon);
	float getFireCooldown();
	void setFireCooldown(float cooldown);
	bool getIsAlive();
	int getHealth();
	void takeDamage(float h);

	float getMagicCooldown() const;
	float getFreezeCooldown() const;
	float getFireballCooldown() const;
};

