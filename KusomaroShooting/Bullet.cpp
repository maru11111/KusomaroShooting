﻿#include "stdafx.h"
#include "Bullet.h"

int BaseBullet::numInstances = 0;


BaseBullet::BaseBullet(Vec2 pos_)
	: pos{ pos_ }
{
	numInstances++;
	damageAmount = 10;
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
	if (pos.x < 0 - TextureAsset(U"Marshmallow").size().x || Scene::Size().x/3.0 + TextureAsset(U"Marshmallow").size().x < pos.x || pos.y < TextureAsset(U"UIBack").size().y*2 - TextureAsset(U"Marshmallow").size().y || Scene::Size().y / 3.0 + TextureAsset(U"Marshmallow").size().y < pos.y)return true;
	else return false;
}

bool BaseBullet::isDestroy() {
	if (isOffScreen())return true;
	else  if (isHit) return true;
	else return false;
}

int BaseBullet::getDamageAmount() {
	return damageAmount;
}

RectF BaseBullet::collision() {
	return RectF(Arg::center(pos), 7, 7);
}

MaroType BaseBullet::getType() {
	return type;
}

void BaseBullet::draw() {
	TextureAsset(U"Marshmallow").drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}




NormalMarshmallow::NormalMarshmallow(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 400;
	type = MaroType::Normal;
}

void NormalMarshmallow::move() {
	pos += vec * speed * Scene::DeltaTime();
}




KusoMarshmallowUp::KusoMarshmallowUp(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 400;
	type = MaroType::Up;
}

void KusoMarshmallowUp::move()
{
	vec.y -= 1.5 * Scene::DeltaTime();
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowUp::draw() {
	TextureAsset(U"KusomaroUp").drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}



KusoMarshmallowDown::KusoMarshmallowDown(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 400;
	type = MaroType::Down;
}

void KusoMarshmallowDown::move()
{
	vec.y += 1.5 * Scene::DeltaTime();
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowDown::draw() {
	TextureAsset(U"KusomaroDown").drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 20, 20).draw(ColorF(1, 0, 0));
}



KusoMarshmallowSine::KusoMarshmallowSine(Vec2 pos_)
	: BaseBullet(pos_)
{
	vec = { 1,0 };
	speed = 300;
	type = MaroType::Sine;
}

void KusoMarshmallowSine::move()
{
	vec.y = Math::Sin(Scene::Time() * 7);
	pos += vec * speed * Scene::DeltaTime();
}

void KusoMarshmallowSine::draw() {
	TextureAsset(U"KusomaroSine").drawAt(pos);
	//Debug
	//RectF(Arg::center(pos), 7, 7).draw(ColorF(1, 0, 0));
}
