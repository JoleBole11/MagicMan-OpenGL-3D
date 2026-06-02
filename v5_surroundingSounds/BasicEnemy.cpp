#include "BasicEnemy.h"

void BasicEnemy::update(float delta_time)
{
	Enemy::update(delta_time);
	if (attackCooldown > 0) {
		attackCooldown -= delta_time;
	}
}

int BasicEnemy::getHealth()
{
	return health;
}

void BasicEnemy::takeDamage(int amount)
{
	health -= amount;
}

void BasicEnemy::attack()
{
	attackCooldown = 2.0f;
}
