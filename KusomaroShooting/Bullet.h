#pragma once

enum class MaroType {
	Normal,
	Up,
	Down,
	Sine,
	Beam,
	Empty
};

class BaseBullet {
public:

	BaseBullet(Vec2 pos_);

	virtual ~BaseBullet();

	virtual void update();

	virtual void move() = 0;

	void setIsHit(bool isHit_);

	bool isOffScreen();

	virtual bool isDestroy();

	int getDamageAmount();

	virtual RectF collision();

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

class KusoMarshmallowBeam : public BaseBullet {
public:
	KusoMarshmallowBeam(Vec2 pos_);
	RectF collision()override;
	void move()override;
	bool isDestroy()override;
	void update()override;
	void draw()override;
private:
	double beamTimer=0;
	double beamColTimer = 0;
	int beamColTime=26*0.05;
	bool isColliderActive=false;
	bool isEndBeam=false;
};
