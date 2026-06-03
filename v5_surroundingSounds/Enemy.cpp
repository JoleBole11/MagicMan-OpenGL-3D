#include "Enemy.h"

void Enemy::update(float delta_time) {
    GameObject::update(delta_time);
}

int Enemy::getHealth() {
    return health;
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        isAlive = false;
    }
}

void Enemy::attack() {
}

void Enemy::setIsAlive(bool alive) {
    isAlive = alive;
}

bool Enemy::getIsAlive() {
    return isAlive;
}