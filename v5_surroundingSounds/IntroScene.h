#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Font.h"
#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>
#include "Sprite.h"
#include "Text.h"

class IntroScene : public Scene {
protected:
    float duration;
    float timer;

    float delta_time = 0;
    float previous_time = 0;

    static std::unique_ptr<Font> shared_font;
    std::unique_ptr<GameObject> text;

public:
    IntroScene();
    ~IntroScene() override;

    void initialize() override;
    void update(float deltaTime) override;
	void update_physics(float deltaTime) override;
    void render() override;
	void render2d() override;
	void render3d() override;
    void cleanup() override;
	void game_loop() override;

    void onEnter() override;
    void onExit() override;
};