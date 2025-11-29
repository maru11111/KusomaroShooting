#pragma once
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"

struct Objects {
	std::unique_ptr<Player>player;
	Array<std::unique_ptr<BaseBullet>>marshmallows;
	Array<std::unique_ptr<BaseEnemy>>enemies;
};
