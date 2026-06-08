#pragma once
#include "Projectile.h"
class FreezeProjectile : public Projectile
{
private:
	float lifetime = 5.0f;
	float damage = 34.0f;
	bool isAlive = true;

public:
	FreezeProjectile(const std::string& name) : Projectile(name) {}

	void update(float delta_time);
	float getDamage() const { return damage; }
	bool getIsAlive() const { return isAlive; }
	void setIsAlive(bool alive) { isAlive = alive; }
};

