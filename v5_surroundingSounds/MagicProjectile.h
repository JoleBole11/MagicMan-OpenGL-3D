#pragma once
#include "GameObject.h"
class MagicProjectile : public GameObject
{
private:
	float lifetime = 5.0f;
	float damage = 50.0f;
	bool isAlive = true;

public:
	MagicProjectile(const std::string& name) : GameObject(name) {}

	void update(float delta_time);
	float getDamage() const { return damage; }
	bool getIsAlive() const { return isAlive; }
	void setIsAlive(bool alive) { isAlive = alive; }
};

