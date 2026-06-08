#include "HeavyEnemy.h"

void HeavyEnemy::update(float delta_time)
{
	Enemy::update(delta_time);

	if (attackCooldown > 0) {
		attackCooldown -= delta_time;
	}

	if (player != nullptr) {
		auto* enemyTransform = get_component<Transform>();
		auto* playerTransform = player->get_component<Transform>();

		if (enemyTransform != nullptr && playerTransform != nullptr) {
			glm::vec3 enemyPos = enemyTransform->position;
			glm::vec3 playerPos = playerTransform->position;

			glm::vec3 direction = playerPos - enemyPos;
			float distance = glm::length(direction);

			direction = glm::normalize(direction);

			auto* rb = get_component<RigidBody>();

			btRigidBody* body = rb->get_body();
			btVector3 current_vel = body->getLinearVelocity();

			if (!isSlowed) {
				btVector3 new_vel(
					direction.x * moveSpeed,
					current_vel.y(),
					direction.z * moveSpeed
				);

				body->setLinearVelocity(new_vel);
			}
			else {
				slowDownTimer -= delta_time;
				if (slowDownTimer <= 0) {
					isSlowed = false;
					moveSpeed = 2.25f;
				}
				btVector3 new_vel(
					direction.x * moveSpeed,
					current_vel.y(),
					direction.z * moveSpeed
				);
				body->setLinearVelocity(new_vel);
			}

			body->activate();
		}
	}
}

int HeavyEnemy::getHealth()
{
	return health;
}

void HeavyEnemy::takeDamage(int damage)
{
	health -= damage;
	if (health <= 0) {
		isAlive = false;
	}
}

void HeavyEnemy::attack()
{
	attackCooldown = 3.0f;
}

void HeavyEnemy::setPlayer(Player* p)
{
	player = p;
}

void HeavyEnemy::slowDown()
{
	slowDownTimer = 3.0f;
	moveSpeed = 1.25f;
}
