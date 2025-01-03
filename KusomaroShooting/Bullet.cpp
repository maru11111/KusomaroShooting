#include "stdafx.h"
#include "Bullet.h"

int BaseBullet::numInstances = 0;


BaseBullet::BaseBullet(Vec2 pos_)
	: pos{ pos_ }
{
	numInstances++;
	damageAmount = 10;
	Print << numInstances;
}

BaseBullet::~BaseBullet() {
	numInstances--;
	Print << numInstances;
}

void BaseBullet::update() {
	move();
}

void BaseBullet::setIsHit(bool isHit_) {
	isHit = isHit_;
}

bool BaseBullet::isOffScreen() {
	if (pos.x < 0 - TextureAsset(U"Marshmallow").size().x*3 || Scene::Size().x + TextureAsset(U"Marshmallow").size().x*3 < pos.x || pos.y < TextureAsset(U"UIBack").size().y*6 - TextureAsset(U"Marshmallow").size().y*3 || Scene::Size().y + TextureAsset(U"Marshmallow").size().y*3 < pos.y)return true;
	else return false;
}

bool BaseBullet::isDestroy() {
	if (isOffScreen())return true;
	//貫通攻撃に上方修正
	//else  if (isHit) return true;
	else return false;
}

int BaseBullet::getDamageAmount() {
	return damageAmount;
}

RectF BaseBullet::collision() {
	return RectF(Arg::center(pos), 7*3, 7*3);
}

MaroType BaseBullet::getType() {
	return type;
}

void BaseBullet::draw() {
	TextureAsset(U"Marshmallow").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}




NormalMarshmallow::NormalMarshmallow(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 400*2;
	type = MaroType::Normal;
}

void NormalMarshmallow::move() {
	pos += vec * speed * Scene::DeltaTime();
}




KusoMarshmallowUp::KusoMarshmallowUp(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 400*2;
	type = MaroType::Up;
}

void KusoMarshmallowUp::move()
{
	vec.y -= 1.5  * Scene::DeltaTime();
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowUp::draw() {
	TextureAsset(U"KusomaroUp").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}



KusoMarshmallowDown::KusoMarshmallowDown(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 400*2;
	type = MaroType::Down;
}

void KusoMarshmallowDown::move()
{
	vec.y += 1.5 * Scene::DeltaTime();
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowDown::draw() {
	TextureAsset(U"KusomaroDown").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}



KusoMarshmallowSine::KusoMarshmallowSine(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 300*2;
	type = MaroType::Sine;
}

void KusoMarshmallowSine::move()
{
	vec.y = Math::Sin(timer*10 - 90_deg);
	pos += vec * speed * Scene::DeltaTime();

	timer += Scene::DeltaTime();
}

void KusoMarshmallowSine::draw() {
	TextureAsset(U"KusomaroSine").scaled(3).drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 7, 7).draw(ColorF(1, 0, 0));
}



KusoMarshmallowBeam::KusoMarshmallowBeam(Vec2 pos_)
	: BaseBullet(pos_)
{
	damageAmount = 1;
	type = MaroType::Beam;
}

void KusoMarshmallowBeam::update() {
	
	beamTimer += Scene::DeltaTime();
	beamColTimer += Scene::DeltaTime();

	if (isEndBeam) {
		isColliderActive = false;
	}

	if (isColliderActive) {
		isColliderActive = false;
	}

	if (beamColTime <= beamColTimer) {
		isColliderActive = true;
		beamColTimer -= beamColTime;
	}
}

RectF KusoMarshmallowBeam::collision() {
	if (isColliderActive) return RectF(pos.movedBy(60, -TextureAsset(U"UiBeam").size().y / 15.0 - 32), Min(TextureAsset(U"UiBeam").size().x / 5 * 3, Scene::Size().x), TextureAsset(U"UiBeam").size().y / 15 * 3);
	else RectF(0, 0, 0, 0);
}

void KusoMarshmallowBeam::move() {
	//動かない
}

bool KusoMarshmallowBeam::isDestroy() {
	if (isEndBeam) return true;
	else false;
}

void KusoMarshmallowBeam::draw() {
	//スプライトシートを再生
	if (not isEndBeam) {
		int n = (int)(beamTimer / 0.05) % (5 * 15);
		Print << n;
		//最後のシートに到達したか
		if (n == 5 * 15 - 1) isEndBeam = true;

		TextureAsset(U"UiBeam")((n % 5) * TextureAsset(U"UiBeam").size().x / 5, (int)(n / 5) * TextureAsset(U"UiBeam").size().y / 15, TextureAsset(U"UiBeam").size().x / 5, TextureAsset(U"UiBeam").size().y / 15).scaled(3).draw(pos.movedBy(40, -TextureAsset(U"UiBeam").size().y / 15.0 - 32));
	}

	//if(isColliderActive)RectF(pos.movedBy(60, -TextureAsset(U"UiBeam").size().y / 15.0 - 32), Min(TextureAsset(U"UiBeam").size().x / 5 * 3, Scene::Size().x), TextureAsset(U"UiBeam").size().y / 15 * 3).draw();
}
