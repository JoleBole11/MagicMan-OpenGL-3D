#include "FireballProjectile.h"

void FireballProjectile::update(float delta_time) {
	Projectile::update(delta_time);
	lifetime -= delta_time;
	if (lifetime <= 0) {
		isAlive = false;
	}
}