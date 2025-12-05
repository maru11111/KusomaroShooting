#pragma once
#include "../../Entity/Enemy.h"

class GameScene;

class GameSceneUI{
public:
	GameSceneUI(GameScene *gameScene_);

	~GameSceneUI()=default;

	void drawUIUimm(double offsetX, double offsetY)const;
	void drawHpBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase, double damageEase)const;
	void drawMaroBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase)const;
	void drawBossBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, BaseBoss* boss)const;
	void drawMarshmallowUI()const;

	void commonUIDraw()const;
	void draw()const;

private:
	GameScene *gameScene;
};

