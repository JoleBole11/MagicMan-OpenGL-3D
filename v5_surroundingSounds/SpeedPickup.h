#pragma once
#include "Pickup.h"
class SpeedPickup : public Pickup
{
public:
	SpeedPickup(const std::string& name) : Pickup(name) {}

	void onPickup(Player* player) override;
};

