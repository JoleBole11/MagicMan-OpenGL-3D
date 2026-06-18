#include "SpeedPickup.h"

void SpeedPickup::onPickup(Player* player)
{
	player->activateSpeedPower();
}
