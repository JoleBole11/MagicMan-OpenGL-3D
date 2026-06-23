#include "SpeedPickup.h"

void SpeedPickup::onPickup(Player* player)
{
	SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().speedPickupSound, SoundManager::get_instance().surrounding_sounds, player->get_component<Transform>()->position);
	player->activateSpeedPower();
}
