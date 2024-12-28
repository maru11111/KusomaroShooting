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

Player::~Player() {
	maroBox.clear();
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
	//Print << U"aaaa";

	//移動
	if (isMovable()) {
		move();
	}

	//攻撃
	if (KeyJ.down() || KeySpace.down() || KeyEnter.down()) {
		attack();
	}

	//時間経過でマシュマロ補充
	if (numMarshmallows < maxNumMarshmallows) {
		marshmallowAddTimer += Scene::DeltaTime();

		if (marshmallowAddInterval < marshmallowAddTimer) {
			marshmallowAddTimer -= marshmallowAddInterval;
			addMarshmallow();
		}
	}
	else marshmallowAddTimer = 0;

	//無敵時間があったら更新
	if (isInvincibility()) {
		remainingInvincibilityTime -= Scene::DeltaTime();
		if (remainingInvincibilityTime < 0)remainingInvincibilityTime = 0;
	}

	//ヒットバック中なら
	if (isHitBack) {
		hitBackTimer += Scene::DeltaTime();

		pos.x -= hitBackSpeed *Scene::DeltaTime();

		hitBackSpeed *= 0.9;

		//画面外に出ない
		if (pos.x < 0)pos.x = 0;
		if (Scene::Size().x / 3.0 < pos.x)pos.x = Scene::Size().x / 3.0;
		if (pos.y < 60)pos.y = 60;
		if (Scene::Size().y / 3.0 < pos.y)pos.y = Scene::Size().y / 3.0;


		if (hitBackTime <= hitBackTimer) {
			//ヒットバック終了
			hitBackSpeed = firstHitBackSpeed;
			isHitBack = false;
		}
	}
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
	pos.y += 6 * Math::Sin(Scene::Time() * 1.25) * Scene::DeltaTime();

	//画面外に出ない
	if (pos.x < 0)pos.x = 0;
	if (Scene::Size().x/3.0 < pos.x)pos.x = Scene::Size().x/3.0;
	if (pos.y < 60)pos.y = 60;
	if (Scene::Size().y/3.0 < pos.y)pos.y = Scene::Size().y/3.0;

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
	//無敵時間中
	if (isInvincibility()) {
		//ダメージを受けない
		Print << U"MUTEKI";
	}
	//無敵時間外
	else {
		//無敵時間スタート
		startInvincibilityTime();
		//ダメージを受ける
		hp -= damageAmount;
		//0以下になったら0に戻す
		if (hp < 0)hp = 0;
		//ヒットバック開始
		isHitBack = true;
		hitBackTimer = 0;
	}
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

void Player::setKusomaro(MaroType type) {
	if (not maroBox.empty())maroBox[0] = type;
	else maroBox << type;
}

RectF Player::collision() const {
	return RectF(Arg::center(pos.movedBy(5, -5)), 15, 15);
}

void Player::startInvincibilityTime() {
	remainingInvincibilityTime = invincibilityTime;
}
bool Player::isInvincibility() {
	if (0 < remainingInvincibilityTime) return true;
	else return false;
}

bool Player::isMovable() {
	if (not isHitBack) return true;
	else return false;
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

	//無敵時間中
	if (isInvincibility()) {
		//点滅させる
		int isDraw = (int)(Scene::Time() / (0.125 /1.5)) % 2;
		if (isDraw) {
			//スプライトシートを再生
			int n = (int)(Scene::Time() / 0.250) % 3;
			TextureAsset(U"UiDamage")(n * TextureAsset(U"UiDamage").size().x/3.0, 0, TextureAsset(U"UiDamage").size().x/3.0, TextureAsset(U"UiDamage").size().y).drawAt(pos);
		}
		else {
			//スプライトシートを再生
			int n = (int)(Scene::Time() / 0.250) % 3;
			TextureAsset(U"UiDamage")(n * TextureAsset(U"UiDamage").size().x/3.0, 0, TextureAsset(U"UiDamage").size().x/3.0, TextureAsset(U"UiDamage").size().y).drawAt(pos,ColorF(1.0, 0.8));
		}
	}
	//無敵時間外
	else {
		//スプライトシートを再生
		int n = (int)(Scene::Time() / 0.0625) % 102;
		TextureAsset(U"UiNormalAndBlink")(n * TextureAsset(U"UiNormalAndBlink").size().x/102, 0, TextureAsset(U"UiNormalAndBlink").size().x/102, TextureAsset(U"UiNormalAndBlink").size().y).drawAt(pos);
	}

	//Debug
	//RectF(Arg::center(pos.movedBy(5,-5)), 15, 15).draw(ColorF(1, 0, 0));
}
