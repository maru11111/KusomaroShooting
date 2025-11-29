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

	void dyingUpdate();

	void defeatBossUpdate();
	void toMountainUpdate();

	void toStartPos();

	void move();

	void attack();

	void damage(int damageAmount, bool piercingInv);

	MaroType chooseMaro();

	void addMarshmallow();

	void stayOnScreen();

	void setKusomaro(MaroType type);

	void setEditorPos();

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

	bool getIsCrisis();

	void heal(int healAmount);
	double getHpEase();
	double getHpHealEase();
	double getMaroEase();
	double getMaroAddEase();
	double getDamageEase();

	void drawForAttack(double opacity);

	void drawEffectFront();
	void drawEffectBack();

	void draw();

	void dyingDraw();

private:
	void drawPlayer();

	Objects& objects;
	int maxHp = 10;
	int hp = maxHp;
	int damageAmount = 10;
	Vec2 pos = { -100, Scene::CenterF().y + TextureAsset(U"UIBack").size().y * 3};
	Vec2 startPos = { 100, Scene::CenterF().y + TextureAsset(U"UIBack").size().y * 3 };
	double toBaseTimer = 0;
	Vec2 vec = { 0,0 };
	double speed = 100*3;
	double remainingInvincibilityTime=0;
	double invincibilityTime=2;
	int numMarshmallows = 0;
	int maxNumMarshmallows = 30;
	double marshmallowAddTimer = 0;
	const double marshmallowAddInterval = 5;
	//ふつうのマシュマロが出る確率(％)
	int normalMaroAppearProbability = 60;
	//回復マロ
	int healMaroAppearProbability = 10;
	//クソマロ判定をされたマロのなかでの確率(％)
	int upMaroAppearProbability =   30;
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
	Effect effectFront;
	Effect effectBack;


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
	//hpが低いとき
	const double LowHpBorder = 0.3;
	const double lowHpEffectRange=0.20;
	double prevValue = 0;

	//maroAnim
	bool isMaroAdding = false;
	int prevMaroNum;
	const double maroAddAmountForAnim = 3.0;
	double maroAddSumForAnim = 0;
	double maroAddBackAnimTimer = 0;
	double prevBackAnimMaro = 0;

	//eat
	double eatTimer=0;
	double isEating = false;
	MaroType ateMaroType;

	//基準位置
	Vec2 basePos = Vec2(100, TextureAsset(U"UIBack").size().y * 6 + (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6)/2.0);
	Vec2 prevPos=pos;

	//移動時エフェクト
	double effectTimer = 0;

	//慣性用タイマー
	double vecTimer = 0;

	//当たり判定
	Vec2 bodyColSize = { 45, 45 };
	double bodyColAttackOffsetY = 0;
	double bodyColAttackOffsetY_ = 0;

	//ボス撃破後の移動
	double toMountainTimer = 0;
	Vec2 toMountainPos = {Scene::Size().x+30, Scene::Size().y+100};

	//近接攻撃
	double eR;
	double eS;

	//プレイヤー描画用タイマー
	double drawTimer = 0;
};
