#pragma once

enum class MaroType {
	Normal,
	Up,
	Down,
	Sine,
	Beem,
	Empty
};

class BaseBullet {
public:

	BaseBullet(Vec2 pos_);

	~BaseBullet();

	void update();

	virtual void move() = 0;

	void setIsHit(bool isHit_);

	bool isOffScreen();

	bool isDestroy();

	int getDamageAmount();

	RectF collision();

	MaroType getType();

	virtual void draw();

protected:
	Vec2 vec;
	Vec2 pos;
	double speed;
	int damageAmount;
	static int numInstances;
	MaroType type;
	bool isHit=false;
};

class NormalMarshmallow : public BaseBullet {
public:
	NormalMarshmallow(Vec2 pos_);
	void move()override;
};

class KusoMarshmallowUp : public BaseBullet {
public:
	KusoMarshmallowUp(Vec2 pos_);
	void move()override;
	void draw()override;
};

class KusoMarshmallowDown : public BaseBullet {
public:
	KusoMarshmallowDown(Vec2 pos_);
	void move()override;
	void draw()override;
};

class KusoMarshmallowSine : public BaseBullet {
public:
	KusoMarshmallowSine(Vec2 pos_);
	void move()override;
	void draw()override;
};
