#include "FreezeProjectile.h"

void FreezeProjectile::update(float delta_time) {
	Projectile::update(delta_time);
	lifetime -= delta_time;
	if (lifetime <= 0) {
		isAlive = false;
	}
}