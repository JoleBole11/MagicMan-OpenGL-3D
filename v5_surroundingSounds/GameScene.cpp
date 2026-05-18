#include "GameScene.h"
#include "Scene.h"
#include "GameObject.h"
#include "Input.h"
#include <vector>
#include "SoundManager.h"
#include "Utilities.h"
#include "Camera.h"

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

std::unique_ptr<Font> GameScene::shared_font = nullptr;

GameScene::GameScene() : Scene("Game")
{
	
}

GameScene::~GameScene() {
	cleanup();
}

void GameScene::initialize() {
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	world->setGravity(btVector3(0, -9.81f, 0));

	debug_drawer = new GLDebugDrawer();
	debug_drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	world->setDebugDrawer(debug_drawer);

	camera = std::make_unique<Camera>(60.0f, float(window_size[0]) / float(window_size[1]), 0.1f, 300.0f);

	skull = std::make_unique<GameObject>("Skull");
	skull->add_component<MeshRenderer>("models/skull/skull_downloadable.obj");

	auto* t = skull->get_component<Transform>();
	t->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	t->position = glm::vec3(0.0f, 0.0f, -3.0f);

	auto* shape = new btBoxShape(btVector3(1, 1, 1));
	skull->add_component<RigidBody>(1.0f, shape, world);

	skull1 = std::make_unique<GameObject>("Skull");
	skull1->add_component<MeshRenderer>("models/skull/skull_downloadable.obj");

	auto* t1 = skull1->get_component<Transform>();
	t1->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	t1->position = glm::vec3(0.0f, -5.0f, -3.0f);

	auto* shape1 = new btBoxShape(btVector3(1, 1, 1));
	skull1->add_component<RigidBody>(0.0f, shape1, world);

	rocket = std::make_unique<GameObject>("Sprite");
	rocket->add_component<Sprite>("sprites/rocket.png");

	auto* t2 = rocket->get_component<Transform>();
	if (t2) {
		t2->scale = glm::vec3(0.3f, 0.3f, 0.3f);
	}

	shared_font = std::make_unique<Font>(
		"fonts/Roboto-Regular.ttf",
		24
	);

	text = std::make_unique<GameObject>("Text");
	text->add_component<Text>(
		shared_font.get(),
		"Print this!",
		glm::vec3(1.0f, 0.84f, 0.0f)
	);
	text->get_component<Transform>()->position = glm::vec3(250.0f, 550.0f, 0.0f);
}

void GameScene::move_camera() {
	float v = Input::get_axis("Vertical");
	float h = Input::get_axis("Horizontal");

	glm::vec3 movement =
		camera->get_forward() * v * movement_speed * delta_time
		+ camera->get_right() * h * movement_speed * delta_time;

	camera->set_position(
		camera->get_position() + movement
	);

	float x = 0.0, y = 0.0;
	Input::get_mouse_position(&x, &y);

	float yaw = camera->get_rotation().y + float(-x * rotation_speed * delta_time);
	float pitch = camera->get_rotation().x + float(-y * rotation_speed * delta_time);

	pitch = glm::clamp(pitch, -359.0f, 359.0f);

	camera->set_rotation(glm::vec3(pitch, yaw, 0.0f));
}

void GameScene::update_physics(float delta_time) {

	world->stepSimulation(delta_time);
}

void GameScene::update(float dt) {

	move_camera();
	camera->update();

	if (Input::get_key_down('E'))
		Input::set_cursor_lock(is_cursor_locked = !is_cursor_locked);

	if (Input::get_key_down('R'))
		SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().first_sound, SoundManager::get_instance().surrounding_sounds, glm::vec3(4, 0, 4));
	else if (Input::get_key_down('T'))
		SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().second_sound, SoundManager::get_instance().surrounding_sounds, glm::vec3(1, 0, 1));

	skull->update(dt);
	skull1->update(dt);
	rocket->update(dt);
	text->update(dt);

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

	auto* tf = rocket->get_component<Transform>();
	tf->rotation = glm::quat{ 1,0,0,0 };
	rocket->render();

	auto* tf2 = text->get_component<Transform>();
	tf2->rotation = glm::quat{ 1,0,0,0 };
	text->render();

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

	skull->render();
	skull1->render();

	world->debugDrawWorld();

	glPopMatrix();

	glPopAttrib();
}

void GameScene::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->render();

	render3d();
	render2d();

	glutSwapBuffers();
}

void GameScene::game_loop() {
	float current_time = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (float)((current_time - previous_time) / 1000);
	previous_time = current_time;

	update(delta_time);
	update_physics(delta_time);
	render();

	glutSwapBuffers();
	glutPostRedisplay();
}

void GameScene::cleanup()
{
	delete &skull;
	delete &skull1;
	delete &rocket;
	delete &text;
	delete &camera;
	delete debug_drawer;
	debug_drawer = nullptr;
}

void GameScene::onEnter()
{
	GameInstance::getInstance()->setCurrentGameScene(this);
}

void GameScene::onExit()
{
}
