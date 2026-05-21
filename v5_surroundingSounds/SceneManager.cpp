#include "SceneManager.h"
#include "Scene.h"
#include <freeglut.h>
#include <iostream>

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::getInstance() {
    if (!instance) {
        instance = new SceneManager();
    }
    return instance;
}

void SceneManager::addScene(const std::string& name, std::unique_ptr<Scene> scene) {
    scenes[name] = std::move(scene);
}

void SceneManager::changeScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        nextScene = it->second.get();
        shouldChangeScene = true;
    }
    else {
        std::cerr << "Scene '" << name << "' not found." << std::endl;
    }
}

void SceneManager::update(float deltaTime) {
    if (shouldChangeScene && nextScene) {
        if (currentScene) {
            currentScene->onExit();
        }

        currentScene = nextScene;
        nextScene = nullptr;
        shouldChangeScene = false;

        if (!currentScene->isInitialized()) {
            currentScene->initialize();
        }
        currentScene->onEnter();
    }

    if (currentScene) {
        currentScene->update(deltaTime);
    }
}

void SceneManager::update_physics(float deltaTime)
{
    if (currentScene) {
        currentScene->update_physics(deltaTime);
    }
}

void SceneManager::render() {
    if (currentScene) {
        currentScene->render();
    }
}


Scene* SceneManager::getScene(const std::string& name) const
{
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        return it->second.get();
    }
    return nullptr;
}

void SceneManager::game_loop() {
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    update(deltaTime);
	update_physics(deltaTime);
    render();

    glutPostRedisplay();

    if (currentScene) {
        currentScene->game_loop();
    }
}

void SceneManager::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

SceneManager::~SceneManager() {
    scenes.clear();
}