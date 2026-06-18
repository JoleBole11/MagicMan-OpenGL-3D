#pragma once
#include "Pickup.h"
class HealPickup : public Pickup
{
public:
	HealPickup(const std::string& name) : Pickup(name) {}

	void onPickup(Player* player) override;
};

