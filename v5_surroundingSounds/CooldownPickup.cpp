#include "CooldownPickup.h"

void CooldownPickup::onPickup(Player* player)
{
	SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().cooldownPickupSound, SoundManager::get_instance().surrounding_sounds, player->get_component<Transform>()->position);
	player->activateCooldownPower();
}
