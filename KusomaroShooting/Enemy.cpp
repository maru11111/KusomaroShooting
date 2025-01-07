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
	attack();
}

void BaseEnemy::damage(int damageAmount) {
	hp -= damageAmount;
	if (hp < 0)hp = 0;
}

bool BaseEnemy::isDestroy() {
	if (pos.x < 0 - 50  || Scene::Size().x + Scene::Size().x < pos.x  || pos.y < 0 - Scene::Size().y || Scene::Size().y + 50 < pos.y)return true;
	if (hp <= 0) return true;
	else return false;
}

int BaseEnemy::getDamageAmount() {
	return damageAmount;
}
Vec2 BaseEnemy::getPos() {
	return pos;
}




GarbageBagNormal::GarbageBagNormal(Objects& objects_, Vec2 pos_)
	: BaseEnemy(objects_, pos_)
{
	maxHp = 10;
	hp = maxHp;
	vec = { -1,0 };
	speed = 75*3;
	damageAmount = 1;
}

void GarbageBagNormal::move() {
	pos += vec * speed * Scene::DeltaTime();
	if (pos.x <= Scene::Size().x / 3.5) {
		speed *= 1.01;
	}
}

TwoQuads GarbageBagNormal::collision()const{
	return (Quad)RectF(Arg::center(pos), 7*3, 7*3);
}

void GarbageBagNormal::draw() {
	
	TextureAsset(U"GarbageBag").scaled(3).rotatedAt({TextureAsset(U"GarbageBag").size().x * 3 / 2.0, TextureAsset(U"GarbageBag").size().y * 3 / 2.0 + 5}, Scene::Time() * 1 * (-360_deg)).drawAt(pos);

	//int n = (int)(Scene::Time() / 0.125*3.0/2.0) % 8;
	
	//TextureAsset(U"GarbageBag")(n * TextureAsset(U"GarbageBag").size().x/8.0, 0, TextureAsset(U"GarbageBag").size().x / 8.0, TextureAsset(U"GarbageBag").size().y ).scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1,0,0));
	//RectF(Arg::center(pos), 7, 7).draw(ColorF(1, 0, 0));
}

GarbageBagFast::GarbageBagFast(Objects& objects_, Vec2 pos_)
	: BaseEnemy(objects_, pos_)
{
	maxHp = 10;
	hp = maxHp;
	vec = { -1,0 };
	speed = 75 * 3 * 2;
	damageAmount = 1;
}

void GarbageBagFast::move() {
	pos += vec * speed * Scene::DeltaTime();
	if (pos.x <= Scene::Size().x / 4) {
		speed *= 1.01;
	}
}

TwoQuads GarbageBagFast::collision()const {
	return (Quad)RectF(Arg::center(pos), 7 * 3, 7 * 3);
}

void GarbageBagFast::draw(){
	TextureAsset(U"GarbageBag").scaled(3).rotatedAt({ TextureAsset(U"GarbageBag").size().x * 3 / 2.0, TextureAsset(U"GarbageBag").size().y * 3 / 2.0 + 5 }, Scene::Time() * 1 * (-360_deg)).drawAt(pos);
}



GarbageBagWithCan::GarbageBagWithCan(Objects& objects_, Vec2 pos_)
	: BaseEnemy(objects_, pos_)
{
	maxHp = 10;
	hp = maxHp;
	vec = { -1,0 };
	speed = 75;
	damageAmount = 1;
}

void GarbageBagWithCan::move(){
	pos += vec * speed * Scene::DeltaTime();
	if (pos.x <= Scene::Size().x / 4) {
		speed *= 1.01;
	}
}

void GarbageBagWithCan::attack() {
	timer += Scene::DeltaTime();

	Print << objects.enemies.size();

	if (attackInterval <= timer) {
		timer -= attackInterval;
		//缶を飛ばす
		objects.enemies << std::make_unique<Can>(objects, pos, (objects.player->getPos() - pos).normalized());
	}
}

TwoQuads GarbageBagWithCan::collision()const {
	return (Quad)RectF(Arg::center(pos), 7 * 3, 7 * 3);
}

void GarbageBagWithCan::draw() {
	TextureAsset(U"GarbageBag")
		.scaled(3)
		.rotatedAt({ TextureAsset(U"GarbageBag").size().x * 3 / 2.0, TextureAsset(U"GarbageBag").size().y * 3 / 2.0 + 5 }, Scene::Time() * 1 * (-360_deg))
		.drawAt(pos);
}


Can::Can(Objects& objects_, Vec2 pos_, Vec2 vec_)
	: BaseEnemy(objects_, pos_)
{
	maxHp = 10;
	hp = maxHp;
	vec = vec_;
	speed = 75*3*2;
	damageAmount = 1;
}

void Can::move() {
	pos += vec * speed * Scene::DeltaTime();
}

TwoQuads Can::collision()const {
	return (Quad)RectF(Arg::center(pos), 15, 15);
}

void Can::draw() {
	TextureAsset(U"RedCan").scaled(3).rotated(vec.getAngle()).drawAt(pos);
}

Fish::Fish(Objects& objects_, Vec2 pos_)
	:BaseEnemy(objects_, pos_)
{
	maxHp = 10;
	hp = maxHp;
	speed = 75 * 3;
	damageAmount = 1;
	vec = (objects.player->getPos() - pos).normalized();
}

void Fish::move() {
	timer += Scene::DeltaTime();

	if (moveInterval <= timer) {
		isMoving = true;
		timer = 0;
	}

	if (isMoving) {
		if (objects.player != nullptr) vec = (objects.player->getPos() - pos).normalized();
		pos += vec * speed * Scene::DeltaTime();
	}
	else {

	}
}

TwoQuads Fish::collision()const {
	return RectF(Arg::center(pos), 30, 10).rotated(vec.getAngle()+90_deg);
}

void Fish::draw() {
	if (isMoving) {
		int n = (int)(timer / 0.100) % 10;
		TextureAsset(U"Fish")(n * TextureAsset(U"Fish").size().x / 10.0, 0, TextureAsset(U"Fish").size().x / 10.0, TextureAsset(U"Fish").size().y).scaled(4).rotated(vec.getAngle()).drawAt(pos);
		if (n == 10 - 1) isMoving = false;
	}
	else {
		int n = (int)(timer / 0.150) % 10;
		TextureAsset(U"FishWait")(n * TextureAsset(U"Fish").size().x / 10.0, 0, TextureAsset(U"Fish").size().x / 10.0, TextureAsset(U"Fish").size().y).scaled(4).rotated(vec.getAngle()).drawAt(pos);
	}
	//Debug
	//RectF(Arg::center(pos), 30, 10).rotated(vec.getAngle()+90_deg).draw();
}


Umbrella::Umbrella(Objects& objects_, Vec2 pos_)
	: BaseEnemy{ objects_, pos_ }
{
	maxHp = 10;
	hp = maxHp;
	damageAmount = 1;
}

void Umbrella::move() {
	currentVelX = - maxVelX * Math::Sin(timer);

	Print << abs(currentVelX) / maxVelX;

	angle = 30_deg * (currentVelX / maxVelX);

	pos += Vec2(currentVelX -30, gravity)*Scene::DeltaTime();

	timer += Scene::DeltaTime();
}

TwoQuads Umbrella::collision()const {
	return TwoQuads((Quad)RectF(Arg::center(pos.movedBy(0, -13)), 90, 35).rotated(angle), (Quad)(RectF(Arg::center(pos.movedBy(0, 7)), 10, 95).rotated(angle)) ) ;
}

void Umbrella::draw() {
	TextureAsset(U"Umbrella").scaled(3).rotated(angle-45_deg).drawAt(pos);
	//Debug
	//今回はcol1だけ使用
	//RectF(Arg::center(pos.movedBy(0, -13)), 90, 35).rotated(angle).draw();
	//RectF(Arg::center(pos.movedBy(0, 7)), 10, 95).rotated(angle).draw();
}
