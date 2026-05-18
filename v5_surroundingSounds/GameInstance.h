#pragma once

class GameScene;

class GameInstance {
private:
    static GameInstance* instance;
    GameScene* currentGameScene;

    GameInstance() : currentGameScene(nullptr) {}

public:
    static GameInstance* getInstance();
    static GameScene* getCurrentGameScene();

    void setCurrentGameScene(GameScene* scene);


    ~GameInstance() = default;

    GameInstance(const GameInstance&) = delete;
    GameInstance& operator=(const GameInstance&) = delete;
};