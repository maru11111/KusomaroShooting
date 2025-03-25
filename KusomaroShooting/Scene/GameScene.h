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

	void dyingUpdate();

	void updateFadeIn(double t)override;

	virtual void spawnEnemy();

	void addScore(int score);

	void destroyObjects();

	void commonDraw()const;

	void drawBackground()const;

	void drawFadeIn(double t)const override;

	void draw()const override;

	Objects& getObj();

	bool isHitStopping = false;;
	const double hitStopTime = 0.15;
	double hitStopTimer = 0;
	bool isHitStopStart = false;
	double drawTimer = 0;
	double damageUIEffectTimer = 0;
	bool isDamageUIEffectPlaying=false;
	//int counterForSlowUpdate=0;
	double slowTimer=0;
	const double slowInterval = 1.0 / 60.0 * 3;

	bool isInitDyingVar=false;
	const double dyingHitStopTime = 3.0;
	const double dyingSlowInterval = 1.0 / 60.0 * 3 /*30/1.5*/;

	void loadJson(String path)const;

	void drawUIUimm(double offsetX, double offsetY)const;
	void drawHpBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase, double damageEase)const;
	void drawMaroBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase)const;
	void drawBossBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, BaseBoss *boss)const;
	void drawMarshmallowUI()const;

private:
	void changeStage(Stage nextStage);

	//スコア
	int currentScore = 0;
	int stageStartScore = 0;
	double prevScore = 0;
	double currentTime = 0;
	double scoreAnimTimer = 0;
	bool isPlayScoreAnim = false;

	//GameSceneのステート
	enum class GameState {
		Tutorial,
		StageStart,
		Stage,
		BossAppear,
		BossBattle,
		Pause
	};
	GameState gameState = GameState::StageStart;
	GameState prevGameState;
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
	Array<String>stageName = {
		U"Uinitial Dawning",
		U"Breezy Noon",
		U"E Rain",
		U"Amber Sunset",
		U"Mortal Night", 
		U"-",
		U"EDITOR"
	};
	Array<String>stageSubName = {
		U"ウイニシャル ドーニング",
		U"ブリージー ヌーン",
		U"イー レイン",
		U"アンバー サンセット",
		U"モータル ナイト",
		U"-",
		U"デバッグ"
	};

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
	double changeStageTimer = 0;

	//ゲームオーバー
	enum class SelectedButton {
		Continue,
		ReStart,
		BackToTitle
	};
	mutable SelectedButton selectedButton = SelectedButton::ReStart;
	double gameOverTimer = 0;
	//String currentBGMName=U"Start";

	//ヒットストップ時audio停止用フラグ
	bool isPlayAfterPause=false;

	//ボスのポインタ
	GarbageBox* bossPtr=nullptr;

	//ボス撃破後の動き
	enum class DefeatBossState {
		Slow,
		BossFalling,
		ToMountain
	};
	DefeatBossState defeatBossState = DefeatBossState::Slow;
	void bossFallingUpdate();
	void toMountainUpdate();
	bool isInitDefeatBossState=false;
	double defeatBossStateTimer = 0;
	double cityTimer = 0;
	double backGroundSpeedEase = 1;
	mutable double prevFrontCityPosX;
	mutable double prevMiddleCityPosX;
	mutable double prevBackCityPosX;

	mutable double frontCityPosX;
	mutable double middleCityPosX;
	mutable double backCityPosX;

	//ポーズ画面
	enum class PauseState {
		GoBack,
		Retry,
		Title,
		Config,
		ConfigMode
	};
	PauseState pauseState = PauseState::GoBack;
	const double PauseTriangleSize=22.5;
	double pauseTriangleTimer = 0;

	//チュートリアル
	enum class TutorialState {
		Move,
		Attack,
		Maro1,
		Maro2,
		Score,
		Pause,
		Try
	};
	TutorialState tutorialState = TutorialState::Move;
	double spawnTimer = 0;

	//フェードイン
	bool isFadingIn = false;
	double fadeInTimerEase = 0;
	double fadeInTimer = 0;

protected:
	Objects objects;
	mutable Array<SpawnEnemyData> spawnEnemyData;
	Stage currentStage = Stage::Morning;
};

