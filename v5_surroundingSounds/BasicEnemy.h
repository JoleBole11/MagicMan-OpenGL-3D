#pragma once
#include "Enemy.h"

class BasicEnemy : public Enemy
{
private:
	int health = 100;
	int damage = 25;
	float attackCooldown = 2.0f;
	bool isAlive = true;
	Player* player = nullptr;
	float moveSpeed = 3.0f;

public:
	BasicEnemy(const std::string& name) : Enemy(name) {}

	void update(float delta_time) override;
	int getHealth() override;
	void takeDamage(int damage) override;
	int getDamage() override { return damage; }
	void attack() override;
	void setIsAlive(bool alive) override { isAlive = alive; }
	bool getIsAlive() override { return isAlive; }
	float getAttackCooldown() override { return attackCooldown; }
	
	void setPlayer(Player* p) { player = p; }
};

