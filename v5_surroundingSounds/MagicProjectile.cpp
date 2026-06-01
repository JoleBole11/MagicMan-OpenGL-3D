#include "MagicProjectile.h"

void MagicProjectile::update(float delta_time) {
	GameObject::update(delta_time);
	lifetime -= delta_time;
	if (lifetime <= 0) {
		isAlive = false;
	}
}