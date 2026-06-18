#pragma once
#include "GameObject.h"
#include "Player.h"
class Pickup : public GameObject
{
private:
	float disappearTimer = 6;

	bool isAlive = true;

public:
	Pickup(const std::string& name) : GameObject(name) {}

	void update(float delta_time);
	virtual void onPickup(Player* player) = 0;

	void setIsAlive(bool _isAlive);
	bool getIsAlive();

	float getDisappearTimer();
};

