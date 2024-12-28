#pragma once
#include "Common.h"
#include "Objects.h"

class GameScene : public App::Scene
{
public:
	GameScene(const InitData& init);

	void update()override;

	void draw()const override;

	void drawBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY)const;
	void drawBossBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY)const;
	void drawMarshmallowUI()const;

private:
	Objects objects;
	RenderTexture renderTexture{ Size(320, 214) };
	RenderTexture fontRenderTexture{ Size(320, 214) * 3 };
	Effect effect;
};

