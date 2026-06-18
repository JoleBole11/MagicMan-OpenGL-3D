#pragma once
enum CollisionGroup {
    CG_DEFAULT = 1,
    CG_PLAYER = 2,
    CG_ENEMY = 4,
    CG_PROJECTILE = 8,
    CG_PICKUP = 16
};