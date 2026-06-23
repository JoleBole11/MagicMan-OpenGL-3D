#include "NukePickup.h"
#include "GameScene.h"
class GameScene;

void NukePickup::onPickup(Player* player)
{
    SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().nukePickupSound, SoundManager::get_instance().surrounding_sounds, player->get_component<Transform>()->position);
    GameScene* gameScene = GameInstance::getInstance()->getCurrentGameScene();

    gameScene->nukeEnemies();
}
