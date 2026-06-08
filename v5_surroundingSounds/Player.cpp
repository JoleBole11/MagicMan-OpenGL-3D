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

void Player::handleInput(Camera* camera, btDynamicsWorld* world, std::vector<Projectile*>& projectiles, float delta_time)
{
	auto* rbComp = get_component<RigidBody>();
	auto* t = get_component<Transform>();
	if (!rbComp || !t || !camera) return;

	btRigidBody* body = rbComp->get_body();

	float v = Input::get_axis("Vertical");
	float h = Input::get_axis("Horizontal");

	glm::vec3 dir = camera->get_forward() * v + camera->get_right() * h;
	dir.y = 0.0f;

	btVector3 current_vel = body->getLinearVelocity();

	btVector3 new_vel(
		dir.x * movementSpeed,
		current_vel.y(),
		dir.z * movementSpeed
	);

	body->setLinearVelocity(new_vel);

	camera->set_position(t->position + glm::vec3(0, 1.0f, 0));

	float x = 0.0, y = 0.0;
	Input::get_mouse_position(&x, &y);

	float yaw = camera->get_rotation().y + float(-x * rotationSpeed * delta_time);
	float pitch = camera->get_rotation().x + float(-y * rotationSpeed * delta_time);

	pitch = glm::clamp(pitch, -70.0f, 70.0f);

	camera->set_rotation(glm::vec3(pitch, yaw, 0.0f));

	if (fireCooldown > 0.0f) {
		fireCooldown -= delta_time;
	}

	if (Input::get_mouse_button_down(0) && fireCooldown <= 0.0f) {
		Projectile* projectile = nullptr;
		Transform* tp = nullptr;
		btCollisionShape* projectileShape = nullptr;
		glm::vec3 forward = camera->get_forward() * 40.0f;
		btVector3 velocity = btVector3(forward.x, forward.y, forward.z);

		PhysicsType* physicsType = nullptr;

		switch (selectedWeapon)
		{
		case Magic:
			projectile = new MagicProjectile("Projectile");
			projectile->add_component<MeshRenderer>("models/projectiles/MagicProjectile.obj");
			tp = projectile->get_component<Transform>();
			tp->rotation = camera->get_rotation();
			tp->position = t->position + glm::vec3(0, 1.0f, 0);
			tp->scale = glm::vec3(0.25f, 0.25f, 0.25f);
			projectileShape = new btSphereShape(0.25f);
			projectile->add_component<RigidBody>(0.1f, projectileShape, world);
			projectile->get_component<RigidBody>()->get_body()->setLinearVelocity(velocity);

			physicsType = new PhysicsType{
				ObjectType::PROJECTILE,
				projectile
			};

			projectile->get_component<RigidBody>()->get_body()->setUserPointer(physicsType);

			projectiles.push_back(projectile);
			fireCooldown = magicCooldown;
			break;
		case Freeze:
			projectile = new FreezeProjectile("Projectile");
			projectile->add_component<MeshRenderer>("models/projectiles/FreezeProjectile.obj");
			tp = projectile->get_component<Transform>();
			tp->rotation = camera->get_rotation();
			tp->position = t->position + glm::vec3(0, 1.0f, 0);
			tp->scale = glm::vec3(0.25f, 0.25f, 0.25f);
			projectileShape = new btSphereShape(0.25f);
			projectile->add_component<RigidBody>(0.1f, projectileShape, world);
			projectile->get_component<RigidBody>()->get_body()->setLinearVelocity(velocity);

			physicsType = new PhysicsType{
				ObjectType::PROJECTILE,
				projectile
			};

			projectile->get_component<RigidBody>()->get_body()->setUserPointer(physicsType);

			projectiles.push_back(projectile);
			fireCooldown = freezeCooldown;
			break;
		case Fireball:
			projectile = new FireballProjectile("Projectile");
			projectile->add_component<MeshRenderer>("models/projectiles/FireballProjectile.obj");
			tp = projectile->get_component<Transform>();
			tp->rotation = camera->get_rotation();
			tp->position = t->position + glm::vec3(0, 1.0f, 0);
			tp->scale = glm::vec3(0.25f, 0.25f, 0.25f);
			projectileShape = new btSphereShape(0.25f);
			projectile->add_component<RigidBody>(0.1f, projectileShape, world);
			projectile->get_component<RigidBody>()->get_body()->setLinearVelocity(velocity);

			physicsType = new PhysicsType{
				ObjectType::PROJECTILE,
				projectile
			};

			projectile->get_component<RigidBody>()->get_body()->setUserPointer(physicsType);

			projectiles.push_back(projectile);
			fireCooldown = fireballCooldown;
			break;
		default:
			break;
		}

		SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().fireSound, SoundManager::get_instance().surrounding_sounds, t->position);
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
