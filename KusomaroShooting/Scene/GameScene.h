#pragma once
#include "Common.h"
#include "Objects.h"
#include "GameScene.h"
#include "SpawnEnemyData.h"

class GameScene : public App::Scene
{
public:
	GameScene(const InitData& init);

	void update()override;

	virtual void spawnEnemy();

	void destroyObjects();

	void draw()const override;

	Objects& getObj();

	void loadJson(String path)const;

	void drawBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY)const;
	void drawBossBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY)const;
	void drawMarshmallowUI()const;

private:
	RenderTexture renderTexture{ Size(320, 214)*3 };
	RenderTexture fontRenderTexture{ Size(320, 214) * 3 };
	Effect effect;
	bool isTimeStopped=false;
protected:
	Objects objects;
	mutable Array<SpawnEnemyData> spawnEnemyData;
};

