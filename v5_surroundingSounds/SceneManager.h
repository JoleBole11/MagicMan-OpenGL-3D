#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Scene;

class SceneManager {
private:
    static SceneManager* instance;
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
    Scene* currentScene;
    Scene* nextScene;
    bool shouldChangeScene;

    SceneManager() : currentScene(nullptr), nextScene(nullptr), shouldChangeScene(false) {}

public:
    static SceneManager* getInstance();

    void addScene(const std::string& name, std::unique_ptr<Scene> scene);
    void changeScene(const std::string& name);
    void update(float deltaTime);
	void update_physics(float deltaTime);
    void render();

    Scene* getCurrentScene() const { return currentScene; }

    Scene* getScene(const std::string& name) const;

    void reshape(int width, int height);
	void game_loop();

    ~SceneManager();
};