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

class MainMenuScene : public Scene
{
protected:
	float delta_time = 0;
	float previous_time = 0;

	static std::unique_ptr<Font> shared_font;
	std::unique_ptr<GameObject> gameName;
	std::unique_ptr<GameObject> playText;
	std::unique_ptr<GameObject> quitText;
	std::unique_ptr<GameObject> highScoreText;
	std::unique_ptr<GameObject> highScore1;
	std::unique_ptr<GameObject> highScore2;
	std::unique_ptr<GameObject> highScore3;
public:
	MainMenuScene() : Scene("Menu") {}
	~MainMenuScene() override;

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

