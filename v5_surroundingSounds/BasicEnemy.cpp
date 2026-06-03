#include "BasicEnemy.h"
#include "Player.h"
#include "Transform.h"
#include "RigidBody.h"
#include <glm.hpp>

void BasicEnemy::update(float delta_time)
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

			btVector3 new_vel(
				direction.x * moveSpeed,
				current_vel.y(),
				direction.z * moveSpeed
			);

			body->setLinearVelocity(new_vel);
			body->activate();
		}
	}
}

int BasicEnemy::getHealth()
{
	return health;
}

void BasicEnemy::takeDamage(int amount)
{
	health -= amount;
}

void BasicEnemy::attack()
{
	attackCooldown = 2.0f;
}
