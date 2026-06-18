#include "Pickup.h"

void Pickup::update(float delta_time) {
    GameObject::update(delta_time);
}

void Pickup::setIsAlive(bool _isAlive) {
    isAlive = _isAlive;
}

bool Pickup::getIsAlive() {
    return isAlive;
}

float Pickup::getDisappearTimer()
{
    return disappearTimer;
}
