#pragma once
#include "Bullet.h"

struct Objects;

class Player {
public:
	Player(Objects& obj);

	void update();

	void move();

	void attack();

	void damage(int damageAmount);

	MaroType chooseMaro();

	void addMarshmallow();

	RectF collision()const;

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
	double damping = 0;
	int numMarshmallows = 0;
	int maxNumMarshmallows = 30;
	double marshmallowAddTimer = 0;
	double marshmallowAddInterval = 3;
	//ふつうのマシュマロが出る確率(％)
	int normalMaroAppearProbability = 0;
	//クソマロ判定をされたマロのなかでの確率(％)
	int upMaroAppearProbability = 30;
	int downMaroAppearProbability = 30;
	int sineMaroAppearProbability = 30;
	int beemMaroAppearProbability = 10;
	Array<MaroType>maroBox;
};
