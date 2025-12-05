#pragma once
#include "../Util/Common.h"
#include "../Entity/Objects.h"
#include "GameScene.h"
#include "../Entity/SpawnEnemyData.h"

class GameSceneUI;

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

	double getBackDrawTimer() {
		return getData().backgroundDrawTimer;
	}

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

	/* ==== UI分離時publicに移動 ==== */
	Objects objects;
	bool isHpAnimationStart = false;
	bool isHpAnimationEnd = false;

	//BossAppearのステート
	enum class BossAppearState {
		ChangeBackGround,
		HideUI,
		DrawRect,
		AppearBoss
	};
	BossAppearState bossAppearState = BossAppearState::ChangeBackGround;
	double marshmallowUIOffset = TextureAsset(U"UIBack").size().y * 6;
	double easeBossAppear = 0.0;

	//ボスのポインタ
	GarbageBox* bossPtr = nullptr;
	mutable double easeBossHpAnimationTimer = 0;

	RenderTexture fontRenderTexture{ Size(320, 214) * 3 };

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

	//ステージ名
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

	// 現在ステージ
	Stage currentStage = Stage::Morning;

	// スコア
	int currentScore = 0;
	double prevScore = 0;
	double scoreAnimTimer = 0;

	//ゲームオーバー
	enum class SelectedButton {
		Continue,
		ReStart,
		BackToTitle
	};
	mutable SelectedButton selectedButton = SelectedButton::ReStart;
	double gameOverTimer = 0;

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

	//ステージ開始演出
	enum class StageStartState {
		Start,
		Middle,
		End
	};
	StageStartState stageStartState = StageStartState::Start;
	double stageStartAnimTimer = 0;
	double stageStartEaseTimer = 0;

	Vec2 stageNameTextMiddlePos = Scene::CenterF() + Vec2{ 0, 55 };
	Vec2 stageNameTextPos;

	double ease = 0;
	double maxTopRectHeight = 50 + TextureAsset(U"UIBack").size().y * 6;
	double maxBottomRectHeight = 50;

	Camera2D camera{ Vec2{Scene::Center() + Vec2{0,-1}}, 1.0 , CameraControl::None_ };

	//ポーズ画面
	enum class PauseState {
		GoBack,
		Retry,
		Title,
		Config,
		ConfigMode
	};
	PauseState pauseState = PauseState::GoBack;
	const double PauseTriangleSize = 22.5;
	double pauseTriangleTimer = 0;
	/* ============================== */

private:
	// UIマネージャー
	GameSceneUI *gameUI;

	void changeStage(Stage nextStage);

	//スコア
	int stageStartScore = 0;
	double currentTime = 0;
	bool isPlayScoreAnim = false;

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

	mutable bool isPlayHpAnimation = false;

	bool isSpawnBoss = false;

	RenderTexture renderTexture{ Size(320, 214)*3 };
	Effect effect;
	bool isEnemyTimeStopped=false;	

	double stageStartTimer = 0;
	Vec2 stageNameTextEndPos = Scene::CenterF() + Vec2{ -Scene::CenterF().x*1.5, 55};
	Vec2 stageNameTextStartPos = Scene::CenterF() + Vec2{ Scene::CenterF().x*1.5, 55 };

	//ステージ移動演出
	double backGroundOpacity = 1.0;
	double rainOpacity = 0.0;
	double changeStageTimer = 0;

	//ヒットストップ時audio停止用フラグ
	bool isPlayAfterPause=false;

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

	double spawnTimer = 0;

	//フェードイン
	bool isFadingIn = false;
	double fadeInTimerEase = 0;
	double fadeInTimer = 0;

protected:
	mutable Array<SpawnEnemyData> spawnEnemyData;
};

