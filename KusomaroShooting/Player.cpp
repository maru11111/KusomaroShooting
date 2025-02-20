#include "stdafx.h"
#include "Objects.h"
#include "Player.h"
#include "Common.h"

struct AttackEffect : IEffect
{
	int n = 0;
	double timer = 0;
	Vec2 pos;

	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit AttackEffect(Vec2 pos_)
		: pos{ pos_ } {}

	bool update(double t) override
	{
		//スプライトシートを再生
		int n = (int)(timer / (0.035 / 1.5)) % 21;
		TextureAsset(U"UiAttackEffect")
			(n * TextureAsset(U"UiAttackEffect").size().x / 21.0, 0, TextureAsset(U"UiAttackEffect").size().x / 21.0, TextureAsset(U"UiAttackEffect").size().y)
			.scaled(3)
			.drawAt(pos);

		timer += Scene::DeltaTime();

		//trueの間継続
		return (n < 21 - 1);
	}
};

Player::Player(Objects& objects_)
	:objects{ objects_ }
{
	for (int i = 0; i < maxNumMarshmallows; i++) {
		addMarshmallow();
	}
}

Player::Player(Objects& objects_, Vec2 pos_)
	: objects{ objects_ }
	, pos{ pos_ }
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
	//	case MaroType::Beam: a[4]++; break;
	//	}
	//}
	//Print << U"aaaa";

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

		pos.x -= hitBackSpeed * Scene::DeltaTime();

		hitBackSpeed *= 0.9;

		//画面外に出ない
		stayOnScreen();


		if (hitBackTime <= hitBackTimer) {
			//ヒットバック終了
			hitBackSpeed = firstHitBackSpeed;
			isHitBack = false;
		}
	}

	//ビーム中なら
	if (isBeamAttacking) {
		beamTimer += Scene::DeltaTime();

		//後退する動きのイージング
		//const double e = EaseOutBack(beamTimer/(75 * 0.05));

		//後ろに下がる
		if (13 * 0.05 < beamTimer) {
			pos.x -= 3 * Scene::DeltaTime();
		}

		if (75 * 0.050 < beamTimer) {
			isBeamAttacking = false;
			beamTimer = 0;
		}
	}

	//近接攻撃
	if (isMovable() && KeyT.pressed() && not isAttack) {
		isAttack = true;
		isAttackColOn = true;
		isAttackEffectStarted = false;
		//近接攻撃効果音
		AudioManager::Instance()->play(U"CloseRangeAttack");
	}

	//近接攻撃中なら
	if (isAttack) {
		attackTimer += Scene::DeltaTime();

		//エフェクト追加
		if (not isAttackEffectStarted) {
			effect.add<AttackEffect>(pos.movedBy(20 * 3 + 5, -20));
			isAttackEffectStarted = true;
		}

		if (attackColTime <= attackTimer) {
			isAttackColOn = false;
		}

		if (attackTime <= attackTimer) {
			isAttack = false;
			attackTimer = 0;
		}
	}
	
	//移動
	if (isMovable()) {
		move();
	}

	//マシュマロを投げる
	if (isMovable()) {
		if (KeyJ.down() || KeySpace.down() || KeyEnter.down()) {
			attack();
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

	//攻撃中は右に進めない
	if (isAttack)if (vec.x == 1)vec.x = 0;

	//斜め移動補正
	if (abs(vec.x) == 1 || abs(vec.y) == 1)vec.setLength(1);

	//移動
	pos += vec * speed * Scene::DeltaTime();

	//ふわふわさせる
	pos.y += 6 * Math::Sin(Scene::Time() * 1.25) * Scene::DeltaTime();

	//画面外に出ない
	stayOnScreen();

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
		case MaroType::Beam:
			objects.marshmallows << std::make_unique<KusoMarshmallowBeam>(pos);
			isBeamAttacking = true;
			break;
		}

		//マシュマロ(or beam)を投げる音
		if (maroBox[0] != MaroType::Beam) AudioManager::Instance()->play(U"Throw");
		else {
			AudioManager::Instance()->play(U"BeamStart");
			AudioManager::Instance()->play(U"Beam");
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
		//攻撃をキャンセル
		isAttackColOn = false;
		isAttack = false;
		attackTimer = 0;
		//ヒットバック開始
		isHitBack = true;
		hitBackTimer = 0;
		//効果音
		AudioManager::Instance()->play(U"ReceiveDamage");
	}
}

MaroType Player::chooseMaro() {

	if (not (0 <= normalMaroAppearProbability || normalMaroAppearProbability <= 100))throw Error{ U"normalMaroAppearProbabilityの値が不正" };

	//normalMaroAppearProbabilityの確率でふつうマロ生成
	if (Random(100) < normalMaroAppearProbability) {
		return MaroType::Normal;
	}

	//クソマロだった場合
	if (upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability + beamMaroAppearProbability != 100)throw Error{ U"クソマロの出現確率が不正" };

	int num = Random(99);

	if (num < upMaroAppearProbability) return MaroType::Up;
	else if (num < upMaroAppearProbability + downMaroAppearProbability) return MaroType::Down;
	else if (num < upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability) return MaroType::Sine;
	else if (num < upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability + beamMaroAppearProbability)return MaroType::Beam;

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
		case MaroType::Beam:maroBox << MaroType::Beam; break;
		}
	}
	else if (maxNumMarshmallows < numMarshmallows) {
		numMarshmallows = maxNumMarshmallows;
	}
}

void Player::stayOnScreen() {
	if (pos.x < 0)pos.x = 0;
	if (Scene::Size().x < pos.x)pos.x = Scene::Size().x;
	if (pos.y < 60 * 3)pos.y = 60 * 3;
	if (Scene::Size().y < pos.y)pos.y = Scene::Size().y;
}

void Player::setKusomaro(MaroType type) {
	if (not maroBox.empty())maroBox[0] = type;
	else maroBox << type;
}

RectF Player::bodyCollision() const {
	return RectF(Arg::center(pos.movedBy(5 * 3, -5 * 3)), 15 * 3, 15 * 3);
}
RectF Player::attackCollision()const {
	return RectF(Arg::center(pos.movedBy(20 * 3, 0)), 12 * 3, 22 * 3);
}

void Player::startInvincibilityTime() {
	remainingInvincibilityTime = invincibilityTime;
}
bool Player::isInvincibility() {
	if (0 < remainingInvincibilityTime) return true;
	else return false;
}

bool Player::isMovable() {
	if (not isHitBack /*&& not isAttack*/ && not isBeamAttacking) return true;
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
int Player::getDamageAmount()const {
	return damageAmount;
}
bool Player::getIsAttackColOn()const {
	return isAttackColOn;
}
bool Player::getIsBeamAttacking() {
	return isBeamAttacking;
}

Vec2 Player::getPos() {
	return pos;
}

void Player::drawForAttack(double opacity) {
	int n = (int)(Scene::Time() / 0.0625) % 102;

	//角度のイージング
	const double eR = EaseOutCubic(attackTimer * 2);

	//前進速度のイージング
	const double eS = EaseOutExpo(attackTimer * 2);

	TextureAsset(U"UiNormalAndBlink")
		(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y)
		.scaled(3).rotated(eR * 2 * (-1) * 360 * attackTime * Math::Pi / 180.0)
		.drawAt(pos.moveBy(eS * 10 * Scene::DeltaTime() + 5 * Math::Cos(2 * 2 * Math::Pi * attackTimer), -5 * Math::Sin(2 * 2 * Math::Pi * attackTimer)), ColorF(1.0, opacity));
}

void Player::drawEffect() {
	effect.update();
}

void Player::draw() {

	//Debug
	//RectF(pos.movedBy(60, -TextureAsset(U"UiBeam").size().y / 15.0 - 32), Min(TextureAsset(U"UiBeam").size().x/5 * 3, Scene::Size().x) , TextureAsset(U"UiBeam").size().y/15 * 3).draw();

	//スプライトシートを再生
	//int n = (int)(Scene::Time() / 0.05) % (5*15);

	//TextureAsset(U"UiBeam")( (n % 5) * TextureAsset(U"UiBeam").size().x / 5, (int)(n/5) * TextureAsset(U"UiBeam").size().y/15 , TextureAsset(U"UiBeam").size().x/5, TextureAsset(U"UiBeam").size().y/15 ).scaled(3).draw(pos.movedBy(40, -TextureAsset(U"UiBeam").size().y/15.0 -32));

	//無敵時間中
	if (isInvincibility()) {
		//ヒットバック中
		if (isHitBack) {
			//点滅させる
			int isDrawClear = (int)(Scene::Time() / (0.125 / 1.5)) % 2;
			if (isDrawClear) {
				//スプライトシートを再生
				int n = (int)(Scene::Time() / 0.250) % 3;
				TextureAsset(U"UiDamage")(n * TextureAsset(U"UiDamage").size().x / 3.0, 0, TextureAsset(U"UiDamage").size().x / 3.0, TextureAsset(U"UiDamage").size().y).scaled(3).drawAt(pos);
			}
			else {
				//スプライトシートを再生
				int n = (int)(Scene::Time() / 0.250) % 3;
				TextureAsset(U"UiDamage")(n * TextureAsset(U"UiDamage").size().x / 3.0, 0, TextureAsset(U"UiDamage").size().x / 3.0, TextureAsset(U"UiDamage").size().y).scaled(3).drawAt(pos, ColorF(1.0, 0.8));
			}
		}
		//攻撃中
		else if (isAttack) {
			//点滅させる
			int isDrawClear = (int)(Scene::Time() / (0.125 / 1.5)) % 2;
			if (isDrawClear) {
				drawForAttack(1.0);
			}
			else {
				drawForAttack(0.8);
			}
		}
		//それ以外
		else {
			//スプライトシートを再生
			int n = (int)(Scene::Time() / 0.0625) % 102;
			//点滅させる
			int isDrawClear = (int)(Scene::Time() / (0.125 / 1.5)) % 2;
			if (isDrawClear) {
				TextureAsset(U"UiNormalAndBlink")(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y).scaled(3).drawAt(pos, ColorF(1.0, 1.0));
			}
			else {
				TextureAsset(U"UiNormalAndBlink")(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y).scaled(3).drawAt(pos, ColorF(1.0, 0.8));
			}
		}
	}
	//近接攻撃中
	else if (isAttack) {
		drawForAttack(1.0);
	}
	//ビーム中
	else if (isBeamAttacking) {
		//スプライトシートを再生
		int n = (int)(beamTimer / 0.0625) % 84;
		TextureAsset(U"UiBeamUimama")(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y).scaled(3).drawAt(pos);
	}
	//通常
	else {
		//スプライトシートを再生
		int n = (int)(Scene::Time() / 0.0625) % 102;
		TextureAsset(U"UiNormalAndBlink")(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y).scaled(3).drawAt(pos);
	}

	//Debug
	//int n = (int)(Scene::Time() / 0.05) % 51;
	//TextureAsset(U"UiBeam")(n * TextureAsset(U"UiBeam").size().x / 51, 0, TextureAsset(U"UiBeam").size().x / 51, TextureAsset(U"UiBeam").size().y).drawAt(pos);

	//Debug
	//RectF(Arg::center(pos.movedBy(5,-5)), 15, 15).draw(ColorF(1, 0, 0));

	//Debug
	//if(isAttackColOn) RectF(Arg::center(pos.movedBy(20 * 3, 0)), 12 * 3, 22 * 3).draw();
}
