#include "Enemy.h"

void Enemy::update(float delta_time) {
    GameObject::update(delta_time);
}

int Enemy::getHealth() {
    return health;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        isAlive = false;
    }
}

int Enemy::getDamage()
{
    return damage;
}

void Enemy::attack() {
}

void Enemy::setIsAlive(bool alive) {
    isAlive = alive;
}

bool Enemy::getIsAlive() {
    return isAlive;
}

float Enemy::getAttackCooldown()
{
    return attackCooldown;
}
