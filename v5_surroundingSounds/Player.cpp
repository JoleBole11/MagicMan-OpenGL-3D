#include "Player.h"

void Player::update(float delta_time)
{
	GameObject::update(delta_time);

	if (Input::get_key_down('1')) {
		setSelectedWeapon(Magic);
	}
	if (Input::get_key_down('2')) {
		setSelectedWeapon(Freeze);
	}
	if (Input::get_key_down('3')) {
		setSelectedWeapon(Fireball);
	}
}

ProjectileTypes Player::getSelectedWeapon()
{
	return selectedWeapon;
}

void Player::setSelectedWeapon(ProjectileTypes weapon)
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
