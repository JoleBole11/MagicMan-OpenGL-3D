#include "Player.h"

void Player::update(float delta_time)
{
	GameObject::update(delta_time);
}

void Player::takeDamage(float damage)
{
	health -= damage;
	if (health <= 0) {
		isAlive = false;
	}
}
