#pragma once
#include "GameObject.h"
#include "Projectiles.h"
#include "Input.h"
#include "GameInstance.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Camera.h"
#include <vector>
#include "Projectile.h"
#include "PhysicsType.h"
#include "MagicProjectile.h"
#include "FreezeProjectile.h"
#include "FireballProjectile.h"
#include "SoundManager.h"

class Camera;
class btDynamicsWorld;

class Player : public GameObject
{
protected:
	int health = 100;
	float movementSpeed = 5.0f;
	float rotationSpeed = 3.0f;

	float fireCooldown = 0.0f;
	float magicCooldown = 0.5f;
	float freezeCooldown = 0.75f;
	float fireballCooldown = 1.75f;
	float poweredUpCooldown = 0.33f;
	float poweredUpFireballCooldown = 0.8f;

	float cooldownPowerDuration = 0.0f;
	float speedPowerDuration = 0.0f;

	bool isAlive = true;
	bool cooldownPowered = false;
	ProjectileTypes selectedWeapon = Magic;
		
public:
	Player(const std::string& name) : GameObject(name) {}

	void update(float delta_time);

	void handleInput(Camera* camera, btDynamicsWorld* world, std::vector<Projectile*>& projectiles, float delta_time);

	ProjectileTypes getSelectedWeapon();
	void setSelectedWeapon(ProjectileTypes weapon);

	float getFireCooldown();
	void setFireCooldown(float cooldown);

	void activateCooldownPower();
	void activateSpeedPower();

	bool getIsAlive();

	int getHealth();
	void addHealth(int hp);

	void takeDamage(float h);

	float getMagicCooldown() const;
	float getFreezeCooldown() const;
	float getFireballCooldown() const;
};

