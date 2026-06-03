#pragma once
#include "GameObject.h"
#include "Player.h"
class Enemy : public GameObject
{
private:
	int health;
	int damage;
	float attackCooldown;
	bool isAlive;
	Player* player;
	float moveSpeed;
public:
	Enemy() : GameObject() {}
	Enemy(const std::string& name) : GameObject(name) {}

	virtual void update(float delta_time);
	virtual int getHealth();
	virtual void takeDamage(int damage);
	virtual int getDamage();
	virtual void attack();
	virtual void setIsAlive(bool alive);
	virtual bool getIsAlive();
	virtual float getAttackCooldown();
};

