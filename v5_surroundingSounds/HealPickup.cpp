#include "HealPickup.h"

void HealPickup::onPickup(Player* player)
{
	SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().healPickupSound, SoundManager::get_instance().surrounding_sounds, player->get_component<Transform>()->position);
	player->addHealth(25);
}
