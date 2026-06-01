#pragma once
#include "GameObject.h"
class MagicProjectile : public GameObject
{
private:
	float lifetime = 7.0f;
	float damage = 50.0f;
};

