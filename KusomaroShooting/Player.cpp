﻿#include "stdafx.h"
#include "Objects.h"
#include "Player.h"
#include "Common.h"

struct AttackEffect : IEffect
{
	int n = 0;
	double *timer = 0;
	double timerOffset;
	Vec2 pos;

	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit AttackEffect(Vec2 pos_, double *timer_)
		: pos{ pos_ }
		, timer{timer_}
		, timerOffset{*timer_}
	{ }

	bool update(double t) override
	{
		//スプライトシートを再生
		int n = (int)( (*timer - timerOffset) / (0.035 / 1.5)) % 21;
		TextureAsset(U"UiAttackEffect")
			(n * TextureAsset(U"UiAttackEffect").size().x / 21.0, 0, TextureAsset(U"UiAttackEffect").size().x / 21.0, TextureAsset(U"UiAttackEffect").size().y)
			.scaled(3)
			.drawAt(pos);

		//trueの間継続
		return (n < 21 - 1);
	}
};

struct MoveEffect : IEffect {

	Vec2 pos;
	int size;
	ColorF color;

	explicit MoveEffect(Vec2 pos_)
		: pos{pos_ + RandomVec2(RectF(0.0, 0.0, -20.0, 20.0))}
		, size{Random(5, 15)}
	{
		switch (Random(0, 3)) {
		case 0: color = ColorF(229 / 256.0, 222 / 256.0,  66 / 256.0); break;
		case 1: color = ColorF( 97 / 256.0, 126 / 256.0, 209 / 256.0); break;
		case 2: color = ColorF(129 / 256.0, 201 / 256.0,  76 / 256.0); break;
		case 3: color = ColorF(255 / 256.0, 144 / 256.0, 144 / 256.0); break;
		}

		//color = HSV{ (Random(0, 6) * 60) };
	}

	bool update(double t)override {
		RectF(pos.movedBy(-30 * t, 30 * t), size).draw( ColorF(color, Max(0.0, 0.7-t)) );
		return t < 1.0;
	}
};

struct HealEffect : IEffect
{
	int n = 0;
	double *timer;
	double timerOffset;
	Vec2 *pos;

	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit HealEffect(Vec2 *pos_, double* timer_)
		: pos{ pos_ }
		, timer{timer_}
		, timerOffset{*timer}
	{}

	bool update(double t) override
	{
		//Print << U"heal";
		//スプライトシートを再生
		if(pos!=nullptr)drawSpriteAnimForTimerAt(U"HealEffect", 17, 0.045, *pos, *timer-timerOffset);

		//trueの間継続
		return (currentFrame(17, 0.045, *timer-timerOffset) != 17);
	}
};

Player::Player(Objects& objects_)
	:objects{ objects_ }
{
	for (int i = 0; i < maxNumMarshmallows; i++) {
		numMarshmallows++;

		MaroType addType = chooseMaro();

		switch (addType) {
		case MaroType::Normal:maroBox << MaroType::Normal; break;
		case MaroType::Heal:maroBox << MaroType::Heal; break;
		case MaroType::Up:maroBox << MaroType::Up; break;
		case MaroType::Down:maroBox << MaroType::Down; break;
		case MaroType::Sine:maroBox << MaroType::Sine; break;
		case MaroType::Beam:maroBox << MaroType::Beam; break;
		}
	}
}

Player::Player(Objects& objects_, Vec2 pos_)
	: objects{ objects_ }
	, pos{ pos_ }
{
	for (int i = 0; i < maxNumMarshmallows; i++) {
		numMarshmallows++;

		MaroType addType = chooseMaro();

		switch (addType) {
		case MaroType::Normal:maroBox << MaroType::Normal; break;
		case MaroType::Heal:maroBox << MaroType::Heal; break;
		case MaroType::Up:maroBox << MaroType::Up; break;
		case MaroType::Down:maroBox << MaroType::Down; break;
		case MaroType::Sine:maroBox << MaroType::Sine; break;
		case MaroType::Beam:maroBox << MaroType::Beam; break;
		}
	}
	prevPos = pos_;
}

Player::~Player() {
	maroBox.clear();
}

void Player::dyingUpdate() {
	drawTimer += Scene::DeltaTime();
	pos += Vec2{-90 - 80 * Math::Cos(Math::TwoPi*Scene::Time()), 100-40*Math::Sin(Math::TwoPi*Scene::Time())} *Scene::DeltaTime();
}

void Player::update() {

	//effectTimer += Scene::DeltaTime();
	//if (0.05 <= effectTimer) {
	//	//筆のエフェクト
	//	effectBack.add<MoveEffect>(pos.movedBy(-50, 20));
	//	effectTimer -= 0.05;
	//}

	drawTimer += Scene::DeltaTime();

	//時間経過でマシュマロ補充
	if (numMarshmallows < maxNumMarshmallows) {
		marshmallowAddTimer += Scene::DeltaTime();

		if (marshmallowAddInterval < marshmallowAddTimer) {
			marshmallowAddTimer -= marshmallowAddInterval;
			addMarshmallow();
		}
	}
	else marshmallowAddTimer = 0;

	if (isMaroAdding) {
		maroAddBackAnimTimer += Scene::DeltaTime();
		if (0.5 <= maroAddBackAnimTimer)maroAddSumForAnim += maroAddAmountForAnim * Scene::DeltaTime();
		if (numMarshmallows <= prevMaroNum + maroAddSumForAnim) {
			isMaroAdding = false;
			maroAddSumForAnim = 0;
		}
	}


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
	if (isMovable() && (KeyZ.pressed() || KeyN.pressed() || (MouseL.pressed()&&RectF(0,0,Scene::Size()).contains(Cursor::Pos())) ) && not isAttack ) {
		isAttack = true;
		isAttackColOn = true;
		isAttackEffectStarted = false;

		//近接攻撃効果音
		AudioManager::Instance()->playOneShot(U"CloseRangeAttack");
	}

	//近接攻撃中なら
	if (isAttack) {
		attackTimer += Scene::DeltaTime();

		//角度のイージング
		eR = EaseOutCubic(attackTimer * 2);

		//前進速度のイージング
		eS = EaseOutExpo(attackTimer * 2);

		//当たり判定の座標オフセット
		bodyColAttackOffsetY_ = -5 * Math::Sin(2 * 2 * Math::Pi * attackTimer);

		//攻撃中の移動
		pos += Vec2{ eS * 10 * Scene::DeltaTime() + 5 * Math::Cos(2 * 2 * Math::Pi * attackTimer), -5 * Math::Sin(2 * 2 * Math::Pi * attackTimer) };

		//当たり判定のサイズを小さくする
		bodyColSize = {40, 40};

		//攻撃中に判定を動かさない
		bodyColAttackOffsetY += bodyColAttackOffsetY_;

		//エフェクト追加
		if (not isAttackEffectStarted) {
			effectBack.add<AttackEffect>(pos.movedBy(20 * 3 + 5, -20), &drawTimer);
			isAttackEffectStarted = true;
		}

		if (attackColTime <= attackTimer) {
			isAttackColOn = false;
		}

		if (attackTime <= attackTimer) {
			isAttack = false;
			attackTimer = 0;
			//当たり判定のサイズを戻す
			bodyColSize = { 45, 45 };
			bodyColAttackOffsetY_ = 0;
			bodyColAttackOffsetY  = 0;
		}
	}
	
	//移動
	if (isMovable()) {
		move();
	}

	//マシュマロを投げる
	if (isMovable()) {
		if (KeyX.down() || KeyM.down() || (MouseR.down() && RectF(0, 0, Scene::Size()).contains(Cursor::Pos())) ) {
			attack();
		}
	}

	//マシュマロを食べる
	//if (KeyC.down() && 3 <= numMarshmallows && not isEating) {
	//	//マシュマロを食べる。
	//	//効果音
	//	AudioManager::Instance()->playOneShot(U"Eat");
	//	isEating = true;
	//	eatTimer = 0;
	//	ateMaroType = maroBox[0];
	//	maroBox.pop_front();
	//	//3個消費
	//	numMarshmallows -= 3;
	//}
	//else if(KeyC.down()){
	//	AudioManager::Instance()->playOneShot(U"Beep");
	//}

	//マシュマロ食べ中
	if (isEating) {
		eatTimer += Scene::DeltaTime();
		if (0.25 <= eatTimer) {
			isEating = false;
			//食べたマシュマロの効果が発動
			switch (ateMaroType) {
			case MaroType::Normal:
				heal(1);
				break;
			case MaroType::Beam:
				heal(5);
				break;
			default:
				damage(1, true);
				effectBack.add<KusomaroTextEffect>(pos, BaseBullet::kusomaroTexts[Random(0, (int)BaseBullet::kusomaroTexts.size() - 1)]);
				break;
			}
		}
	}

	//回復中
	if (isHealing) {
		healBackAnimTimer += Scene::DeltaTime();
		if(0.5<=healBackAnimTimer)healSumForAnim += healAmountForAnim * Scene::DeltaTime();
		if (hp<=prevHpHeal+healSumForAnim) {
			isHealing = false;
			healSumForAnim = 0;
		}
	}

	//ダメージ時のhp減少アニメーション
	if (isDamageHpAnimation) {
		damageHpAnimTimer += Scene::DeltaTime();

		if (1.0 <= damageHpAnimTimer) {
			damageHpAnimEaseTimer += Scene::DeltaTime();

			if (1.0 <= damageHpAnimEaseTimer) {
				isDamageHpAnimation = false;
			}
		}
	}

	//最後の位置を記憶
	prevPos = pos;
}

void Player::defeatBossUpdate() {
	drawTimer += Scene::DeltaTime();

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

	//近接攻撃中なら
	if (isAttack) {
		attackTimer += Scene::DeltaTime();

		//当たり判定のサイズを小さくする
		bodyColSize = { 40, 40 };

		//攻撃中に判定を動かさない
		bodyColAttackOffsetY += bodyColAttackOffsetY_;

		//エフェクト追加
		if (not isAttackEffectStarted) {
			effectBack.add<AttackEffect>(pos.movedBy(20 * 3 + 5, -20), &drawTimer);
			isAttackEffectStarted = true;
		}

		if (attackColTime <= attackTimer) {
			isAttackColOn = false;
		}

		if (attackTime <= attackTimer) {
			isAttack = false;
			attackTimer = 0;
			//当たり判定のサイズを戻す
			bodyColSize = { 45, 45 };
			bodyColAttackOffsetY_ = 0;
			bodyColAttackOffsetY = 0;
		}
	}

	//回復中
	if (isHealing) {
		healBackAnimTimer += Scene::DeltaTime();
		if (0.5 <= healBackAnimTimer)healSumForAnim += healAmountForAnim * Scene::DeltaTime();
		if (hp <= prevHpHeal + healSumForAnim) {
			isHealing = false;
			healSumForAnim = 0;
		}
	}

	//ダメージ時のhp減少アニメーション
	if (isDamageHpAnimation) {
		damageHpAnimTimer += Scene::DeltaTime();

		if (1.0 <= damageHpAnimTimer) {
			damageHpAnimEaseTimer += Scene::DeltaTime();

			if (1.0 <= damageHpAnimEaseTimer) {
				isDamageHpAnimation = false;
			}
		}
	}

	//最後の位置を記憶
	prevPos = pos;
}

void Player::toMountainUpdate() {
	drawTimer += Scene::DeltaTime();
	toMountainTimer += Scene::DeltaTime();

	pos = prevPos.lerp(toMountainPos, EaseInQuad(toMountainTimer / 4.0));
}

void Player::bossAppearStateUpdate(double timer) {
	drawTimer += Scene::DeltaTime();
	double ease = EaseOutQuad(Min(timer/3.5, 1.0));
	pos = prevPos.lerp(basePos, ease);
}

void Player::toStartPos() {
	drawTimer += Scene::DeltaTime();
	toBaseTimer += Scene::DeltaTime();
	const double toBaseEase = Min(EaseOutCubic(toBaseTimer), 1.0);
	pos = Vec2{ -100, Scene::CenterF().y + TextureAsset(U"UIBack").size().y * 3 }.lerp(startPos, toBaseEase);
	if (1.0 <= toBaseTimer) {
	}
}

void Player::move() {
	//上
	if (KeyW.pressed() || KeyUp.pressed())vec.y = -1;
	//下
	if (KeyS.pressed() || KeyDown.pressed())vec.y = 1;
	//左
	if (KeyA.pressed() || KeyLeft.pressed())vec.x = -1;
	//右
	if (KeyD.pressed() || KeyRight.pressed())vec.x = 1;

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
	vecTimer += Scene::DeltaTime();
	if (1.0 / 60.0 <= vecTimer) {
		vec *= 0.75;
		vecTimer =0;
	}
	//vec.clear();
}

void Player::attack() {

	if (0 < numMarshmallows) {
		switch (maroBox[0]) {
		case MaroType::Normal: objects.marshmallows << std::make_unique<NormalMarshmallow>(objects, pos); break;
		case MaroType::Heal:
			objects.marshmallows << std::make_unique<HealMarshmallow>(objects, pos);
			heal(1);
			break;
		case MaroType::Up: objects.marshmallows << std::make_unique<KusoMarshmallowUp>(objects, pos); break;
		case MaroType::Down: objects.marshmallows << std::make_unique<KusoMarshmallowDown>(objects, pos); break;
		case MaroType::Sine: objects.marshmallows << std::make_unique<KusoMarshmallowSine>(objects, pos); break;
		case MaroType::Beam: objects.marshmallows << std::make_unique<KusoMarshmallowBeam>(objects, pos);break;
		}

		//マシュマロ(or beam or クソマロ)を投げる音
		if (maroBox[0] == MaroType::Normal) AudioManager::Instance()->playOneShot(U"Throw");
		else if (maroBox[0] == MaroType::Heal);
		else AudioManager::Instance()->playOneShot(U"Kusomaro");

		maroBox.pop_front();
		numMarshmallows--;
	}
	else {
		AudioManager::Instance()->playOneShot(U"Beep");
	}
}

void Player::damage(int damageAmount, bool piercingInv) {
	//無敵時間中
	if (isInvincibility() && not piercingInv) {
		//ダメージを受けない
		//Print << U"MUTEKI";
	}
	//無敵時間外
	else {
		//無敵時間スタート
		startInvincibilityTime();
		//ダメージを受ける前のhpを保存
		prevHpDamage = hp;
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
		AudioManager::Instance()->pauseAllAudio();
		//効果音
		AudioManager::Instance()->playOneShot(U"ReceiveDamage");
		//簡易慣性をリセット
		vec = Vec2(0,0);
		//ヒットストップを始める
		isHitStopStart = true;
		//ダメージ時のhp減少アニメーションのためのタイマー
		damageHpAnimTimer = 0;
		damageHpAnimEaseTimer = 0;
		// 〃 のためのフラグ
		isDamageHpAnimation = true;
	}
}

MaroType Player::chooseMaro() {

	if (not( 0 <= normalMaroAppearProbability + healMaroAppearProbability && normalMaroAppearProbability + healMaroAppearProbability <= 100))throw Error{ U"normalMaroAppearProbability || healMaroAPの値が不正" };

	//normalMaroAppearProbabilityの確率でふつうマロ生成
	int num = Random(99);
	if (num < normalMaroAppearProbability) return MaroType::Normal;
	else if (num < normalMaroAppearProbability + healMaroAppearProbability) return MaroType::Heal;

	//クソマロだった場合
	if (upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability + beamMaroAppearProbability != 100)throw Error{ U"クソマロの出現確率が不正" };
	num = Random(99);
	if (num < upMaroAppearProbability) return MaroType::Up;
	else if (num < upMaroAppearProbability + downMaroAppearProbability) return MaroType::Down;
	else if (num < upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability) return MaroType::Sine;
	else if (num < upMaroAppearProbability + downMaroAppearProbability + sineMaroAppearProbability + beamMaroAppearProbability)return MaroType::Beam;

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

void Player::setEditorPos() {
	pos = Vec2{ -150, Scene::Size().y / 2.0 };
}

RectF Player::bodyCollision() const {
	return RectF(Arg::center(pos.movedBy(5 * 3, -5 * 3 - bodyColAttackOffsetY)), bodyColSize);
}
RectF Player::attackCollision()const {
	//拡大後
	return RectF(Arg::center(pos.movedBy(20 * 3 - 30, 0)), 18 * 3 + 30, 22 * 3 + 15);
	//拡大前
	return RectF(Arg::center(pos.movedBy(20 * 3, 0)), 18 * 3, 22 * 3);
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
void Player::setIsBeamAttacking() {
	isBeamAttacking = true;
}

Vec2 Player::getPos() {
	return pos;
}
bool Player::getIsHitStopStart(){
	if (isHitStopStart) {
		isHitStopStart = false;
		return true;
	}
	else return false;
}

void Player::heal(int healAmount) {
	//効果音
	AudioManager::Instance()->playOneShot(U"Heal");
	//エフェクト
	effectFront.add<HealEffect>(&pos, &drawTimer);
	if (hp == maxHp) return;
	if (not isHealing) {
		prevHpHeal = hp;
		prevBackAnimHp = prevHpHeal;
		healBackAnimTimer = 0;
	}
	else {
		prevBackAnimHp = EaseOutExpo(healBackAnimTimer*2.0) * ((double)hp - prevBackAnimHp) + prevBackAnimHp;
		healBackAnimTimer = 0;
	}
	isHealing = true;
	hp += healAmount;
	if (maxHp < hp) hp = maxHp;
}
double Player::getHpEase() {
	if (isHealing) return Min(((double)prevHpHeal + healSumForAnim)/(double)maxHp, (double)hp / (double)maxHp);
	else return (double)hp / (double)maxHp;
}
double Player::getHpHealEase(){
	if (isHealing) return (Min(EaseOutExpo(healBackAnimTimer*2.0), 1.0)*((double)hp-prevBackAnimHp)+prevBackAnimHp)/(double)maxHp;
	else return 0.0;
}
double Player::getDamageEase() {
	if (isDamageHpAnimation) return ((1.0 - Min(EaseInQuart(damageHpAnimEaseTimer), 1.0))) * prevHpDamage / (double)maxHp;
	else return 0.0;
}

void Player::addMarshmallow() {
	if (numMarshmallows < maxNumMarshmallows) {

		if (not isMaroAdding) {
			prevMaroNum = numMarshmallows;
			prevBackAnimMaro = prevMaroNum;
			maroAddBackAnimTimer = 0;
		}
		else {
			prevBackAnimMaro = EaseOutExpo(maroAddBackAnimTimer * 1.5) * ((double)numMarshmallows - prevBackAnimMaro) + prevBackAnimMaro;
			maroAddBackAnimTimer = 0;
		}
		isMaroAdding = true;

		//効果音
		AudioManager::Instance()->playOneShot(U"AddMaro");

		numMarshmallows++;

		MaroType addType = chooseMaro();

		switch (addType) {
		case MaroType::Normal:maroBox << MaroType::Normal; break;
		case MaroType::Heal:maroBox << MaroType::Heal; break;
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
double Player::getMaroEase() {
	if (isMaroAdding) return Min(((double)prevMaroNum + maroAddSumForAnim) / (double)maxNumMarshmallows, (double)numMarshmallows / (double)maxNumMarshmallows);
	else return (double)numMarshmallows / (double)maxNumMarshmallows;
}
double Player::getMaroAddEase() {
	if (isMaroAdding) return (Min(EaseOutExpo(maroAddBackAnimTimer * 1.5), 1.0) * ((double)numMarshmallows - prevBackAnimMaro) + prevBackAnimMaro) / (double)maxNumMarshmallows;
	else return 0.0;
}
bool Player::getIsCrisis() {
	return (double)hp <= (double)maxHp * LowHpBorder;
}


void Player::drawForAttack(double opacity) {
	int n = (int)(drawTimer / 0.0625) % 102;

	TextureAsset(U"UiNormalAndBlink")
		(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y)
		.scaled(3).rotated(eR * 2 * (-1) * 360 * attackTime * Math::Pi / 180.0)
		.drawAt(pos, ColorF(1.0, opacity));
}

void Player::drawEffectFront() {
	effectFront.update();
}

void Player::drawEffectBack() {
	effectBack.update();
}

void Player::drawPlayer() {
	//無敵時間中
	if (isInvincibility()) {
		//ヒットバック中
		if (isHitBack) {
			//点滅させる
			int isDrawClear = (int)(drawTimer / (0.125 / 1.5)) % 2;
			if (isDrawClear) {
				//スプライトシートを再生
				int n = (int)(drawTimer / 0.250) % 3;
				TextureAsset(U"UiDamage")(n * TextureAsset(U"UiDamage").size().x / 3.0, 0, TextureAsset(U"UiDamage").size().x / 3.0, TextureAsset(U"UiDamage").size().y).scaled(3).drawAt(pos);
			}
			else {
				//スプライトシートを再生
				int n = (int)(drawTimer / 0.250) % 3;
				TextureAsset(U"UiDamage")(n * TextureAsset(U"UiDamage").size().x / 3.0, 0, TextureAsset(U"UiDamage").size().x / 3.0, TextureAsset(U"UiDamage").size().y).scaled(3).drawAt(pos, ColorF(1.0, 0.8));
			}
		}
		//攻撃中
		else if (isAttack) {
			//点滅させる
			int isDrawClear = (int)(drawTimer / (0.125 / 1.5)) % 2;
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
			int n = (int)(drawTimer / 0.0625) % 102;
			//点滅させる
			int isDrawClear = (int)(drawTimer / (0.125 / 1.5)) % 2;
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
		//Debug 当たり判定
		//RectF(Arg::center(pos.movedBy(20 * 3, 0)), 18 * 3, 22 * 3).draw(ColorF(1,0,0,0.3));
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
		int n = (int)(drawTimer / 0.0625) % 102;
		TextureAsset(U"UiNormalAndBlink")(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y).scaled(3).drawAt(pos);
	}
}

void Player::dyingDraw() {
	drawSpriteAnimAt(U"UiDamage", 3, 0.250, pos);
}

void Player::draw() {

	//Debug
	//RectF(pos.movedBy(60, -TextureAsset(U"UiBeam").size().y / 15.0 - 32), Min(TextureAsset(U"UiBeam").size().x/5 * 3, Scene::Size().x) , TextureAsset(U"UiBeam").size().y/15 * 3).draw();
	//Debug プレイヤーの当たり判定
	//RectF(Arg::center(pos.movedBy(5 * 3, -5 * 3)), 15 * 3, 15 * 3).draw(ColorF(1, 0, 0, 0.3));

	//hpが低いときは赤く点滅
	if (getIsCrisis()) {
		const ScopedColorMul2D colorMul{ ColorF(1.0, 0.8 + lowHpEffectRange * Periodic::Sawtooth0_1(0.9135s), 0.8 + lowHpEffectRange * Periodic::Sawtooth0_1(0.9135s), 1.0) };
		//点滅効果音
		if (Periodic::Sawtooth0_1(0.9135s) < prevValue ) AudioManager::Instance()->playOneShot(U"LowHp");
		prevValue = Periodic::Sawtooth0_1(0.9135s);
		drawPlayer();
	}
	else {
		drawPlayer();
	}

	//Debug
	//RectF(Arg::center(pos.movedBy(5 * 3, -5 * 3 - bodyColAttackOffsetY)), bodyColSize).draw();

	//TextureAsset(U"UiNormalAndBlink")(n * TextureAsset(U"UiNormalAndBlink").size().x / 102, 0, TextureAsset(U"UiNormalAndBlink").size().x / 102, TextureAsset(U"UiNormalAndBlink").size().y).scaled(3).drawAt(pos, ColorF(1.0, 0.0, 0.0, 0.1));

	//Debug
	//int n = (int)(drawTimer / 0.05) % 51;
	//TextureAsset(U"UiBeam")(n * TextureAsset(U"UiBeam").size().x / 51, 0, TextureAsset(U"UiBeam").size().x / 51, TextureAsset(U"UiBeam").size().y).drawAt(pos);

	//Debug
	//RectF(Arg::center(pos.movedBy(5,-5)), 15, 15).draw(ColorF(1, 0, 0));

	//Debug
	// 以前の攻撃当たり判定
	// 
	// 現在の攻撃当たり判定
	//if(isAttackColOn) RectF(Arg::center(pos.movedBy(20 * 3 -30, 0)), 18 * 3 + 30, 22 * 3+15).draw(ColorF(1.0, 0,0, 0.5));
}
