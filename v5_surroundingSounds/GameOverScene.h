#pragma once
#include "Scene.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Font.h"
#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>
#include "Sprite.h"
#include "Text.h"
#include "Input.h"
#include "SceneManager.h"
class GameOverScene : public Scene
{
protected:
	float delta_time = 0;
	float previous_time = 0;

	static std::unique_ptr<Font> shared_font;
	std::unique_ptr<GameObject> loseText;
	std::unique_ptr<GameObject> score;
	std::unique_ptr<GameObject> continueText;
	std::unique_ptr<GameObject> restartText;
public:
	GameOverScene() : Scene("Menu") {}
	~GameOverScene() override;

	void initialize() override;
	void update(float deltaTime) override;
	void update_physics(float deltaTime) override;
	void render() override;
	void game_loop() override;
	void render2d() override;
	void render3d() override;
	void cleanup() override;

	void onEnter() override;
	void onExit() override;
};

