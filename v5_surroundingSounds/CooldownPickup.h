#pragma once
#include "Pickup.h"
class CooldownPickup : public Pickup
{
private:
	
public:
	CooldownPickup(const std::string& name) : Pickup(name) {}

	void onPickup(Player* player) override;
};