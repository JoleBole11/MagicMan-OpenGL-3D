#include "CooldownPickup.h"

void CooldownPickup::onPickup(Player* player)
{
	player->activateCooldownPower();
}
