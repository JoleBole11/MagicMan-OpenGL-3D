#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Input.h"
#include <vector>
#include "SoundManager.h"
#include "Utilities.h"
#include "Camera.h"
#include "MagicProjectile.h"
#include "FreezeProjectile.h"
#include "FireballProjectile.h"
#include "Player.h"
#include "BasicEnemy.h"
#include "HeavyEnemy.h"
#include "ObjectType.h"
#include "PhysicsType.h"

#include <ctime>
#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>
#include "Sprite.h"
#include "Text.h"

#include "DebugDrawer.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "btBulletDynamicsCommon.h"
#include "MotionStateObject.h"

#include "GameInstance.h"
#include "SceneManager.h"



class GameScene : public Scene
{
public:
	float delta_time = 0;
	float previous_time = 0;
	float spawnTime = 7.5f;

	float movement_speed = 5.0f;
	float rotation_speed = 3.0f;

	int score = 0;
	int randomNum = 0;

	glm::vec2 window_size = { 1000, 1000 };

	bool is_cursor_locked = false;

	std::unique_ptr<Camera> camera;

	std::unique_ptr<Player> player;
	Transform* playerTransform = nullptr;
	RigidBody* playerRb = nullptr;

	std::unique_ptr<GameObject> map;
	std::unique_ptr<GameObject> tree;
	std::unique_ptr<GameObject> rocket;
	std::unique_ptr<GameObject> magicImg;
	std::unique_ptr<GameObject> freezeImg;
	std::unique_ptr<GameObject> fireballImg;
	std::unique_ptr<GameObject> healthText;
	std::unique_ptr<GameObject> scoreText;

	std::vector<std::vector<int>> treeMap;
	std::vector<GameObject*> trees;
	std::vector<Projectile*> projectiles;
	std::vector<GameObject*> enemySpawns;
	std::vector<Enemy*> enemies;
	std::vector<GameObject*> invisibleWalls;

	static std::unique_ptr<Font> shared_font;

	btBroadphaseInterface* broadphase;
	btCollisionConfiguration* collision_configuration;
	btCollisionDispatcher* dispatcher;
	btConstraintSolver* solver;
	btDynamicsWorld* world;

	GLDebugDrawer* debug_drawer;

public:
	GameScene();
	~GameScene() override;

	void move_camera();
	void move_player();
	void SpawnProjectile();
	void EnemySpawner(const glm::vec3& pos);
	void SpawnEnemy(const glm::vec3& pos);

	void initialize() override;
	void update(float dt) override;
	void update_physics(float dt) override;
	void render() override;
	void game_loop() override;
	void render2d() override;
	void render3d() override;
	void cleanup() override;
	void onEnter() override;
	void onExit() override;
};

