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

	void stageUpdate();

	void collisionAndRemoveUpdate();

	void updateWithHitStop();

	virtual void spawnEnemy();

	void destroyObjects();

	void commonDraw()const;

	void draw()const override;

	Objects& getObj();

	bool isHitStopping = false;;
	double hitStopTime = 0.15;
	double hitStopTimer = 0;
	double drawTimer = 0;
	double damageUIEffectTimer = 0;
	bool isDamageUIEffectPlaying=false;
	//int counterForSlowUpdate=0;
	double slowTimer=0;
	const double slowInterval = 1.0 / 60.0 * 3;

	void loadJson(String path)const;

	void drawHpBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase, double damageEase)const;
	void drawMaroBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase)const;
	void drawBossBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, BaseBoss *boss)const;
	void drawMarshmallowUI()const;

private:
	//GameSceneのステート
	enum class GameState {
		Tutorial,
		StageStart,
		Stage,
		BossAppear,
		BossBattle
	};
	GameState gameState = GameState::StageStart;
	bool isChangeGameState=false;
	GameState nextState;
	double gameStateTimer=0;
	double easeTimer1 = 0;
	double easeTimer2 = 0;
	double easeTimer3 = 0;
	double easeTimer4 = 0;
	double easeTimer5 = 0;
	double easeTimer6 = 0;
	double easeTimer7 = 0;



	bool isHpAnimationStart = false;
	bool isHpAnimationEnd = false;
	mutable double easeBossHpAnimationTimer = 0;
	mutable bool isPlayHpAnimation = false;

	double ease=0;
	double maxTopRectHeight = 50 + TextureAsset(U"UIBack").size().y*6;
	double maxBottomRectHeight = 50;

	//BossAppearのステート
	enum class BossAppearState {
		ChangeBackGround,
		HideUI,
		DrawRect,
		AppearBoss
	};
	BossAppearState bossAppearState = BossAppearState::ChangeBackGround;
	double marshmallowUIOffset = TextureAsset(U"UIBack").size().y * 6;
	double easeBossAppear=0.0;
	Camera2D camera{ Vec2{Scene::Center() + Vec2{0,-1}}, 1.0 , CameraControl::None_};
	bool isSpawnBoss = false;

	RenderTexture renderTexture{ Size(320, 214)*3 };
	RenderTexture fontRenderTexture{ Size(320, 214) * 3 };
	Effect effect;
	bool isEnemyTimeStopped=false;	

	//ステージ
	enum class Stage {
		Morning,
		Noon,
		AfterNoon,
		Evening,
		Night,
		MidNight
	};
	Array<String>stageName = {
		U"Uinitial Dawn",
		U"Blue Pallete Noon",
		U"a",
		U"a",
		U"a",
		U"a"
	};
	Stage currentStage=Stage::Morning;

	//ステージ開始演出
	enum class StageStartState {
		Start,
		Middle,
		End
	};
	double stageStartTimer = 0;
	double stageStartEaseTimer = 0;
	double stageStartAnimTimer = 0;
	Vec2 stageNameTextPos;
	Vec2 stageNameTextEndPos = Scene::CenterF() + Vec2{ -Scene::CenterF().x*1.5, 55};
	Vec2 stageNameTextMiddlePos = Scene::CenterF() + Vec2{0, 55};
	Vec2 stageNameTextStartPos = Scene::CenterF() + Vec2{ Scene::CenterF().x*1.5, 55 };
	StageStartState stageStartState = StageStartState::Start;

	//ステージ移動演出
	double backGroundOpacity = 1.0;
	double rainOpacity = 0.0;

protected:
	Objects objects;
	mutable Array<SpawnEnemyData> spawnEnemyData;
};

