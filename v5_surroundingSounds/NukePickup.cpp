#include "NukePickup.h"
#include "GameScene.h"

void NukePickup::onPickup(Player* player)
{
    GameScene* gameScene = GameInstance::getInstance()->getCurrentGameScene();

    gameScene->nukeEnemies();
}
