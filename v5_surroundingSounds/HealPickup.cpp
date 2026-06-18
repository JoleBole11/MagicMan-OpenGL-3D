#include "HealPickup.h"

void HealPickup::onPickup(Player* player)
{
	player->addHealth(25);
}
