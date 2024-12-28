#pragma once
#include "Bullet.h"

struct Objects;

class Player {
public:
	Player(Objects& obj);

	~Player();

	void update();

	void move();

	void attack();

	void damage(int damageAmount);

	MaroType chooseMaro();

	void addMarshmallow();

	void setKusomaro(MaroType type);

	RectF collision()const;

	void startInvincibilityTime();
	bool isInvincibility();
	bool isMovable();

	int getNumMarshmallows()const;

	int getMaxNumMarshmallows()const;

	int getHp()const;

	int getMaxHp()const;

	MaroType getNextMaro()const;

	void draw();

private:
	Objects& objects;
	int maxHp = 10;
	int hp = maxHp;
	Vec2 pos = { 100,100 };
	Vec2 vec = { 0,0 };
	double speed = 100;
	double remainingInvincibilityTime=0;
	double invincibilityTime=2;
	int numMarshmallows = 0;
	int maxNumMarshmallows = 30;
	double marshmallowAddTimer = 0;
	const double marshmallowAddInterval = 3;
	//ふつうのマシュマロが出る確率(％)
	int normalMaroAppearProbability = 90;
	//クソマロ判定をされたマロのなかでの確率(％)
	int upMaroAppearProbability = 30;
	int downMaroAppearProbability = 30;
	int sineMaroAppearProbability = 30;
	int beemMaroAppearProbability = 10;
	Array<MaroType>maroBox;
	bool isHitBack=false;
	double hitBackTime=1;
	double hitBackTimer=0;
	double firstHitBackSpeed=100;
	double hitBackSpeed = firstHitBackSpeed;
};
