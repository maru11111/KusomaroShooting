#pragma once
#include "TwoQuads.h"

struct Objects;

class BaseEnemy {
public:

	BaseEnemy(Objects& objects_, Vec2 pos_);

	virtual void update();

	virtual void attack();

	virtual void move()=0;

	//死んだらtrue, 生きていたらfalseを返す
	virtual bool damage(int damageAmount, bool isExistInv);

	virtual bool isDestroy();

	bool isInvincibility();

	virtual TwoQuads collision()const=0;

	int getDamageAmount();

	Vec2 getPos();

	int getId();

	virtual void draw()=0;

	int score;

	String name;

protected:
	Objects& objects;
	int id;
	static int prevId;
	int maxHp;
	int hp;
	Vec2 pos;
	Vec2 vec;
	double maxSpeed;
	double speed;
	int damageAmount;
	double timer = 0;
	double animTimer = 0;
	double remainingInvincibilityTime = 0;
	double invincibilityTime = 0.5;
	//描画用タイマー
	double drawTimer = 0;
};

class GarbageBagNormal : public BaseEnemy {
public:

	GarbageBagNormal(Objects& objects_, Vec2 pos_);

	void move()override;

	TwoQuads collision()const override;

	void draw()override;
private:
	double firstAngle = Random(0,360);
	double exitTimer = 0;
};

class GarbageBagAccel : public BaseEnemy {
public:

	GarbageBagAccel(Objects& objects_, Vec2 pos_);

	void move()override;

	TwoQuads collision()const override;

	void draw()override;
private:
	double speedTimer = 0;
	double firstAngle = Random(0, 360);
};


class GarbageBagSine : public BaseEnemy {
public:

	GarbageBagSine(Objects& objects_, Vec2 pos_);

	void move()override;

	TwoQuads collision()const override;

	void draw()override;
private:
	double sineTimer = 0;
	double firstAngle = Random(0, 360);
	double exitTimer = 0;
};

class GarbageBagFast : public BaseEnemy {
public:
	GarbageBagFast(Objects& objects_, Vec2 pos_);
	void move()override;

	TwoQuads collision()const override;

	void draw()override;
private:
	double firstAngle = Random(0, 360);
	double exitTimer = 0;
};

class GarbageBagWithCan : public BaseEnemy {
public:
	GarbageBagWithCan(Objects& objects_, Vec2 pos_);
	void move()override;
	void attack()override;
	TwoQuads collision()const override;
	void draw()override;
private:
	double attackInterval=3;
	double firstAngle = Random(0, 360);
	double exitTimer = 0;
};

class GarbageBagWithCanStay : public BaseEnemy {
public:
	GarbageBagWithCanStay(Objects& objects_, Vec2 pos_);
	void move()override;
	void attack()override;
	TwoQuads collision()const override;
	void draw()override;
private:
	double attackInterval = 3;
	double stayTimer = 0;
	double stayTime = 6;
	Vec2 stayPos;
	Vec2 initPos;
	bool isAppear = false;
	double appearTimer = 0;
	double currentAngle=0;
	double speedEaseTimer = 0;
};

class Can : public BaseEnemy {
public:
	enum class CanType {
		Red,
		Grape,
		Orange,
		White,
		Size
	};
	CanType canType;
	Texture texture;
	Can(Objects& objects_, Vec2 pos_, Vec2 vec_);
	void move()override;
	TwoQuads collision()const;
	void draw()override;
private:
};

class CanForBag : public BaseEnemy {
public:
	enum class CanType {
		Red,
		Grape,
		Orange,
		White,
		Size
	};
	CanType canType;
	Texture texture;
	CanForBag(Objects& objects_, Vec2 bagPos_, Vec2 vec_, Vec2* bagPos);
	void move()override;
	TwoQuads collision()const;
	void draw()override;
private:
	double stayTimer = 0;
	Vec2* bagPos;
	double posOffsetY=20;
	bool playThrowSE = false;
};

class Fish : public BaseEnemy {
public:
	Fish(Objects& objects_, Vec2 pos_);
	void move()override;
	TwoQuads collision()const;
	void draw()override;
private:
	double moveInterval=3;
	double isMoving=false;
};

class Umbrella : public BaseEnemy {
public:
	Umbrella(Objects& objects_, Vec2 pos_);
	void move()override;
	TwoQuads collision()const;
	void draw()override;
private:
	double gravity=30;
	double maxVelX = 120;
	double currentVelX=0;
	double angle=0;
};

class HealUmbrella : public BaseEnemy {
public:
	HealUmbrella(Objects& objects_, Vec2 pos_);
	void move()override;
	TwoQuads collision()const;
	bool damage(int damageAmount, bool isExistInv)override;
	void draw()override;
private:
	double gravity = 30;
	double maxVelX = 120;
	double currentVelX = 0;
	double angle = 0;
};

class BaseBoss : public BaseEnemy {
public:
	BaseBoss(Objects& objects_, Vec2 pos_);

	void update()override;

	virtual void dyingUpdate()=0;

	void move()override=0;
	TwoQuads collision()const =0;
	void draw()override=0;

	int getMaxHp()const;

	int getCurrentHp()const;

	String getName()const;

	double getDamageEase();

	double getPrevHpDamage();

	bool damage(int damageAmount, bool isExistInv)override;

protected:
	String name;
	//hpDamageAnim
	bool isDamageHpAnimation = false;
	double damageHpAnimTimer = 0;
	double damageHpAnimEaseTimer = 0;
	int prevHpDamage=0;
};

class GarbageBox : public BaseBoss {
public:
	enum class State {
		Appear,
		Idle,
		ThrowCan,
		RollingAttack,
		RollingAttackLong,
		DashAttack,
		StateNum
	};
	struct TimerVar {
		double timer;
		const double time;
		TimerVar(double time_)
			:time(time_)
		{
			timer = 0;
		}
	};
	GarbageBox(Objects& objects_, Vec2 pos_);
	void move()override;
	void dyingUpdate()override;
	TwoQuads collision()const;
	void updateAttackStateTimer();
	bool isDestroy()override;
	bool isDefeated();
	void draw()override;

private:
	State state = State::Appear;
	State nextState = State::ThrowCan;
	double currentAngle = 0;
	Vec2 basePos = Vec2(Scene::Size().x - 150, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6);
	Vec2 preReadyToAttackPos;
	bool isEndDefeatAnim=false;

	TimerVar timers[(int)State::StateNum] =
	{
		//登場
		TimerVar(100.0),
		//待機
		TimerVar(5.0),
		//缶投げ
		TimerVar(100.0),
		//ローリング体当たり
		TimerVar(100.0),
		//ローリング体当たりロング
		TimerVar(100.0),
		//ダッシュアタック
		TimerVar(100.0)
	};
	//登場
	enum class AppearState {
		ToAppearBasePos,
		PullBody,
		PushBody,
		GataGata,
		ToBasePos
	};
	AppearState appearState = AppearState::ToAppearBasePos;
	Vec2 appearBasePos = Vec2(Scene::Size().x - 350, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6);
	Vec2 initPos;
	double appearPullAngle = 45 * Math::Pi / 180.0;
	double appearPushAngle = - 410 * Math::Pi / 180.0;
	bool isPlayPullAudio = false;
	bool isPlayPushAudio = false;

	//缶投げ
	bool isLidOpen=false;
	bool isEndAttack=false;
	int throwCanNum=0;
	int throwCanMaxNum=25;
	bool isLidClosing = false;
	double lidOpenedTime=0;
	bool isPlayOpenSE=false;
	bool isPlayCloseSE=false;

	//ローリングアタック
	const double firstAngle = 45 * Math::Pi / 180.0;
	double ease=0;
	enum class RollingState {
		PreAction,
		Rolling,
	};
	RollingState rollingState=RollingState::PreAction;
	double easeTimer=0;
	double rollingAngle=360 * Math::Pi / 180.0;

	TimerVar throwCanIntervalTimer = TimerVar(0.3);

	//ダッシュアタック
	enum class DashState {
		PreAction,
		Dash1,
		Dash2,
		EndAction
	};
	enum class DashPattern {
		Top,
		Bottom
	};
	DashState dashState = DashState::PreAction;
	DashPattern dashPattern = DashPattern::Bottom;
	bool isSelectPattern=false;
	Vec2 dashPosTopRightStart = Vec2(Scene::Size().x - 100, basePos.y-120);
	Vec2 dashPosTopRightEnd = Vec2(Scene::Size().x + 150, basePos.y-120);
	Vec2 dashPosTopLeft = Vec2(-150, dashPosTopRightStart.y);
	Vec2 dashPosBottomRightStart = Vec2(Scene::Size().x - 100, Scene::Size().y - 120);
	Vec2 dashPosBottomRightEnd = Vec2(Scene::Size().x + 150, Scene::Size().y - 120);
	Vec2 dashPosBottomLeft = Vec2(-150, dashPosBottomRightStart.y);
};
