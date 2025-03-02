#include "stdafx.h"
#include "Enemy.h"
#include "Objects.h"
#include "Common.h"

BaseEnemy::BaseEnemy(Objects& objects_, Vec2 pos_)
	: objects{ objects_ }
	, pos{ pos_ }
{
	// 固有のidを付与
	id = prevId;
	prevId++;
}

void BaseEnemy::attack() {

}

void BaseEnemy::update() {
	animTimer += Scene::DeltaTime();
	move();
	attack();
	//無敵時間があったら更新
	if (isInvincibility()) {
		remainingInvincibilityTime -= Scene::DeltaTime();
		if (remainingInvincibilityTime < 0)remainingInvincibilityTime = 0;
	}
}

bool BaseEnemy::damage(int damageAmount, bool isExistInv) {
	//無敵時間を考慮
	if (isExistInv) {
		//無敵時間中
		if (isInvincibility()) {
			//ダメージを受けない
			return false;
		}
		else {
			//ダメージを受ける
			remainingInvincibilityTime = invincibilityTime;
			hp -= damageAmount;
			if (hp < 0)hp = 0;
			//ダメージSE
			if (hp == 0) AudioManager::Instance()->play(U"HitHigh");
			else AudioManager::Instance()->play(U"HitLow");
			return true;
		}
	}
	//無敵時間を考慮しない
	else {
		//ダメージを受ける
		hp -= damageAmount;
		if (hp < 0)hp = 0;
		//ダメージSE
		if (hp == 0) AudioManager::Instance()->play(U"HitHigh");
		else AudioManager::Instance()->play(U"HitLow");
		return true;
	}
}

bool BaseEnemy::isDestroy() {
	if (pos.x < 0 - 50  || Scene::Size().x + Scene::Size().x < pos.x  || pos.y < 0 - Scene::Size().y || Scene::Size().y + 50 < pos.y)return true;
	if (hp <= 0) return true;
	else return false;
}

bool BaseEnemy::isInvincibility() {
	if (0 < remainingInvincibilityTime) return true;
	else return false;
}

int BaseEnemy::getDamageAmount() {
	return damageAmount;
}
Vec2 BaseEnemy::getPos() {
	return pos;
}
int BaseEnemy::getId() {
	return id;
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
	
	TextureAsset(U"GarbageBag").scaled(3).rotatedAt({TextureAsset(U"GarbageBag").size().x * 3 / 2.0, TextureAsset(U"GarbageBag").size().y * 3 / 2.0 + 5}, firstAngle + animTimer * 1 * (-360_deg)).drawAt(pos);

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

	//Print << objects.enemies.size();

	if (attackInterval <= timer) {
		timer -= attackInterval;
		//缶を飛ばす
		objects.enemies << std::make_unique<Can>(objects, pos, (objects.player->getPos() - pos).normalized());
		//缶を飛ばす音
		//int type = Random(0, 2);
		//switch (type) {
		//case 0:AudioManager::Instance()->play(U"Can1"); break;
		//case 1:AudioManager::Instance()->play(U"Can2"); break;
		//case 2:AudioManager::Instance()->play(U"Can3"); break;
		//}
		AudioManager::Instance()->play(U"Can2");
	}
}

TwoQuads GarbageBagWithCan::collision()const {
	return (Quad)RectF(Arg::center(pos), 7 * 3, 7 * 3);
}

void GarbageBagWithCan::draw() {
	TextureAsset(U"GarbageBagWithCan")
		.scaled(3)
		.rotatedAt({ TextureAsset(U"GarbageBagWithCan").size().x * 3 / 2.0, TextureAsset(U"GarbageBagWithCan").size().y * 3 / 2.0 + 5 }, Scene::Time() * 0.5 * (-360_deg))
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

	//缶の種類を抽選
	canType = (CanType)Random(0, (int)CanType::Size-1);

	//テクスチャ決定
	switch (canType) {
	case CanType::Red: texture = TextureAsset(U"RedCan"); break;
	case CanType::Grape: texture = TextureAsset(U"GrapeCan"); break;
	case CanType::Orange: texture = TextureAsset(U"OrangeCan"); break;
	case CanType::White: texture = TextureAsset(U"WhiteCan"); break;
	}
}

void Can::move() {
	pos += vec * speed * Scene::DeltaTime();
}

TwoQuads Can::collision()const {
	return (Quad)RectF(Arg::center(pos), 15, 15);
}

void Can::draw() {
	texture.scaled(4).rotated(vec.getAngle()).drawAt(pos);
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
		AudioManager::Instance()->play(U"KaraKara");
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

	//Print << abs(currentVelX) / maxVelX;

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


BaseBoss::BaseBoss(Objects& objects_, Vec2 pos)
	: BaseEnemy(objects_, pos)
{
	id = -1;
}
void BaseBoss::update() {
	//Print << getDamageEase();

	move();
	attack();
	//無敵時間があったら更新
	if (isInvincibility()) {
		remainingInvincibilityTime -= Scene::DeltaTime();
		if (remainingInvincibilityTime < 0)remainingInvincibilityTime = 0;
	}
	
	//ダメージ時のhp減少アニメーション
	if (isDamageHpAnimation) {
		damageHpAnimTimer += Scene::DeltaTime();

		if (2.0 <= damageHpAnimTimer) {
			damageHpAnimEaseTimer += Scene::DeltaTime();

			if (1.0 <= damageHpAnimEaseTimer) {
				isDamageHpAnimation = false;
			}
		}
	}
}
int BaseBoss::getMaxHp()const {
	return maxHp;
}
int BaseBoss::getCurrentHp()const {
	return hp;
}
String BaseBoss::getName()const {
	return name;
}
double BaseBoss::getDamageEase() {
	if (isDamageHpAnimation) return ((1.0 - Min(EaseInQuart(damageHpAnimEaseTimer), 1.0)) * ((double)prevHpDamage - (double)hp)) / ((double)prevHpDamage - (double)hp);
	else return 0.0;
}
double BaseBoss::getPrevHpDamage() {
	return prevHpDamage;
}



GarbageBox::GarbageBox(Objects& objects_, Vec2 pos_)
	: BaseBoss(objects_, pos_)
{
	maxHp = 800;
	hp = maxHp;
	damageAmount = 1;
	name = U"巨大ゴミ箱";
	initPos = pos_;
}

void GarbageBox::move() {
	//Print << U"State:" << (int)state;
	//Print << U"NextState:" << (int)nextState;
	//Print << U"id:" << id;

	switch (state) {
		//登場
	case State::Appear:
		//タイマー
		//Print << U"timer:" << timers[(int)state].timer;
		//Print << U"appearState:" << (int)appearState;
		timers[(int)state].timer += Scene::DeltaTime();

		switch (appearState) {
		case AppearState::ToAppearBasePos:
			if (ease == 0) AudioManager::Instance()->play(U"AppearBoss");
			//登場基準位置に移動
			ease = EaseOutCirc(timers[(int)state].timer / 2.0);
			pos = initPos.lerp(appearBasePos, ease);

			//次の動きに移る
			if (2.0 <= timers[(int)state].timer) {
				appearState = AppearState::PullBody;
				//タイマーリセット
				timers[(int)state].timer = 0;
			}
			break;

		case AppearState::PullBody:
			//体を引く
			ease = Min(EaseInLinear(timers[(int)state].timer/1.0), 1.0);
			currentAngle = ease * appearPullAngle;
			//ガタガタする
			pos.x += ease * 1.0 * Periodic::Square1_1(0.1s);
			//ガタガタ効果音
			if (not isPlayPullAudio) {
				AudioManager::Instance()->play(U"BossAppearPull");
				isPlayPullAudio = true;
			}

			//次の動きに移る
			if (1.5 <= timers[(int)state].timer) {
				appearState = AppearState::PushBody;
				//タイマーリセット
				timers[(int)state].timer = 0;
				//イージングリセット
				ease = 0;
			}
			break;

		case AppearState::PushBody:
			//体を乗り出す
			ease = EaseOutExpo(timers[(int)state].timer * 2);
			currentAngle = Math::Lerp(appearPullAngle, appearPushAngle, ease);
			//ガタガタする
			//pos.x += ease * 2.5 * Periodic::Square1_1(0.1s);
			//体を乗り出すときの効果音
			if (not isPlayPushAudio) {
				AudioManager::Instance()->play(U"RollingAttack");
				isPlayPushAudio = true;
			}

			//次の動きに移る
			if (0.5 <= timers[(int)state].timer) {
				appearState = AppearState::GataGata;
				//タイマーリセット
				timers[(int)state].timer = 0;
				//イージングリセット
				ease = 0;
			}
			break;

		case AppearState::GataGata:
			ease = EaseOutExpo(timers[(int)state].timer * 2);
			//ガタガタする
			pos.x += ease * 2.5 * Periodic::Square1_1(0.1s);
			//体を乗り出す角度を維持
			currentAngle = appearPushAngle;

			//次の動きに移る
			if (0.5 <= timers[(int)state].timer) {
				appearState = AppearState::ToBasePos;
				//タイマーリセット
				timers[(int)state].timer = 0;
				//イージングリセット
				ease = 0;
			}
			break;

		case AppearState::ToBasePos:
			ease = EaseOutCirc(timers[(int)state].timer / 0.5);
			//基準位置に移動
			pos = appearBasePos.lerp(basePos, ease);
			//角度を戻す
			currentAngle = Math::Lerp(appearPushAngle+360*Math::Pi/180.0, 0, ease);

			//待機に移る
			if (0.5 <= timers[(int)state].timer) {
				state = State::Idle;
				//タイマーリセット
				timers[(int)state].timer = 0;
				//イージングリセット
				ease = 0;
			}
			break;
		}
		break;

		//待機
	case State::Idle:
		//タイマー
		timers[(int)state].timer += Scene::DeltaTime();

		//攻撃準備中
		if (timers[(int)state].time-1.0 <= timers[(int)state].timer) {
			easeTimer += Scene::DeltaTime();
			ease = EaseOutSine(easeTimer);
			pos = preReadyToAttackPos.lerp(basePos, ease);
		}
		//攻撃準備前
		else {
			//Print << U"ふわふわ";
			//ふわふわする
			pos.y += 0.5 * Math::Cos(Scene::Time() * Math::Pi / 4.0);
			preReadyToAttackPos = pos;
		}

		if (timers[(int)state].time <= timers[(int)state].timer) {
			//次の攻撃に移る
			timers[(int)state].timer = 0;
			easeTimer = 0;
			state = nextState;
		}
		break;

		//缶を飛ばす
	case State::ThrowCan:
		//効果音
		if(timers[(int)state].timer==0) AudioManager::Instance()->play(U"ReadyToThrowCan");

		//タイマー更新
		timers[(int)state].timer += Scene::DeltaTime();

		//ふたが開くときの効果音
		if (currentFrame(56, 0.08, timers[(int)state].timer) == 47 && not isPlayOpenSE) {
			AudioManager::Instance()->play(U"Open");
			isPlayOpenSE = true;
		}

		//ふたが開ききったか判定
		if (currentFrame(56, 0.08, timers[(int)state].timer) == 51 && not isLidOpen && not isLidClosing) {
			lidOpenedTime = timers[(int)state].timer;
			isLidOpen = true;
		}

		//もし蓋が開いていたら
		if (isLidOpen) {
			throwCanIntervalTimer.timer += Scene::DeltaTime();
			if (throwCanIntervalTimer.time <= throwCanIntervalTimer.timer) {
				//缶を飛ばす
				objects.enemies << std::make_unique<Can>(objects, pos.movedBy(0, -100), (objects.player->getPos() - pos.movedBy(0,-100)).normalized());
				//缶を飛ばす音
				//int type = Random(0, 2);
				//switch (type) {
				//case 0:AudioManager::Instance()->play(U"Can1"); break;
				//case 1:AudioManager::Instance()->play(U"Can2"); break;
				//case 2:AudioManager::Instance()->play(U"Can3"); break;
				//}
				AudioManager::Instance()->play(U"Can2");
				//リセット
				throwCanIntervalTimer.timer -= throwCanIntervalTimer.time;
				//飛ばした缶の数をカウント
				throwCanNum++;
			}
		}
		//指定の数だけ缶を出し切ったら
		if (throwCanNum == throwCanMaxNum) {
			//蓋を閉じ始める
			isLidOpen = false;
			isLidClosing = true;
			throwCanNum = 0;
			timers[(int)state].timer = lidOpenedTime;
		}
		//ふたを閉じるときの効果音
		if (currentFrame(56, 0.08, timers[(int)state].timer) == 55 && not isPlayCloseSE && isLidClosing) {
			AudioManager::Instance()->play(U"Close");
			isPlayCloseSE = true;
		}

		//蓋を閉じている最中なら
		if (isLidClosing) {
			//閉じ終えたらidleに戻る
			if (currentFrame(56, 0.08, timers[(int)state].timer) == 56) {
				//時間リセット
				timers[(int)state].timer = 0;
				//蓋を閉じ終える
				isLidClosing = false;
				//フラグをリセット
				isPlayOpenSE = false;
				isPlayCloseSE = false;
				//攻撃を終了する
				isEndAttack = true;
			}
		}
		break;

	case State::RollingAttack:
		updateAttackStateTimer();
		//Print << timers[(int)state].timer;

		switch (rollingState) {
		case RollingState::PreAction:
			//効果音
			if (easeTimer == 0) AudioManager::Instance()->play(U"ReadyToDashAttack");
			if (timers[(int)state].timer <= 2.25) {
				easeTimer += Scene::DeltaTime();
				ease = EaseOutCirc(Min(easeTimer / 2.25, 1.0));
				currentAngle = ease * firstAngle;
				pos.x += ease * 1.1 *Periodic::Square1_1(0.1s);
			}
			else {
				rollingState = RollingState::Rolling;
				timers[(int)state].timer = 0;
				easeTimer = 0;
			}
			break;
		case RollingState::Rolling:
			if(easeTimer==0) AudioManager::Instance()->play(U"RollingAttack");
			if (timers[(int)state].timer <= 1.0) {
				easeTimer += Scene::DeltaTime();
				ease = EaseOutCubic(easeTimer / 1.0);
				currentAngle = -ease * rollingAngle;
				pos.x -= 20 * Math::Cos(timers[(int)state].timer*Math::Pi);
				pos.y -= 16 * Math::Cos(timers[(int)state].timer * Math::Pi*2.0);
			}
			else {
				//PreActionに戻す
				rollingState = RollingState::PreAction;
				//タイマーリセット
				timers[(int)state].timer = 0;
				easeTimer = 0;
				//25％の確率でローリングロングに移行
				if (Random(0, 3) == 0) {
					state = State::RollingAttackLong;
				}
				else {
					//攻撃終了
					isEndAttack = true;
				}
			}
			break;
		default:
			throw Error(U"不明な状態");
			break;
		}
		break;

	case State::RollingAttackLong:
		updateAttackStateTimer();
		
		switch (rollingState) {
		case RollingState::PreAction:
			if (easeTimer == 0) AudioManager::Instance()->play(U"ReadyToRollingAttackLong");
			if (timers[(int)state].timer <= 3.25) {
				easeTimer += Scene::DeltaTime();
				ease = EaseOutCirc(Min(easeTimer / 2.25, 1.0));
				currentAngle = ease * firstAngle;
				pos.x += ease * 3.0 * Periodic::Square1_1(0.1s);
			}
			else {
				rollingState = RollingState::Rolling;
				timers[(int)state].timer = 0;
				easeTimer = 0;
			}
			break;
		case RollingState::Rolling:
			if (easeTimer == 0) AudioManager::Instance()->play(U"RollingAttackLong");
			if (timers[(int)state].timer <= 1.0) {
				easeTimer += Scene::DeltaTime();
				ease = EaseOutCubic(easeTimer / 1.0);
				currentAngle = -ease * rollingAngle;
				pos.x -= 30 * Math::Cos(timers[(int)state].timer * Math::Pi/1.0);
				pos.y -= 13 * Math::Cos(timers[(int)state].timer * Math::Pi * 2.0/1.0);
			}
			else {
				//PreActionに戻す
				rollingState = RollingState::PreAction;
				//タイマーリセット
				timers[(int)state].timer = 0;
				easeTimer = 0;
				//攻撃終了
				isEndAttack = true;
			}
			break;
		default:
			throw Error(U"不明な状態");
			break;
		}
		break;

	case State::DashAttack:
		updateAttackStateTimer();

		//ダッシュパターンを決定
		if (not isSelectPattern) {
			dashPattern = (DashPattern)Random(0, 1);
			isSelectPattern = true;
		}

		switch (dashState) {
		case DashState::PreAction:
			if (timers[(int)state].timer <= 2.25) {
				if (easeTimer == 0) AudioManager::Instance()->play(U"ReadyToDashAttack");
				easeTimer += Scene::DeltaTime();
				ease = EaseOutCirc(Min(easeTimer / 2.25, 1.0));
				currentAngle = ease * firstAngle;
				if(dashPattern==DashPattern::Top) pos = basePos.lerp(dashPosTopRightStart, ease);
				else pos = basePos.lerp(dashPosBottomRightStart, ease);
				pos.x += ease * 1.3 * Periodic::Square1_1(0.1s);
			}
			else {
				dashState = DashState::Dash1;
				timers[(int)state].timer = 0;
				easeTimer = 0;
			}
			break;

		case DashState::Dash1:
			if (timers[(int)state].timer <= 2.75) {
				if(easeTimer==0) AudioManager::Instance()->play(U"DashAttack");
				easeTimer += Scene::DeltaTime();
				ease = EaseInQuad(Min(easeTimer / 1.75, 1.0));
				currentAngle -= 20 * Scene::DeltaTime();
				if (dashPattern == DashPattern::Top) pos = dashPosTopRightStart.lerp(dashPosTopLeft, ease);
				else pos = dashPosBottomRightStart.lerp(dashPosBottomLeft, ease);
			}
			else {
				dashState = DashState::Dash2;
				timers[(int)state].timer = 0;
				easeTimer = 0;
			}
			break;

		case DashState::Dash2:
			if (timers[(int)state].timer <= 1.75) {
				easeTimer += Scene::DeltaTime();
				ease = EaseInQuad(Min(easeTimer / 1.75, 1.0));
				currentAngle += 20 * Scene::DeltaTime();
				if (dashPattern == DashPattern::Top) pos = dashPosBottomLeft.lerp(dashPosBottomRightEnd, ease);
				else pos = dashPosTopLeft.lerp(dashPosTopRightEnd, ease);
			}
			else {
				dashState = DashState::EndAction;
				timers[(int)state].timer = 0;
				easeTimer = 0;
				isSelectPattern = false;
				currentAngle = 0;
			}
			break;
			
		case DashState::EndAction:
			if (timers[(int)state].timer <= 1.00) {
				easeTimer += Scene::DeltaTime();
				ease = EaseOutQuad(Min(easeTimer, 1.0));
				if (dashPattern==DashPattern::Top) pos = dashPosBottomRightEnd.lerp(basePos, ease);
				else pos = dashPosTopRightEnd.lerp(basePos, ease);
			}
			else {
				dashState = DashState::PreAction;
				timers[(int)state].timer = 0;
				easeTimer = 0;
				isEndAttack = true;
			}
			break;
		}
		break;
	default:
		throw Error(U"不明な行動");
		break;
	}

	//次の状態を選ぶ
	//攻撃が終了していれば
	if (isEndAttack) {
		//一度待機状態に戻る
		state = State::Idle;
		//フラグを元に戻す
		isEndAttack = false;
		isLidOpen = false;
		//ランダムな行動を選ぶ
		nextState = (State)(Random((int)State::ThrowCan, (int)State::DashAttack));
		//角度をリセット
		currentAngle = 0;
	}
}

TwoQuads GarbageBox::collision()const {
	return TwoQuads((Quad)RectF(Arg::center(pos.movedBy(0, -5)), 140, 190).rotated(currentAngle) );
}

void GarbageBox::updateAttackStateTimer() {
	timers[(int)state].timer += Scene::DeltaTime();
	if (timers[(int)state].time <= timers[(int)state].timer) {
		//待機モーションに戻る
		state = State::Idle;
		timers[(int)state].timer = 0;
		//攻撃を終了する
		isEndAttack = true;
	}
}

bool BaseBoss::damage(int damageAmount, bool isExistInv) {
	//無敵時間を考慮
	if (isExistInv) {
		//無敵時間中
		if (isInvincibility()) {
			//ダメージを受けない
			return false;
		}
		else {
			//ダメージを受ける前のhpを保存
			if (not isDamageHpAnimation)prevHpDamage = hp;
			else prevHpDamage = (1.0 - Min(EaseInQuart(damageHpAnimEaseTimer), 1.0)) * ((double)prevHpDamage - (double)hp)+(double)hp;
			//ダメージを受ける
			remainingInvincibilityTime = invincibilityTime;
			hp -= damageAmount;
			if (hp < 0)hp = 0;
			//ダメージSE
			if (hp == 0) AudioManager::Instance()->play(U"HitHigh");
			else AudioManager::Instance()->play(U"HitLow");
			//ダメージ減少アニメーション
			//ダメージ時のhp減少アニメーションのためのタイマー
			damageHpAnimTimer = 0;
			damageHpAnimEaseTimer = 0;
			// 〃 のためのフラグ
			isDamageHpAnimation = true;

			return true;
		}

	}
	//無敵時間を考慮しない
	else {
		//ダメージを受ける前のhpを保存
		if (not isDamageHpAnimation) prevHpDamage = hp;
		else prevHpDamage = (1.0 - Min(EaseInQuart(damageHpAnimEaseTimer), 1.0)) * ((double)prevHpDamage-(double)hp)+(double)hp;
		//ダメージを受ける
		hp -= damageAmount;
		if (hp < 0)hp = 0;
		//ダメージSE
		if (hp == 0) AudioManager::Instance()->play(U"HitHigh");
		else AudioManager::Instance()->play(U"HitLow");
		//ダメージ減少アニメーション
		//ダメージ時のhp減少アニメーションのためのタイマー
		damageHpAnimTimer = 0;
		damageHpAnimEaseTimer = 0;
		// 〃 のためのフラグ
		isDamageHpAnimation = true;
		return true;
	}
}

bool GarbageBox::isDestroy() {
	if (hp <= 0) return true;
	else return false;
}

void GarbageBox::draw() {

	switch (state) {
	case State::Appear:
		drawRotateSpriteAnim(U"GarbageBox", 3, 0.14, pos, currentAngle);
		break;

	case State::Idle:
		drawSpriteAnim(U"GarbageBox", 3, 0.14, pos);
		break;

	case State::ThrowCan:
		if (not isLidOpen) drawSpriteAnim(U"GarbageBoxOpen", 56, 0.08, pos, timers[(int)state].timer);
		else drawSprite(U"GarbageBoxOpen", 56, 51, pos);
		break;

	case State::RollingAttack:
		switch (rollingState) {
		case RollingState::PreAction:
			drawRotateSpriteAnim(U"GarbageBox", 3, 0.14, pos, currentAngle);
			break;
		case RollingState::Rolling:
			drawRotateSpriteAnim(U"GarbageBox", 3, 0.14, pos, currentAngle);
			break;
		}
		break;

	case State::RollingAttackLong:
		switch (rollingState) {
		case RollingState::PreAction:
			drawRotateSpriteAnim(U"GarbageBox", 3, 0.14, pos, currentAngle);
			break;
		case RollingState::Rolling:
			drawRotateSpriteAnim(U"GarbageBox", 3, 0.14, pos, currentAngle);
			break;
		}
		break;

	case State::DashAttack:
		drawRotateSpriteAnim(U"GarbageBox", 3, 0.14, pos, currentAngle);
		break;
	}

	//当たり判定
	//RectF(Arg::center(pos.movedBy(0,-5)), 140, 190).draw(ColorF(1, 0, 0, 0.25));
	//n = (int)(Scene::Time() / 0.07) % 52;
	//TextureAsset(U"GarbageBoxOpen")(n * TextureAsset(U"GarbageBoxOpen").size().x / 52.0, 0, TextureAsset(U"GarbageBoxOpen").size().x / 52.0, TextureAsset(U"GarbageBoxOpen").size().y).scaled(3).draw(600, 300 + 15 * Math::Sin(Scene::Time()));
	//あた
	//RectF(Arg::center(pos.movedBy(0, -5)), 140, 190).rotated(currentAngle).draw(ColorF(1, 0, 0, 0.3));
}

int BaseEnemy::prevId = 0;
