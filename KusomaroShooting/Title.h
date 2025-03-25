#pragma once
#include "Common.h"
#include "GameScene.h"

class Title : public App::Scene
{
public:
	Title(const InitData& init);
	~Title();

	void update()override;

	void selectStateUpdate();

	void rankingStateUpdate();

	void configStateUpdate();

	void updateFadeIn(double t)override;

	void updateFadeOut(double t)override;

	//void drawFadeIn(double t)const override;

	void drawFadeOut(double t)const override;

	void drawBackground()const;
	void draw()const override;

	mutable double timer = 0;

	double fadeOutTimer = 0;

	double buttonAnimTimer = 0;

	const double BackgroundBlackMaxOpacity=0.4;
	double backgroundBlackOpacity=0.4;

	double fadeOutPosX=300;

	double fadeOutPosY = -300;

	double fadeInPosY = -150;

	double startButtonTimer = 0;
	double tutorialButtonTimer = 0;
	double rankingButtonTimer = 0;
	double settingButtonTimer = 0;
	double titleTimer = 0;
	double uiBackTimer = 0;
	double hpTimer = 0;
	double maroTimer = 0;
	double boxTimer = 0;

	bool fade1 = false;
	bool fade2 = false;
	bool fade3 = false;
	bool fade4 = false;
	bool fade5 = false;
	bool fade6 = false;
	bool fade7 = false;
	bool fade8 = false;
	bool fade9 = false;

	bool bgmStopFlag=false;

	//遷移先
	enum class NextGameState {
		Tutorial,
		Game
	};
	NextGameState nextGameState;

	//ボタン
	enum class Button {
		Start,
		Tutorial,
		Ranking,
		Config
	};
	Button button = Button::Start;

	//タイトル画面の状態
	enum class TitleState {
		Select,
		Ranking,
		Config
	};
	TitleState titleState = TitleState::Select;
};

