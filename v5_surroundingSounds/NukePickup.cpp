#include "NukePickup.h"
#include "GameScene.h"
class GameScene;

void NukePickup::onPickup(Player* player)
{
    GameScene* gameScene = GameInstance::getInstance()->getCurrentGameScene();

    gameScene->nukeEnemies();
}
