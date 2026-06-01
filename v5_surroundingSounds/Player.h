#pragma once
#include "GameObject.h"
#include "Projectiles.h"
#include "Input.h"
#include "GameInstance.h"
#include "GameScene.h"

class Player : public GameObject
{
protected:
	float health = 100;
	float fireCooldown = 0;
	ProjectileType selectedWeapon = Magic;
		
public:
	Player(const std::string& name) : GameObject(name) {}

	void update(float dt);

	void FireProjectile();
};

