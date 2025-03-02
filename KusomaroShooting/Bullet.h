#pragma once

struct Objects;

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

	//マシュマロ文章
	static Array<String>kusomaroTexts;
	static Array<String>beamTexts;

	BaseBullet(Objects& objects_, Vec2 pos_);

	virtual ~BaseBullet();

	virtual void update();

	virtual void move() = 0;

	void setIsHit(bool isHit_);

	bool isOffScreen();

	virtual bool isDestroy();

	int getDamageAmount();

	virtual RectF collision();

	MaroType getType();

	Vec2 getPos();

	//インスタンスにとって当たったことの無い敵かどうか
	bool isNotEnemyHit(int id);

	bool getIsHit();

	virtual void backGroundDraw()const;

	void addId(int id);

	virtual void draw();

protected:
	Objects& objects;
	Vec2 vec;
	Vec2 pos;
	double speed;
	int damageAmount;
	static int numInstances;
	MaroType type;
	//何かしらの敵に当たったことがあるか
	bool isHit=false;
	Array<int>idOfEnemyHitOnce;
	bool isDestroyNextFrame=false;
};

class NormalMarshmallow : public BaseBullet {
public:
	NormalMarshmallow(Objects& objects_, Vec2 pos_);
	void move()override;
};

class KusoMarshmallowUp : public BaseBullet {
public:
	KusoMarshmallowUp(Objects& objects_, Vec2 pos_);
	void move()override;
	void draw()override;
};

class KusoMarshmallowDown : public BaseBullet {
public:
	KusoMarshmallowDown(Objects& objects_, Vec2 pos_);
	void move()override;
	void draw()override;
};

class KusoMarshmallowSine : public BaseBullet {
public:
	KusoMarshmallowSine(Objects& objects_, Vec2 pos_);
	void move()override;
	void draw()override;
private:
	double timer=0;
};

class KusoMarshmallowBeam : public BaseBullet {
public:
	KusoMarshmallowBeam(Objects& objects_, Vec2 pos_);
	void move()override;
	bool isDestroy()override;
	void draw()override;
private:
	double timer = 0;
	bool isShotBeam=false;
};

class Beam : public BaseBullet {
public:
	Beam(Objects& objects_, Vec2 pos_);
	RectF collision()override;
	void move()override;
	bool isDestroy()override;
	void update()override;
	void backGroundDraw()const override;
	void draw()override;
private:
	double beamTimer = 0;
	double beamColTimer = 0;
	int beamColTime = 26 * 0.05;
	bool isColliderActive = false;
	bool isEndBeam = false;
	double backGroundOpacity = 0.1;
};
