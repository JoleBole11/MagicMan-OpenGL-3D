#include "GameScene.h"

std::unique_ptr<Font> GameScene::shared_font = nullptr;
std::mt19937 mt(time(nullptr));
std::uniform_real_distribution<float> scaleDist(0.9f, 1.2f);
std::uniform_real_distribution<float> offsetDist(-2.0f, 2.0f);
std::uniform_real_distribution<float> rotationDist(0.0f, 360.0f);
std::uniform_int_distribution<int> pickupChanceDist(1, 4);
std::uniform_int_distribution<int> pickupTypeDist(1, 9);
std::uniform_int_distribution<int> enemySpawnDist(1, 8);

GameScene::GameScene() : Scene("Game")
{
	
}

GameScene::~GameScene() {
	cleanup();
}

void GameScene::initialize() {
	cleanup();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_pos);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);

	GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);

	GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat mat_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat mat_shininess[] = { 32.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_NORMALIZE);

	int randomDrop = 1;

	SoundManager::get_instance().playSong(SoundManager::get_instance().gameplaySong);

	broadphase = new btDbvtBroadphase();
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration);
	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
	world->setGravity(btVector3(0, -9.81f, 0));

	debug_drawer = new GLDebugDrawer();
	debug_drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	world->setDebugDrawer(debug_drawer);

	player = std::make_unique<Player>("Player");
	auto* playerShape = new btBoxShape(btVector3(0.5f, 1.0f, 0.5f));
	playerTransform = player->get_component<Transform>();
	playerTransform->position = glm::vec3(3, 0.5f, 3);
	player->add_component<RigidBody>(1.0f, playerShape, world);
	playerRb = player->get_component<RigidBody>();
	world->removeRigidBody(playerRb->get_body());
	world->addRigidBody(playerRb->get_body(), CG_PLAYER, CG_ENEMY | CG_PICKUP | CG_DEFAULT);
	playerRb->get_body()->setAngularFactor(btVector3(0, 0, 0));
	playerRb->get_body()->forceActivationState(DISABLE_DEACTIVATION);
	playerRb->get_body()->setUserPointer(new PhysicsType{ ObjectType::PLAYER, player.get()});

	playerHands = std::make_unique<GameObject>("PlayerHands");
	playerHands->add_component<MeshRenderer>("models/Player/Hands.obj");
	playerHands->get_component<Transform>()->position = playerTransform->position + glm::vec3(0, 1.2f, 0);
	playerHands->get_component<Transform>()->scale = glm::vec3(0.1f, 0.1f, 0.1f);

	camera = std::make_unique<Camera>(60.0f, float(window_size[0]) / float(window_size[1]), 0.1f, 300.0f);
	Input::set_cursor_lock(is_cursor_locked = true);

	treeMap = {
		{0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
		{0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
		{1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
		{0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
		{0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	};

	for (int i = 0; i < treeMap.size(); i++) {
		for (int j = 0; j < treeMap[i].size(); j++) {
			if (treeMap[i][j] == 1) {
				GameObject* tree = new GameObject("Tree");
				tree->add_component<MeshRenderer>("models/tree/tree.obj");
				Transform* t = tree->get_component<Transform>();

				float randomScale = scaleDist(mt);
				float posOffsetX = offsetDist(mt);
				float posOffsetZ = offsetDist(mt);
				float randomRotationY = rotationDist(mt);

				t->position = glm::vec3((i + 1) * 5 - 40 + posOffsetX, 0.0f, (j + 1) * 5 - 40 + posOffsetZ);
				t->scale = glm::vec3(randomScale, randomScale, randomScale);
				t->rotation = glm::angleAxis(glm::radians(randomRotationY), glm::vec3(0, 1, 0));

				btBoxShape* shape = new btBoxShape(btVector3(0.5f, 5.0f, 0.5f));
				tree->add_component<RigidBody>(0.0f, shape, world);
				world->removeRigidBody(tree->get_component<RigidBody>()->get_body());
				world->addRigidBody(tree->get_component<RigidBody>()->get_body(), CG_DEFAULT, CG_ENEMY | CG_PICKUP | CG_PLAYER | CG_PROJECTILE);
				trees.push_back(tree);
			}
		}
	}

	auto* invisibleWall = new GameObject("InvisibleWall");
	auto* shape1 = new btBoxShape(btVector3(0.1f, 5.0f, 70.0f));
	invisibleWall->get_component<Transform>()->position = glm::vec3(-30.0f, 0.0f, 35.0f);
	invisibleWall->add_component<RigidBody>(0.0f, shape1, world);
	world->removeRigidBody(invisibleWall->get_component<RigidBody>()->get_body());
	world->addRigidBody(invisibleWall->get_component<RigidBody>()->get_body(), CG_DEFAULT, CG_ENEMY | CG_PICKUP | CG_PLAYER | CG_PROJECTILE);
	invisibleWalls.push_back(invisibleWall);

	auto* invisibleWall1 = new GameObject("InvisibleWall");
	auto* shape2 = new btBoxShape(btVector3(0.1f, 5.0f, 70.0f));
	invisibleWall1->get_component<Transform>()->position = glm::vec3(55.0f, 0.0f, 35.0f);
	invisibleWall1->add_component<RigidBody>(0.0f, shape2, world);
	world->removeRigidBody(invisibleWall1->get_component<RigidBody>()->get_body());
	world->addRigidBody(invisibleWall1->get_component<RigidBody>()->get_body(), CG_DEFAULT, CG_ENEMY | CG_PICKUP | CG_PLAYER | CG_PROJECTILE);
	invisibleWalls.push_back(invisibleWall1);

	auto* invisibleWall2 = new GameObject("InvisibleWall");
	invisibleWall2->get_component<Transform>()->position = glm::vec3(0.0f, 0.0f, -25.0f);
	invisibleWall2->get_component<Transform>()->setRotationDegrees(0, 90, 0);
	auto* shape3 = new btBoxShape(btVector3(0.1f, 5.0f, 70.0f));
	invisibleWall2->add_component<RigidBody>(0.0f, shape3, world);
	world->removeRigidBody(invisibleWall2->get_component<RigidBody>()->get_body());
	world->addRigidBody(invisibleWall2->get_component<RigidBody>()->get_body(), CG_DEFAULT, CG_ENEMY | CG_PICKUP | CG_PLAYER | CG_PROJECTILE);
	invisibleWalls.push_back(invisibleWall2);

	auto* invisibleWall3 = new GameObject("InvisibleWall");
	auto* shape4 = new btBoxShape(btVector3(0.1f, 5.0f, 70.0f));
	invisibleWall3->get_component<Transform>()->position = glm::vec3(0.0f, 0.0f, 55.0f);
	invisibleWall3->get_component<Transform>()->setRotationDegrees(0, 90, 0);
	invisibleWall3->add_component<RigidBody>(0.0f, shape4, world);
	world->removeRigidBody(invisibleWall3->get_component<RigidBody>()->get_body());
	world->addRigidBody(invisibleWall3->get_component<RigidBody>()->get_body(), CG_DEFAULT, CG_ENEMY | CG_PICKUP | CG_PLAYER | CG_PROJECTILE);
	invisibleWalls.push_back(invisibleWall3);

	map = std::make_unique<GameObject>("Map");
	map->add_component<MeshRenderer>("models/map/grassBlock.obj");
	auto* t = map->get_component<Transform>();
	t->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 0, 0));
	t->position = glm::vec3(0.0f, 0.0f, 3.0f);
	t->scale = glm::vec3(0.7f, 1.0f, 0.7f);
	auto* shape = new btBoxShape(btVector3(70, 0.1f, 70));
	map->add_component<RigidBody>(0.0f, shape, world);
	world->removeRigidBody(map->get_component<RigidBody>()->get_body());
	world->addRigidBody(map->get_component<RigidBody>()->get_body(), CG_DEFAULT, CG_ENEMY | CG_DEFAULT | CG_PLAYER | CG_PROJECTILE);

	enemySpawner(glm::vec3(20, 0.5f, 20));
	enemySpawner(glm::vec3(20, 0.5f, -20));
	enemySpawner(glm::vec3(-20, 0.5f, 20));
	enemySpawner(glm::vec3(-20, 0.5f, -20));
	enemySpawner(glm::vec3(-0, 0.5f, 20));
	enemySpawner(glm::vec3(-0, 0.5f, -20));

	magicImg = std::make_unique<GameObject>("Sprite");
	magicImg->add_component<Sprite>("sprites/magic.png");
	auto* tm = magicImg->get_component<Transform>();
	if (tm) {
		tm->scale = glm::vec3(0.05f, 0.05f, 0.05f);
	}
	magicImg->get_component<Transform>()->position = glm::vec3(375.0f, 75.0f, 0.0f);

	freezeImg = std::make_unique<GameObject>("Sprite");
	freezeImg->add_component<Sprite>("sprites/freeze.png");
	auto* tf = freezeImg->get_component<Transform>();
	if (tf) {
		tf->scale = glm::vec3(0.05f, 0.05f, 0.05f);
	}
	freezeImg->get_component<Transform>()->position = glm::vec3(500.0f, 75.0f, 0.0f);

	fireballImg = std::make_unique<GameObject>("Sprite");
	fireballImg->add_component<Sprite>("sprites/fireball.png");
	auto* tf2 = fireballImg->get_component<Transform>();
	if (tf2) {
		tf2->scale = glm::vec3(0.0325f, 0.045f, 0.05f);
	}
	fireballImg->get_component<Transform>()->position = glm::vec3(640.0f, 75.0f, 0.0f);


	shared_font = std::make_unique<Font>(
		"fonts/Roboto-Regular.ttf",
		36
	);

	healthText = std::make_unique<GameObject>("healthText");
	healthText->add_component<Text>(
		shared_font.get(),
		"Health: " + std::to_string(player->getHealth()),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	healthText->get_component<Transform>()->position = glm::vec3(100.0f, 100.0f, 0.0f);

	scoreText = std::make_unique<GameObject>("scoreText");
	scoreText->add_component<Text>(
		shared_font.get(),
		"Score: " + std::to_string(score),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	scoreText->get_component<Transform>()->position = glm::vec3(100.0f, 900.0f, 0.0f);
}

void GameScene::enemySpawner(const glm::vec3& pos)
{
	auto* enemySpawn = new GameObject("EnemySpawn");
	enemySpawn->get_component<Transform>()->position = pos;

	enemySpawns.push_back(enemySpawn);
}

void GameScene::spawnEnemy(const glm::vec3& pos)
{
	auto* enemy = new BasicEnemy("Enemy");
	auto* enemyShape = new btBoxShape(btVector3(1, 1, 1));
	enemy->add_component<MeshRenderer>("models/enemies/basicEnemy.obj");
	enemy->get_component<Transform>()->position = pos;
	enemy->add_component<RigidBody>(10.0f, enemyShape, world);
	auto* rb = enemy->get_component<RigidBody>()->get_body();
	world->removeRigidBody(rb);
	world->addRigidBody(rb, CG_ENEMY, CG_PLAYER | CG_PROJECTILE | CG_DEFAULT);
	enemy->get_component<RigidBody>()->get_body()->setAngularFactor(btVector3(0, 0, 0));
	enemy->get_component<Transform>()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
	
	PhysicsType* type = new PhysicsType{
	ObjectType::ENEMY,
	enemy
	};
	
	enemy->get_component<RigidBody>()->get_body()->setUserPointer(type);
	
	enemy->setPlayer(player.get());
	enemies.push_back(enemy);
}

void GameScene::spawnHeavyEnemy(const glm::vec3& pos)
{
	auto* enemy = new HeavyEnemy("Enemy");
	auto* enemyShape = new btBoxShape(btVector3(1, 1, 1));
	enemy->add_component<MeshRenderer>("models/enemies/heavyEnemy.obj");
	enemy->get_component<Transform>()->position = pos;
	enemy->add_component<RigidBody>(10.0f, enemyShape, world);
	auto* rb = enemy->get_component<RigidBody>()->get_body();
	world->removeRigidBody(rb);
	world->addRigidBody(rb, CG_ENEMY, CG_PLAYER | CG_PROJECTILE | CG_DEFAULT);
	enemy->get_component<RigidBody>()->get_body()->setAngularFactor(btVector3(0, 0, 0));
	enemy->get_component<Transform>()->scale = glm::vec3(0.7f, 0.5f, 0.7f);

	PhysicsType* type = new PhysicsType{
	ObjectType::ENEMY,
	enemy
	};

	enemy->get_component<RigidBody>()->get_body()->setUserPointer(type);

	enemy->setPlayer(player.get());
	enemies.push_back(enemy);
}

void GameScene::spawnEnemies()
{
	switch (round)
	{
	case 1:
		for (int i = 0; i < 2; i++) {
			spawnEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
		break;
	case 2:
		for (int i = 0; i < 4; i++) {
			spawnEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
		break;
	case 3:
		for (int i = 0; i < 3; i++) {
			spawnEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
		spawnHeavyEnemy(enemySpawns[3]->get_component<Transform>()->position);
		break;
	case 4:
		for (int i = 0; i < 3; i++) {
			spawnEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
		for (int i = 3; i < 5; i++) {
			spawnHeavyEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
		break;
	case 5:
		for (int i = 0; i < 4; i++) {
			spawnEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
		for (int i = 4; i < 6; i++) {
			spawnHeavyEnemy(enemySpawns[i]->get_component<Transform>()->position);
		}
	default:
		break;
	}
}

void GameScene::dropPickup(const glm::vec3& pos)
{
	dropChance = pickupChanceDist(mt);
	if (dropChance == 3) {
		dropType = pickupTypeDist(mt);
		if (dropType <= 3) {
			SpeedPickup* pickup = new SpeedPickup("speedPickup");
			pickup->add_component<MeshRenderer>("models/Pickups/coke.obj");
			btBoxShape* pickupBox = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
			Transform* pickupT = pickup->get_component<Transform>();
			pickupT->position = glm::vec3(pos.x, 1.0f, pos.z);
			pickupT->scale = glm::vec3(0.5f, 0.5f, 0.5f);
			pickupT->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 0, 0));

			pickup->add_component<RigidBody>(0.0f, pickupBox, world);
			auto* rb = pickup->get_component<RigidBody>()->get_body();
			world->removeRigidBody(rb);
			world->addRigidBody(rb, CG_PICKUP, CG_PLAYER);
			btRigidBody* pickupBody = pickup->get_component<RigidBody>()->get_body();
			pickup->get_component<RigidBody>()->get_body()->setUserPointer(new PhysicsType{ ObjectType::PICKUP, pickup });
			pickups.push_back(pickup);
		}
		else if (dropType <= 6) {
			HealPickup* pickup = new HealPickup("healthPickup");
			pickup->add_component<MeshRenderer>("models/Pickups/medkit.obj");
			btBoxShape* pickupBox = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
			Transform* pickupT = pickup->get_component<Transform>();
			pickupT->position = glm::vec3(pos.x, 2, pos.z);
			pickupT->scale = glm::vec3(0.3f, 0.3f, 0.3f);
			pickupT->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 0, 0));

			pickup->add_component<RigidBody>(0.0f, pickupBox, world);
			auto* rb = pickup->get_component<RigidBody>()->get_body();
			world->removeRigidBody(rb);
			world->addRigidBody(rb, CG_PICKUP, CG_PLAYER);
			btRigidBody* pickupBody = pickup->get_component<RigidBody>()->get_body();
			pickup->get_component<RigidBody>()->get_body()->setUserPointer(new PhysicsType{ ObjectType::PICKUP, pickup });
			pickups.push_back(pickup);
		}
		else if (dropType <= 8) {
			CooldownPickup* pickup = new CooldownPickup("cooldownPickup");
			pickup->add_component<MeshRenderer>("models/Pickups/clock.obj");
			btBoxShape* pickupBox = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
			Transform* pickupT = pickup->get_component<Transform>();
			pickupT->position = glm::vec3(pos.x, 2, pos.z);
			pickupT->scale = glm::vec3(3.0f, 3.0f, 3.0f);
			pickupT->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 0, 0));

			pickup->add_component<RigidBody>(0.0f, pickupBox, world);
			auto* rb = pickup->get_component<RigidBody>()->get_body();
			world->removeRigidBody(rb);
			world->addRigidBody(rb, CG_PICKUP, CG_PLAYER);
			btRigidBody* pickupBody = pickup->get_component<RigidBody>()->get_body();
			pickup->get_component<RigidBody>()->get_body()->setUserPointer(new PhysicsType{ ObjectType::PICKUP, pickup });
			pickups.push_back(pickup);
		}
		else {
			NukePickup* pickup = new NukePickup("nukePickup");
			pickup->add_component<MeshRenderer>("models/Pickups/nuke.obj");
			btBoxShape* pickupBox = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
			Transform* pickupT = pickup->get_component<Transform>();
			pickupT->position = glm::vec3(pos.x, 1.5f, pos.z);
			pickupT->scale = glm::vec3(0.7f, 0.7f, 0.7f);
			pickupT->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 0, 0));

			pickup->add_component<RigidBody>(0.0f, pickupBox, world);
			auto* rb = pickup->get_component<RigidBody>()->get_body();
			world->removeRigidBody(rb);
			world->addRigidBody(rb, CG_PICKUP, CG_PLAYER);
			btRigidBody* pickupBody = pickup->get_component<RigidBody>()->get_body();
			pickup->get_component<RigidBody>()->get_body()->setUserPointer(new PhysicsType{ ObjectType::PICKUP, pickup });
			pickups.push_back(pickup);
		}
	}
}

void GameScene::nukeEnemies()
{
	for (auto& enemy : enemies) {
		enemy->setIsAlive(false);
	}
}

void GameScene::spawnProjectile()
{
	Projectile* projectile = nullptr;
	Transform* tp = nullptr;
	btCollisionShape* projectileShape = nullptr;
	glm::vec3 dir = camera->get_forward() * 40.0f;
	btVector3 velocity = btVector3(dir.x, dir.y, dir.z);

	PhysicsType* physicsType = nullptr;

	switch (player->getSelectedWeapon())
	{
	case Magic:
		projectile = new MagicProjectile("Projectile");
		projectile->add_component<MeshRenderer>("models/projectiles/MagicProjectile.obj");
		tp = projectile->get_component<Transform>();
		tp->rotation = camera->get_rotation();
		tp->position = playerTransform->position + glm::vec3(0, 1.0f, 0);
		tp->scale = glm::vec3(0.25f, 0.25f, 0.25f);
		projectileShape = new btSphereShape(0.25f);
		projectile->add_component<RigidBody>(0.1f, projectileShape, world);
		world->removeRigidBody(projectile->get_component<RigidBody>()->get_body());
		world->addRigidBody(projectile->get_component<RigidBody>()->get_body(), CG_PROJECTILE, CG_ENEMY | CG_DEFAULT);

		projectile->get_component<RigidBody>()->get_body()->setLinearVelocity(velocity);

		physicsType = new PhysicsType{
			ObjectType::PROJECTILE,
			projectile
		};

		projectile->get_component<RigidBody>()->get_body()->setUserPointer(physicsType);

		projectiles.push_back(projectile);
		player->setFireCooldown(player->getMagicCooldown());
		break;
	case Freeze:
		projectile = new FreezeProjectile("Projectile");
		projectile->add_component<MeshRenderer>("models/projectiles/FreezeProjectile.obj");
		tp = projectile->get_component<Transform>();
		tp->rotation = camera->get_rotation();
		tp->position = playerTransform->position + glm::vec3(0, 1.0f, 0);
		tp->scale = glm::vec3(0.25f, 0.25f, 0.25f);
		projectileShape = new btSphereShape(0.25f);
		projectile->add_component<RigidBody>(0.1f, projectileShape, world);
		world->removeRigidBody(projectile->get_component<RigidBody>()->get_body());
		world->addRigidBody(projectile->get_component<RigidBody>()->get_body(), CG_PROJECTILE, CG_ENEMY | CG_DEFAULT);

		projectile->get_component<RigidBody>()->get_body()->setLinearVelocity(velocity);

		physicsType = new PhysicsType{
			ObjectType::PROJECTILE,
			projectile
		};

		projectile->get_component<RigidBody>()->get_body()->setUserPointer(physicsType);

		projectiles.push_back(projectile);
		player->setFireCooldown(player->getFreezeCooldown());
		break;
	case Fireball:
		projectile = new FireballProjectile("Projectile");
		projectile->add_component<MeshRenderer>("models/projectiles/FireballProjectile.obj");
		tp = projectile->get_component<Transform>();
		tp->rotation = camera->get_rotation();
		tp->position = playerTransform->position + glm::vec3(0, 1.0f, 0);
		tp->scale = glm::vec3(0.25f, 0.25f, 0.25f);
		projectileShape = new btSphereShape(0.25f);
		projectile->add_component<RigidBody>(0.1f, projectileShape, world);
		world->removeRigidBody(projectile->get_component<RigidBody>()->get_body());
		world->addRigidBody(projectile->get_component<RigidBody>()->get_body(), CG_PROJECTILE, CG_ENEMY | CG_DEFAULT);

		projectile->get_component<RigidBody>()->get_body()->setLinearVelocity(velocity);

		physicsType = new PhysicsType{
			ObjectType::PROJECTILE,
			projectile
		};

		projectile->get_component<RigidBody>()->get_body()->setUserPointer(physicsType);

		projectiles.push_back(projectile);
		player->setFireCooldown(player->getFireballCooldown());
		break;
	}
}

void GameScene::update_physics(float delta_time) {
	world->stepSimulation(delta_time);

	btCollisionObject* playerObj = playerRb->get_body();

	int numManifolds = world ? world->getDispatcher()->getNumManifolds() : 0;
	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* manifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* objA = (btCollisionObject*)(manifold->getBody0());
		btCollisionObject* objB = (btCollisionObject*)(manifold->getBody1());

		PhysicsType* dataA = static_cast<PhysicsType*>(objA->getUserPointer());

		PhysicsType* dataB = static_cast<PhysicsType*>(objB->getUserPointer());

		switch (dataA->type)
		{
			case ObjectType::PLAYER:
				if (dataB->type == ObjectType::PROJECTILE) {
					continue;
				}

				if (dataB->type == ObjectType::ENEMY) {
					
					Enemy* enemy = static_cast<Enemy*>(dataB->object);

					if (enemy->getAttackCooldown() <= 0) {
						enemy->attack();
						player->takeDamage(enemy->getDamage());
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().enemyAttackSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().playerHitSound, SoundManager::get_instance().surrounding_sounds, playerTransform->position);
					}
				}

				if (dataB->type == ObjectType::PICKUP) {
					Pickup* pickup1 = static_cast<Pickup*>(dataB->object);

					pickup1->onPickup(player.get());
					pickup1->setIsAlive(false);
				}
				
				break;
			case ObjectType::ENEMY:

				if (dataB->type == ObjectType::PROJECTILE) {
					Enemy* enemy = static_cast<Enemy*>(dataA->object);
					Projectile* projectile = static_cast<Projectile*>(dataB->object);

					if (auto* projectileObj = dynamic_cast<MagicProjectile*>(projectile)) {
						enemy->takeDamage(projectileObj->getDamage());
						projectileObj->setIsAlive(false);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().magicHitSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
					}
					else if (auto* projectileObj = dynamic_cast<FreezeProjectile*>(projectile)) {
						enemy->takeDamage(projectileObj->getDamage());
						enemy->slowDown();
						projectileObj->setIsAlive(false);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().freezeHitSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
					}
					else if (auto* projectileObj = dynamic_cast<FireballProjectile*>(projectile)) {
						enemy->takeDamage(projectileObj->getDamage());
						projectileObj->setIsAlive(false);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().fireballHitSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
					}
				}

				if (dataB->type == ObjectType::PLAYER) {
					Enemy* enemy = static_cast<Enemy*>(dataA->object);

					if (enemy->getAttackCooldown() <= 0) {
						enemy->attack();
						player->takeDamage(enemy->getDamage());
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().enemyAttackSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().playerHitSound, SoundManager::get_instance().surrounding_sounds, playerTransform->position);
					}
				}

				break;
			case ObjectType::PROJECTILE:
				if (dataB->type == ObjectType::PLAYER) {
					continue;
				}

				if (dataB->type == ObjectType::ENEMY) {
					Enemy* enemy = static_cast<Enemy*>(dataB->object);
					MagicProjectile* projectile = static_cast<MagicProjectile*>(dataA->object);

					if (auto* projectileObj = dynamic_cast<MagicProjectile*>(projectile)) {
						enemy->takeDamage(projectileObj->getDamage());
						projectileObj->setIsAlive(false);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().magicHitSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
					}
					else if (auto* projectileObj = dynamic_cast<FreezeProjectile*>(projectile)) {
						enemy->takeDamage(projectileObj->getDamage());
						enemy->slowDown();
						projectileObj->setIsAlive(false);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().freezeHitSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
					}
					else if (auto* projectileObj = dynamic_cast<FireballProjectile*>(projectile)) {
						enemy->takeDamage(projectileObj->getDamage());
						projectileObj->setIsAlive(false);
						SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().fireballHitSound, SoundManager::get_instance().surrounding_sounds, enemy->get_component<Transform>()->position);
					}
				}
				break;
			case ObjectType::PICKUP:
				if (dataB->type == ObjectType::PLAYER) {
					Pickup* pickup1 = static_cast<Pickup*>(dataA->object);

					pickup1->onPickup(player.get());
					pickup1->setIsAlive(false);
				}
				break;
			default:
				break;
		}
		if (manifold->getNumContacts() > 0) {
			if (dataA->type == ObjectType::PROJECTILE) {
				Projectile* projectile = static_cast<Projectile*>(dataA->object);
				projectile->setIsAlive(false);
				break;
			}
			else if (dataB->type == ObjectType::PROJECTILE) {
				Projectile* projectile = static_cast<Projectile*>(dataB->object);
				projectile->setIsAlive(false);
				break;
			}
		}
	}
}

void GameScene::update(float dt) {
	delta_time = dt;
	player->handleInput(camera.get(), world, projectiles, delta_time);
	camera->update();

	if (Input::get_key_down('E'))
		Input::set_cursor_lock(is_cursor_locked = !is_cursor_locked);

	if (roundTimer > 0 && round != 5) {
		roundTimer -= dt;
	}
	else if (roundTimer <= 0) {
		roundTimer = 24.0f;
		round++;
	}

	if(spawnTime > 0)
		spawnTime -= dt;
	else {
		spawnTime = 10.0f;
		spawnEnemies();
	}

	for (auto& tree : trees) {
		tree->update(dt);
	}
	for (auto& wall : invisibleWalls) {
		wall->update(dt);
	}

	auto it = projectiles.begin();
	while (it != projectiles.end()) {
		if (!(*it)->getIsAlive()) {
			delete *it;
			it = projectiles.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto& projectile : projectiles) {
		projectile->update(dt);
	}

	auto enemyIt = enemies.begin();
	while (enemyIt != enemies.end()) {
		if (!(*enemyIt)->getIsAlive()) {
			score += (*enemyIt)->getPointsWorth();
			dropPickup((*enemyIt)->get_component<Transform>()->position);
			delete *enemyIt;
			enemyIt = enemies.erase(enemyIt);
		}
		else {
			++enemyIt;
		}
	}

	for (auto& enemy : enemies) {
		enemy->update(dt);
	}

	auto pickupIt = pickups.begin();
	while (pickupIt != pickups.end()) {
		if (!(*pickupIt)->getIsAlive()) {
			delete* pickupIt;
			pickupIt = pickups.erase(pickupIt);
		}
		else {
			++pickupIt;
		}
	}

	for (auto& pickup : pickups) {
		pickup->update(dt);
		glm::quat yaws = glm::angleAxis(glm::radians(1.0f), glm::vec3(0, 1, 0));
		pickup->get_component<Transform>()->rotation = yaws * pickup->get_component<Transform>()->rotation;

		glm::quat& rot = pickup->get_component<Transform>()->rotation;
		btTransform bt = pickup->get_component<RigidBody>()->get_body()->getWorldTransform();
		bt.setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
		pickup->get_component<RigidBody>()->get_body()->setWorldTransform(bt);
		pickup->get_component<RigidBody>()->get_body()->getMotionState()->setWorldTransform(bt);
	}

	if (!player->getIsAlive()) {
		PlayerPrefs::getInstance()->addScore(score);
		PlayerPrefs::getInstance()->setLastScore(score);
		SceneManager::getInstance()->changeScene("GameOver");
	}

	healthText->get_component<Text>()->setText("Health: " + std::to_string(player->getHealth()));
	scoreText->get_component<Text>()->setText("Score: " + std::to_string(score));

	if (player->getSelectedWeapon() == Magic) {
		magicImg->get_component<Transform>()->scale = glm::vec3(0.075f, 0.075f, 0.05f);
		freezeImg->get_component<Transform>()->scale = glm::vec3(0.05f, 0.05f, 0.05f);
		fireballImg->get_component<Transform>()->scale = glm::vec3(0.0325f, 0.045f, 0.05f);
	}
	else if (player->getSelectedWeapon() == Freeze) {
		magicImg->get_component<Transform>()->scale = glm::vec3(0.05f, 0.05f, 0.05f);
		freezeImg->get_component<Transform>()->scale = glm::vec3(0.075f, 0.075f, 0.05f);
		fireballImg->get_component<Transform>()->scale = glm::vec3(0.0325f, 0.045f, 0.05f);
	}
	else if (player->getSelectedWeapon() == Fireball) {
		magicImg->get_component<Transform>()->scale = glm::vec3(0.05f, 0.05f, 0.05f);
		freezeImg->get_component<Transform>()->scale = glm::vec3(0.05f, 0.05f, 0.05f);
		fireballImg->get_component<Transform>()->scale = glm::vec3(0.0525f, 0.06f, 0.05f);
	}

	map->update(dt);
	healthText->update(dt);
	scoreText->update(dt);
	player->update(dt);
	magicImg->update(dt);
	freezeImg->update(dt);
	fireballImg->update(dt);
	playerHands->get_component<Transform>()->position = camera->get_position() + camera->get_forward() * 1.0f - camera->get_up() * 0.2f;

	glm::vec3 cameraRot = camera->get_rotation();
	glm::quat yaw = glm::angleAxis(glm::radians(-cameraRot.y), glm::vec3(0, 1, 0));
	glm::quat pitch = glm::angleAxis(glm::radians(-cameraRot.x), glm::vec3(1, 0, 0));
	glm::quat flip = glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0));
	playerHands->get_component<Transform>()->rotation = yaw * pitch * flip;

	SoundManager::get_instance().update();
	SoundManager::get_instance().update_listener(camera->get_position(), camera->get_forward(), camera->get_up());
	Input::update();
}

void GameScene::render2d() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMultMatrixf(glm::value_ptr(glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), -1.0f, 1.0f)));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);

	auto* tm = magicImg->get_component<Transform>();
	tm->rotation = glm::quat{ 1,0,0,0 };
	magicImg->render();

	auto* tf = freezeImg->get_component<Transform>();
	tf->rotation = glm::quat{ 1,0,0,0 };
	freezeImg->render();

	auto* tf2 = fireballImg->get_component<Transform>();
	tf2->rotation = glm::quat{ 1,0,0,0 };
	fireballImg->render();

	auto* tf3 = healthText->get_component<Transform>();
	tf3->rotation = glm::quat{ 1,0,0,0 };
	healthText->render();

	auto* tf4 = scoreText->get_component<Transform>();
	tf4->rotation = glm::quat{ 1,0,0,0 };
	scoreText->render();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void GameScene::render3d() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);

	for (auto& tree : trees) {
		tree->render();
	}
	for (auto& wall : invisibleWalls) {
		wall->render();
	}
	for (auto& projectile : projectiles) {
		projectile->render();
	}
	for (auto& enemy : enemies) {
		enemy->render();
	}
	player->render();

	glColor4f(1, 1, 1, 1);
	for (auto* pickup : pickups) {
		pickup->render();
	}

	if (player->getSelectedWeapon() == Magic)
		glColor4f(0.65f, 1.0f, 0.65f, 1.0f);
	else if (player->getSelectedWeapon() == Freeze)
		glColor4f(0.65f, 0.65f, 1.0f, 1.0f);
	else
		glColor4f(1.0f, 0.65f, 0.65f, 1.0f);
	
	playerHands->render();

	glColor4f(1, 1, 1, 1);
	map->render();

	//world->debugDrawWorld();

	glPopMatrix();

	glPopAttrib();
}

void GameScene::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->render();
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	render3d();
	render2d();

	glutSwapBuffers();
}

void GameScene::game_loop() {

}

void GameScene::cleanup()
{
	for (auto& tree : trees) {
		delete tree;
	}
	trees.clear();

	for (auto& wall : invisibleWalls) {
		delete wall;
	}
	invisibleWalls.clear();

	for (auto& enemy : enemies) {
		delete enemy;
	}
	enemies.clear();

	for (auto& projectile : projectiles) {
		delete projectile;
	}
	projectiles.clear();

	for (auto& spawn : enemySpawns) {
		delete spawn;
	}
	enemySpawns.clear();

	for (auto& pickup : pickups) {
		delete pickup;
	}
	pickups.clear();

	map.reset();
	magicImg.reset();
	freezeImg.reset();
	fireballImg.reset();
	healthText.reset();
	scoreText.reset();
	player.reset();
	playerTransform = nullptr;
	playerRb = nullptr;

	if (world) {
		delete world;
		world = nullptr;
	}
	if (solver) {
		delete solver;
		solver = nullptr;
	}
	if (dispatcher) {
		delete dispatcher;
		dispatcher = nullptr;
	}
	if (collision_configuration) {
		delete collision_configuration;
		collision_configuration = nullptr;
	}
	if (broadphase) {
		delete broadphase;
		broadphase = nullptr;
	}

	if (debug_drawer) {
		delete debug_drawer;
		debug_drawer = nullptr;
	}

	SoundManager::get_instance().stopMusic();

	score = 0;
	spawnTime = 7.5f;
	randomNum = 0;
	
	initialized = false;
}

void GameScene::onEnter()
{
	GameInstance::getInstance()->setCurrentGameScene(this);
	srand(time(0));
}

void GameScene::onExit()
{
	cleanup();
}