#include "Player.h"

void Player::update(float dt) {
	if (fireCooldown > 0) {
		fireCooldown -= dt;
	}
	if (Input::get_mouse_button_down(0) && fireCooldown <= 0) {
		FireProjectile();
		fireCooldown = 1.5f;
	}
}

void Player::FireProjectile() {
	GameScene* gameScene = GameInstance::getCurrentGameScene();
	switch (selectedWeapon)
	{
	case Magic:
		
		if (gameScene) {
			gameScene->SpawnProjectile();
		}
		break;
	case Freeze:
		break;
	case Fireball:
		break;
	}
}