#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Input.h"
#include <vector>
#include "SoundManager.h"
#include "GameObject.h"
#include "Utilities.h"
#include "Camera.h"
#include "Input.h"

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

class GameScene : public Scene
{
public:
	float delta_time = 0;
	float previous_time = 0;

	float movement_speed = 5.0f;
	float rotation_speed = 3.0f;

	glm::vec2 window_size = { 600, 600 };

	bool is_cursor_locked = false;

	std::unique_ptr<Camera> camera;

	std::unique_ptr<GameObject> map;
	std::unique_ptr<GameObject> tree;
	std::unique_ptr<GameObject> rocket;
	std::unique_ptr<GameObject> text;

	std::vector<std::vector<int>> treeMap;
	std::vector<GameObject*> trees;
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

