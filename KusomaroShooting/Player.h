#pragma once
#include "Bullet.h"

struct Objects;

class Player {
public:
	Player(Objects& obj);

	Player(Objects& obj, Vec2 pos);

	~Player();

	void update();

	void bossAppearStateUpdate(double timer);

	void move();

	void attack();

	void damage(int damageAmount);

	MaroType chooseMaro();

	void addMarshmallow();

	void stayOnScreen();

	void setKusomaro(MaroType type);

	RectF bodyCollision()const;
	RectF attackCollision()const;

	void startInvincibilityTime();
	bool isInvincibility();
	bool isMovable();

	int getNumMarshmallows()const;

	int getMaxNumMarshmallows()const;

	int getHp()const;

	int getMaxHp()const;

	MaroType getNextMaro()const;

	int getDamageAmount()const;

	bool getIsAttackColOn()const;

	bool getIsBeamAttacking();
	void setIsBeamAttacking();

	Vec2 getPos();

	bool getIsHitStopStart();

	void heal(int healAmount);
	double getHpEase();
	double getHpHealEase();
	double getMaroEase();
	double getMaroAddEase();
	double getDamageEase();

	void drawForAttack(double opacity);

	void drawEffect();

	void draw();

private:
	Objects& objects;
	int maxHp = 10;
	int hp = maxHp;
	int damageAmount=10;
	Vec2 pos = { 100,100 };
	Vec2 vec = { 0,0 };
	double speed = 100*3;
	double remainingInvincibilityTime=0;
	double invincibilityTime=2;
	int numMarshmallows = 0;
	int maxNumMarshmallows = 30;
	double marshmallowAddTimer = 0;
	const double marshmallowAddInterval = 5;
	//ふつうのマシュマロが出る確率(％)
	int normalMaroAppearProbability = 20;
	//クソマロ判定をされたマロのなかでの確率(％)
	int upMaroAppearProbability = 30;
	int downMaroAppearProbability = 30;
	int sineMaroAppearProbability = 30;
	int beamMaroAppearProbability = 10;
	Array<MaroType>maroBox;
	bool isHitStopStart = false;
	bool isHitBack=false;
	double hitBackTime=1;
	double hitBackTimer=0;
	double firstHitBackSpeed=100*3;
	double hitBackSpeed = firstHitBackSpeed;
	bool isAttack = false;
	bool isAttackColOn = false;
	bool isAttackEffectStarted = false;
	double attackTime = 0.5;
	double attackTimer = 0;
	double attackColTime=0.1;
	bool isBeamAttacking = false;
	double beamTimer=0;
	bool startBlink = false;
	bool keepBlink = false;
	Effect effect;

	//hpHealAnim
	bool isHealing = false;
	int prevHpHeal;
	const double healAmountForAnim=4.0;
	double healSumForAnim = 0;
	double healBackAnimTimer=0;
	double prevBackAnimHp = 0;
	//hpDamageAnim
	bool isDamageHpAnimation = false;
	double damageHpAnimTimer = 0;
	double damageHpAnimEaseTimer = 0;
	int prevHpDamage;

	//maroAnim
	bool isMaroAdding = false;
	int prevMaroNum;
	const double maroAddAmountForAnim = 3.0;
	double maroAddSumForAnim = 0;
	double maroAddBackAnimTimer = 0;
	double prevBackAnimMaro = 0;

	//基準位置
	Vec2 basePos = Vec2(100, TextureAsset(U"UIBack").size().y * 6 + (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6)/2.0);
	Vec2 prevPos=pos;
};
