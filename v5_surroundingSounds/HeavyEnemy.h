#pragma once
#include "Enemy.h"

class HeavyEnemy : public Enemy
{
private:
	int health = 200;
	int damage = 50;
	float attackCooldown = 3.0f;
	bool isAlive = true;
	Player* player = nullptr;
	float moveSpeed = 2.25f;
	int pointsWorth = 2;
	float slowDownTimer = 0;
	bool isSlowed = false;
public:
	HeavyEnemy(const std::string& name) : Enemy(name) {}

	void update(float delta_time) override;
	int getHealth() override;
	void takeDamage(int damage) override;
	int getDamage() override { return damage; }
	void attack() override;
	void setIsAlive(bool alive) override { isAlive = alive; }
	bool getIsAlive() override { return isAlive; }
	float getAttackCooldown() override { return attackCooldown; }
	void setPlayer(Player* p) override;
	void slowDown() override;
	int getPointsWorth() override;
};

