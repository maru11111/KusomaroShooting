#include "stdafx.h"
#include "Enemy.h"
#include "Objects.h"

BaseEnemy::BaseEnemy(Objects& objects_, Vec2 pos_)
	: objects{ objects_ }
	, pos{ pos_ }
{}

void BaseEnemy::attack() {

}

void BaseEnemy::update() {
	move();
}

void BaseEnemy::damage(int damageAmount) {
	hp -= damageAmount;
}

bool BaseEnemy::isDestroy() {
	if (pos.x < 0 - 50 || Scene::Size().x + 50 < pos.x || pos.y < 0 - 50 || Scene::Size().y + 50 < pos.y)return true;
	else if (hp <= 0) return true;
	else false;
}




GarbageBag::GarbageBag(Objects& objects_, Vec2 pos_)
	: BaseEnemy(objects_, pos_)
{
	vec = { -1,0 };
	speed = 150;

}

void GarbageBag::move() {
	pos += vec * speed * Scene::DeltaTime();
}

RectF GarbageBag::collision()const{
	return RectF(Arg::center(pos), 20, 20);
}

void GarbageBag::draw() {
	
	TextureAsset(U"GarbageBag").rotatedAt({TextureAsset(U"GarbageBag").size().x / 2, TextureAsset(U"GarbageBag").size().y / 2 + 5}, Scene::Time() * 1 * (-360_deg)).drawAt(pos);

	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1,0,0));
	//RectF(Arg::center(pos + Vec2{0,5}), 3, 3).draw(ColorF(1, 0, 0));
}
