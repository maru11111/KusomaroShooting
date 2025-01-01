#pragma once

struct Objects;

class BaseEnemy {
public:

	BaseEnemy(Objects& objects_, Vec2 pos_);

	void update();

	virtual void attack();

	virtual void move()=0;

	void damage(int damageAmount);

	bool isDestroy();

	virtual RectF collision()const=0;

	int getDamageAmount();

	Vec2 getPos();

	virtual void draw()=0;
	
protected:
	Objects& objects;
	int maxHp;
	int hp;
	Vec2 pos;
	Vec2 vec;
	double speed;
	int damageAmount;
};

class GarbageBagNormal : public BaseEnemy {
public:

	GarbageBagNormal(Objects& objects_, Vec2 pos_);

	void move()override;

	RectF collision()const override;

	void draw()override;
};

class GarbageBagFast : public BaseEnemy {
public:
	GarbageBagFast(Objects& objects_, Vec2 pos_);
	void move()override;

	RectF collision()const override;

	void draw()override;
};
