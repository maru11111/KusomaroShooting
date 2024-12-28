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
	if (hp < 0)hp = 0;
}

bool BaseEnemy::isDestroy() {
	if (pos.x < 0 - 50  || Scene::Size().x / 3 + 50 < pos.x  || pos.y < 0 - 50 || Scene::Size().y / 3 + 50 < pos.y)return true;
	if (hp <= 0) return true;
	else return false;
}

int BaseEnemy::getDamageAmount() {
	return damageAmount;
}
Vec2 BaseEnemy::getPos() {
	return pos;
}


GarbageBag::GarbageBag(Objects& objects_, Vec2 pos_)
	: BaseEnemy(objects_, pos_)
{
	maxHp = 10;
	hp = maxHp;
	vec = { -1,0 };
	speed = 75;
	damageAmount = 1;

}

void GarbageBag::move() {
	pos += vec * speed * Scene::DeltaTime();
}

RectF GarbageBag::collision()const{
	return RectF(Arg::center(pos), 7, 7);
}

void GarbageBag::draw() {
	
	//TextureAsset(U"GarbageBag").rotatedAt({TextureAsset(U"GarbageBag").size().x / 2, TextureAsset(U"GarbageBag").size().y / 2 + 5}, Scene::Time() * 1 * (-360_deg)).drawAt(pos);

	int n = (int)(Scene::Time() / 0.125*3.0/2.0) % 8;
	
	TextureAsset(U"GarbageBag")(n * TextureAsset(U"GarbageBag").size().x/8.0, 0, TextureAsset(U"GarbageBag").size().x / 8.0, TextureAsset(U"GarbageBag").size().y ).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1,0,0));
	//RectF(Arg::center(pos), 7, 7).draw(ColorF(1, 0, 0));
}
