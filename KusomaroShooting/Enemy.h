#pragma once
#include "TwoQuads.h"

struct Objects;

class BaseEnemy {
public:

	BaseEnemy(Objects& objects_, Vec2 pos_);

	void update();

	virtual void attack();

	virtual void move()=0;

	//ダメージを受ける関数。無敵時間ならfalse, そうでなければtrueを返す
	bool damage(int damageAmount, bool isExistInv);

	virtual bool isDestroy();

	bool isInvincibility();

	virtual TwoQuads collision()const=0;

	int getDamageAmount();

	Vec2 getPos();

	int getId();

	virtual void draw()=0;

protected:
	Objects& objects;
	int id;
	static int prevId;
	int maxHp;
	int hp;
	Vec2 pos;
	Vec2 vec;
	double speed;
	int damageAmount;
	double timer = 0;
	double remainingInvincibilityTime = 0;
	double invincibilityTime = 0.5;
};

class GarbageBagNormal : public BaseEnemy {
public:

	GarbageBagNormal(Objects& objects_, Vec2 pos_);

	void move()override;

	TwoQuads collision()const override;

	void draw()override;
};

class GarbageBagFast : public BaseEnemy {
public:
	GarbageBagFast(Objects& objects_, Vec2 pos_);
	void move()override;

	TwoQuads collision()const override;

	void draw()override;
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
	double maxVelX = 200;
	double currentVelX=0;
	double angle=0;
};

class GarbageBox : public BaseEnemy {
public:
	enum class State {
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
	TwoQuads collision()const;
	void updateAttackStateTimer();
	bool isDestroy()override;
	void draw()override;

private:
	State state = State::Idle;
	State nextState = State::RollingAttack;
	double currentAngle = 0;
	Vec2 basePos = Vec2(Scene::Size().x - 150, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6);

	TimerVar timers[(int)State::StateNum] =
	{
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
	//缶投げ
	bool isLidOpen=false;
	bool isEndAttack=false;
	int throwCanNum=0;
	int throwCanMaxNum=25;
	bool isLidClosing = false;
	double lidOpenedTime=0;

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
