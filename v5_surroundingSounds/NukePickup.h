#pragma once
#include "Pickup.h"
class NukePickup : public Pickup
{
public:
	NukePickup(const std::string& name) : Pickup(name) {}

	void onPickup(Player* player) override;
};

