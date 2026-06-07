#include "Player.h"

void Player::update(float delta_time)
{
	GameObject::update(delta_time);
}

ProjectileType Player::getSelectedWeapon()
{
	return selectedWeapon;
}

void Player::setSelectedWeapon(ProjectileType weapon)
{
	selectedWeapon = weapon;
}

float Player::getFireCooldown()
{
	return fireCooldown;
}

void Player::setFireCooldown(float cooldown)
{
	fireCooldown = cooldown;
}

bool Player::getIsAlive()
{
	return isAlive;
}

int Player::getHealth()
{
	return health;
}

void Player::takeDamage(float damage)
{
	health -= damage;
	if (health <= 0) {
		isAlive = false;
	}
}

float Player::getMagicCooldown() const
{
	return magicCooldown;
}

float Player::getFreezeCooldown() const
{
	return freezeCooldown;
}

float Player::getFireballCooldown() const
{
	return fireballCooldown;
}
