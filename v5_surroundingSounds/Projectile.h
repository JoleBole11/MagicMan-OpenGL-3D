#pragma once
#include "GameObject.h"
class Projectile : public GameObject
{
private:
	float lifetime;
	float damage;
	bool isAlive;

public:
	Projectile(const std::string& name) : GameObject(name) {}

	virtual void update(float delta_time);
	virtual float getDamage() const { return damage; }
	virtual bool getIsAlive() const { return isAlive; }
	virtual void setIsAlive(bool alive) { isAlive = alive; }
};

