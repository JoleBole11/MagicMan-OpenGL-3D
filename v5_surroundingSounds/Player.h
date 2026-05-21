#pragma once
#include "GameObject.h"
class Player : public GameObject
{
private:
	int health = 100;
	int fireCooldown = 0;
	int damage = 25;
		
public:
	Player(const std::string& name) : GameObject(name) {}
};

