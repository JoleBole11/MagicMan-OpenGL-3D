#pragma once
#include "Enemy.h"

class BasicEnemy : public Enemy
{
private:
	int health = 50;
	int damage = 25;
	float attackCooldown = 2.0f;
	bool isAlive = true;
	Player* player = nullptr;
	float moveSpeed = 3.0f;

public:
	BasicEnemy(const std::string& name) : Enemy(name) {}

	void update(float delta_time) override;
	int getHealth() override;
	void takeDamage(int amount) override;
	void attack() override;
	void setIsAlive(bool alive) override { isAlive = alive; }
	bool getIsAlive() override { return isAlive; }
	
	void setPlayer(Player* p) { player = p; }
};

