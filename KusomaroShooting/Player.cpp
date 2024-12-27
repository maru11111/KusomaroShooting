#include "stdafx.h"
#include "Objects.h"
#include "Player.h"

Player::Player(Objects& objects_)
	:objects{ objects_ }
{
	for (int i = 0; i < maxNumMarshmallows; i++) {
		addMarshmallow();
	}
}

void Player::update() {

	//Debug
	//for (auto& maro : maroBox)Print << (int)maro;
	//int a[5] = { 0,0,0,0,0 };
	//for (auto& maro : maroBox){
	//	switch (maro) {
	//	case MaroType::Normal:a[0]++; break;
	//	case MaroType::Up: a[1]++; break;
	//	case MaroType::Down: a[2]++; break;
	//	case MaroType::Sine: a[3]++; break;
	//	case MaroType::Beem: a[4]++; break;
	//	}
	//}
	//Print << a;

	move();

	if (KeyJ.down() || KeySpace.down() || KeyEnter.down()) {
		attack();
	}

	if (numMarshmallows < maxNumMarshmallows) {
		marshmallowAddTimer += Scene::DeltaTime();

		if (marshmallowAddInterval < marshmallowAddTimer) {
			marshmallowAddTimer -= marshmallowAddInterval;
			addMarshmallow();
		}
	}
	else marshmallowAddTimer = 0;

}

void Player::move() {
	//上
	if (KeyW.pressed() || KeyUp.pressed())vec.y = -1;
	//下
	if (KeyS.pressed() || KeyDown.pressed())vec.y = 1;
	//左
	if (KeyA.pressed() || KeyUp.pressed())vec.x = -1;
	//右
	if (KeyD.pressed() || KeyUp.pressed())vec.x = 1;

	//斜め移動補正
	if (abs(vec.x) == 1 || abs(vec.y) == 1)vec.setLength(1);

	//移動
	pos += vec * speed * Scene::DeltaTime();

	//ふわふわさせる
	pos.y += 10 * Math::Sin(Scene::Time() * 1.25) * Scene::DeltaTime();

	//画面外に出ない
	if (pos.x < 0)pos.x = 0;
	if (Scene::Size().x < pos.x)pos.x = Scene::Size().x;
	if (pos.y < 0)pos.y = 0;
	if (Scene::Size().y < pos.y)pos.y = Scene::Size().y;

	//簡易慣性
	vec *= 0.9;
}

void Player::attack() {

	if (0 < numMarshmallows) {
		switch (maroBox[0]) {
		case MaroType::Normal: objects.marshmallows << std::make_unique<NormalMarshmallow>(pos); break;
		case MaroType::Up: objects.marshmallows << std::make_unique<KusoMarshmallowUp>(pos); break;
		case MaroType::Down: objects.marshmallows << std::make_unique<KusoMarshmallowDown>(pos); break;
		case MaroType::Sine: objects.marshmallows << std::make_unique<KusoMarshmallowSine>(pos); break;
		case MaroType::Beem: Print << U"未実装"; break;
		}
		maroBox.pop_front();
		numMarshmallows--;
	}
}

void Player::damage(int damageAmount) {
	hp -= damageAmount;
}

MaroType Player::chooseMaro() {

	if (not (0 <= normalMaroAppearProbability || normalMaroAppearProbability <= 100))throw Error{ U"normalMaroAppearProbabilityの値が不正" };

	//normalMaroAppearProbabilityの確率でふつうマロ生成
	if (Random(100) < normalMaroAppearProbability) {
		return MaroType::Normal;
	}

	//クソマロだった場合
	if (upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability + beemMaroAppearProbability != 100)throw Error{ U"クソマロの出現確率が不正" };

	int num = Random(99);

	if (num < upMaroAppearProbability) return MaroType::Up;
	else if (num < upMaroAppearProbability + downMaroAppearProbability) return MaroType::Down;
	else if (num < upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability) return MaroType::Sine;
	else if (num < upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability + beemMaroAppearProbability)return MaroType::Beem;

}

void Player::addMarshmallow() {
	if (numMarshmallows < maxNumMarshmallows) {

		numMarshmallows++;

		MaroType addType = chooseMaro();

		switch (addType) {
		case MaroType::Normal:maroBox << MaroType::Normal; break;
		case MaroType::Up:maroBox << MaroType::Up; break;
		case MaroType::Down:maroBox << MaroType::Down; break;
		case MaroType::Sine:maroBox << MaroType::Sine; break;
		case MaroType::Beem:maroBox << MaroType::Beem; break;
		}
	}
	else if (maxNumMarshmallows < numMarshmallows) {
		numMarshmallows = maxNumMarshmallows;
	}
}

RectF Player::collision() const {
	return RectF(Arg::center(pos), 50, 50);
}

int Player::getNumMarshmallows()const {
	return numMarshmallows;
}
int Player::getMaxNumMarshmallows()const {
	return maxNumMarshmallows;
}
int Player::getHp()const {
	return hp;
}
int Player::getMaxHp()const {
	return maxHp;
}
MaroType Player::getNextMaro()const {
	if (0 < numMarshmallows)return maroBox[0];
	else return MaroType::Empty;
}

void Player::draw() {

	//スプライトシートを再生

	int n = (int)(Scene::Time() / 0.125) % 3;

	TextureAsset(U"UiNormal")(n * 35, 0, 35, 28).drawAt(pos);

	//Debug
	//RectF(Arg::center(pos), 50, 50).draw(ColorF(1, 0, 0));
}
