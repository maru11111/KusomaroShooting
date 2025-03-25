#include "stdafx.h"
#include "Title.h"

Title::Title(const InitData& init)
	: IScene{ init }
{
	AudioManager::Instance()->play(U"Title", 0s);
}

void Title::update() {
	//背景動かす
	getData().backgroundDrawTimer += Scene::DeltaTime()/7.0;

	switch (titleState) {
	case TitleState::Select:
		selectStateUpdate();
		break;

	case TitleState::Ranking:
		rankingStateUpdate();
		break;

	case TitleState::Config:
		configStateUpdate();
		break;
	}
}

void Title::selectStateUpdate() {
	//ボタンアニメーション
	buttonAnimTimer += Scene::DeltaTime();
	if (1.0 <= buttonAnimTimer) {
		buttonAnimTimer = 0;
	}

	switch (button) {
	case Button::Start:
		//ゲーム開始
		if (KeyEnter.down()) {
			AudioManager::Instance()->playOneShot(U"GameStart");
			AudioManager::Instance()->stop(U"Title", 1.5s);
			nextGameState = NextGameState::Game;
			getData().startFromTitle = true;
			changeScene(State::Game, 6s);
		}

		//移動
		if (KeyS.down() || KeyDown.down()) {
			button = Button::Tutorial;
			AudioManager::Instance()->playOneShot(U"ChangeButton");
			buttonAnimTimer = 0;
		}
		break;

	case Button::Tutorial:
		//チュートリアルに移動
		if (KeyEnter.down()) {
			AudioManager::Instance()->stop(U"Title", 0.5s);
			AudioManager::Instance()->playOneShot(U"Select");
			nextGameState = NextGameState::Tutorial;
			getData().isTutorial = true;
			changeScene(State::Game, 2s);
		}

		//移動
		if (KeyW.down() || KeyUp.down()) {
			button = Button::Start;
			AudioManager::Instance()->playOneShot(U"ChangeButton");
			buttonAnimTimer = 0;
		}
		if (KeyS.down() || KeyDown.down()) {
			button = Button::Ranking;
			AudioManager::Instance()->playOneShot(U"ChangeButton");
			buttonAnimTimer = 0;
		}
		break;

	case Button::Ranking:
		//ランキング表示
		if (KeyEnter.down()) {
			AudioManager::Instance()->playOneShot(U"Select");
			titleState = TitleState::Ranking;
		}

		//移動
		if (KeyW.down() || KeyUp.down()) {
			button = Button::Tutorial;
			AudioManager::Instance()->playOneShot(U"ChangeButton");
			buttonAnimTimer = 0;
		}
		if (KeyS.down() || KeyDown.down()) {
			button = Button::Config;
			AudioManager::Instance()->playOneShot(U"ChangeButton");
			buttonAnimTimer = 0;
		}
		break;

	case Button::Config:
		//コンフィグを開く
		if (confirmInput()) {
			AudioManager::Instance()->playOneShot(U"Select");
			titleState = TitleState::Config;
		}
		
		//移動
		if (KeyW.down() || KeyUp.down()) {
			button = Button::Ranking;
			AudioManager::Instance()->playOneShot(U"ChangeButton");
			buttonAnimTimer = 0;
		}
		break;
	}
}

void Title::rankingStateUpdate() {
	if (KeyShift.down()) {
		titleState = TitleState::Select;
		AudioManager::Instance()->play(U"Cancel");
	}
	LeaderBoard::Instance()->update();
}

void Title::configStateUpdate() {
	//前の画面に戻る
	if ((KeyShift.down()) && not isSetting) {
		titleState = TitleState::Select;
		AudioManager::Instance()->playOneShot(U"Cancel");
		return;
	}
	configUpdate();
}

Title::~Title() {
	LeaderBoard::Instance()->reset();
}

void Title::drawBackground()const {
	const double cloudBigPosX = -(int)(getData().backgroundDrawTimer / 0.0100) % (320 * 3);
	const double cloudNormalPosX = -(int)(getData().backgroundDrawTimer / 0.0125) % (320 * 3);
	const double cloudSmallPosX = -(int)(getData().backgroundDrawTimer / 0.0150) % (320 * 3);
	const double farBackMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0250) % (320 * 3);
	const double backMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0300) % (320 * 3);
	const double middleMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0175) % (320 * 3);
	const double frontMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0125) % (320 * 3);

	const double frontCityPosX = -(int)(getData().backgroundDrawTimer / 0.004125) % (320 * 3);
	const double middleCityPosX = -(int)(getData().backgroundDrawTimer / 0.003125) % (320 * 3);
	const double backCityPosX = -(int)(getData().backgroundDrawTimer / 0.002125) % (320 * 3);

	TextureAsset(U"SkyMorning").scaled(3).draw(0, 0);

	TextureAsset(U"BackGroundMountainBack").scaled(3).draw(backMountainPosX, 0);
	TextureAsset(U"BackGroundMountainBack").scaled(3).draw(Scene::Size().x + backMountainPosX, 0);
	TextureAsset(U"BackGroundMountainMiddle").scaled(3).draw(middleMountainPosX, 0);
	TextureAsset(U"BackGroundMountainMiddle").scaled(3).draw(Scene::Size().x + middleMountainPosX, 0);
	TextureAsset(U"BackGroundMountainFront").scaled(3).draw(frontMountainPosX, 0);
	TextureAsset(U"BackGroundMountainFront").scaled(3).draw(Scene::Size().x + frontMountainPosX, 0);
	TextureAsset(U"BackGroundCityBack").scaled(3).draw(frontCityPosX, 0);
	TextureAsset(U"BackGroundCityBack").scaled(3).draw(Scene::Size().x + frontCityPosX, 0);
	TextureAsset(U"BackGroundCityMiddle").scaled(3).draw(middleCityPosX, 0);
	TextureAsset(U"BackGroundCityMiddle").scaled(3).draw(Scene::Size().x + middleCityPosX, 0);
	TextureAsset(U"BackGroundCity").scaled(3).draw(backCityPosX, 0);
	TextureAsset(U"BackGroundCity").scaled(3).draw(Scene::Size().x + backCityPosX, 0);

	TextureAsset(U"CloudSmallMorning").scaled(3).draw(cloudSmallPosX, 0);
	TextureAsset(U"CloudSmallMorning").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
	TextureAsset(U"CloudNormalMorning").scaled(3).draw(cloudNormalPosX, 0);
	TextureAsset(U"CloudNormalMorning").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
	TextureAsset(U"CloudBigMorning").scaled(3).draw(cloudBigPosX, 0);
	TextureAsset(U"CloudBigMorning").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
}


void Title::updateFadeIn(double t) {
	//背景動かす
	getData().backgroundDrawTimer += Scene::DeltaTime() / 7.0;
}
//void Title::drawFadeIn(double t)const {
//	draw();
//}

void Title::updateFadeOut(double t) {
	switch (nextGameState) {
	case NextGameState::Game:
		fadeOutTimer += Scene::DeltaTime();

		if (not fade1) {
			AudioManager::Instance()->playOneShot(U"FadeOut");
			fade1 = true;
		}
		settingButtonTimer += Scene::DeltaTime() * 1.1;

		if (0.175 <= settingButtonTimer) {
			if (not fade2) {
				AudioManager::Instance()->playOneShot(U"FadeOut");
				fade2 = true;
			}
			rankingButtonTimer += Scene::DeltaTime() * 1.1;
		}

		if (0.175 <= rankingButtonTimer) {
			if (not fade3) {
				AudioManager::Instance()->playOneShot(U"FadeOut");
				fade3 = true;
			}
			tutorialButtonTimer += Scene::DeltaTime() * 1.1;
		}

		if (0.175 <= tutorialButtonTimer) {
			if (not fade4) {
				AudioManager::Instance()->playOneShot(U"FadeOut");
				fade4 = true;
			}
			startButtonTimer += Scene::DeltaTime() * 1.1;
		}

		if (0.175 <= startButtonTimer) {
			if (not fade5) {
				AudioManager::Instance()->playOneShot(U"FadeOut");
				fade5 = true;
			}
			titleTimer += Scene::DeltaTime() * 1.1;
		}

		if (0.175 * 4.5 <= titleTimer) {
			//次のbgmを鳴らす
			switch (getData().startStage) {
			case Stage::Morning:
				if (not AudioAsset(U"Morning").isPlaying()) AudioManager::Instance()->play(U"Morning", 3.625s);
				break;
			case Stage::Noon:
				if (not AudioAsset(U"Noon").isPlaying()) AudioManager::Instance()->play(U"Noon", 3.625s);
				break;
			case Stage::AfterNoon:
				if (not AudioAsset(U"AfterNoon").isPlaying()) AudioManager::Instance()->play(U"AfterNoon", 3.625s);
				break;
			case Stage::Evening:
				if (not AudioAsset(U"Evening").isPlaying()) AudioManager::Instance()->play(U"Evening", 3.625s);
				break;
			case Stage::Night:
				if (not AudioAsset(U"Night").isPlaying()) AudioManager::Instance()->play(U"Night", 3.625s);
				break;
			case Stage::MidNight:
				break;
			}
			if (not fade6) {
				AudioManager::Instance()->playOneShot(U"FadeIn");
				fade6 = true;
			}
			uiBackTimer += Scene::DeltaTime() * 1.1;
		}

		if (0.175 <= uiBackTimer) {
			//背景を明るく
			backgroundBlackOpacity = BackgroundBlackMaxOpacity * (1.0 - EaseInOutCubic(Min(uiBackTimer / 1.75, 1.0)));
			if (not fade7) {
				AudioManager::Instance()->playOneShot(U"FadeIn");
				fade7 = true;
			}
			hpTimer += Scene::DeltaTime()*1.1;
		}

		if (0.175 <= hpTimer) {
			if (not fade8) {
				AudioManager::Instance()->playOneShot(U"FadeIn");
				fade8 = true;
			}
			maroTimer += Scene::DeltaTime()*1.1;
		}

		if (0.175 <= maroTimer) {
			if (not fade9) {
				AudioManager::Instance()->playOneShot(U"FadeIn");
				fade9 = true;
			}
			boxTimer += Scene::DeltaTime()*1.1;
		}
		break;

	case NextGameState::Tutorial:
		//背景動かす
		getData().backgroundDrawTimer += Scene::DeltaTime() / 7.0;
		break;
	}
}

void Title::drawFadeOut(double t)const {
	switch (nextGameState) {
	case NextGameState::Game:
	{
		timer += Scene::DeltaTime();
		getData().backgroundDrawTimer += Scene::DeltaTime() / 7.0 * Max(7*2.5 * EaseInSine(Min(fadeOutTimer / 2.5, 1.0)), 1.0);
		drawBackground();
		RectF(0, 0, Scene::Size()).draw(ColorF(0.0, backgroundBlackOpacity));
		{
			const ScopedRenderStates2D renderState{ SamplerState::ClampLinear };
			TextureAsset(U"Title").scaled(0.5).draw(0, fadeOutPosY * EaseInOutQuart(Min(titleTimer, 1.0)));
		}

		const double easeBack = 1.0 - EaseInOutQuart(Min(uiBackTimer, 1.0));
		const double easeHp = 1.0 - EaseInOutQuart(Min(hpTimer, 1.0));
		const double easeMaro = 1.0 - EaseInOutQuart(Min(maroTimer, 1.0));
		const double easeBox = 1.0 - EaseInOutQuart(Min(boxTimer, 1.0));

		//UI背景
		TextureAsset(U"UIBack")(0 * TextureAsset(U"UIBack").size().x / 65, 0, TextureAsset(U"UIBack").size().x / 65.0, TextureAsset(U"UIBack").size().y).scaled(6).draw(-6, -6 + fadeInPosY * easeBack);
		//UI uimm
		TextureAsset(U"UimmNormalForUI")(0 * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0 - 6, -6 + fadeInPosY * easeBack);
		//HPバーの後ろ部分を描画 
		TextureAsset(U"PlayerBarBack").scaled(6).draw(50 * 3 - 4, 6 * 3 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeHp);
		//HPバーの前部分を描画
		TextureAsset(U"PlayerHpFront").scaled(6).draw(50 * 3 - 4 + 12, 6 * 3 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeHp);
		//マシュマロバーの後ろ部分を描画 
		TextureAsset(U"PlayerBarBack").scaled(6).draw(50 * 3 - 4, 24 * 3 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeMaro);
		//マシュマロバーの前部分を描画
		TextureAsset(U"MarshmallowBarFront").scaled(6).draw(50 * 3 - 4 + 12, 24 * 3 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeMaro);
		//マシュマロボックス
		TextureAsset(U"MarshmallowBox").scaled(6).drawAt((320 / 2 - 5) * 3, 24 * 3 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeBox);
		//HP
		FontAsset(U"GameUI_Pixel")(U"HP").draw(165 - 4 + 2.0, 24 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + 2.5 + fadeInPosY * easeHp, ColorF(0.25, 0.25, 0.25));
		FontAsset(U"GameUI_Pixel")(U"HP").draw(165 - 4, 24 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeHp);
		//MP
		FontAsset(U"GameUI_Pixel")(U"MP:", 30, U"/30").draw(165 - 4 + 2.0, 24 * 3 + 14 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + 2.0 + fadeInPosY * easeMaro, ColorF(0.25, 0.25, 0.25));
		FontAsset(U"GameUI_Pixel")(U"MP:", 30, U"/30").draw(165 - 4, 24 * 3 + 14 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeMaro);
		//next
		FontAsset(U"GameUI_Pixel")(U"Next").draw(423 + 2.0, 12 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + 2.0 + fadeInPosY * easeBox, ColorF(0.25, 0.25, 0.25));
		FontAsset(U"GameUI_Pixel")(U"Next").draw(423, 12 + 8 - 6 - 0.0 * TextureAsset(U"UIBack").size().y * 6 + fadeInPosY * easeBox);



		TextureAsset(U"StartButton").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(startButtonTimer, 1.0)), 0);
		TextureAsset(U"TutorialButton").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(tutorialButtonTimer, 1.0)), 0);
		TextureAsset(U"RankingButton").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(rankingButtonTimer, 1.0)), 0);
		TextureAsset(U"SettingButton").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(settingButtonTimer, 1.0)), 0);

		TextureAsset(U"StartStr").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(startButtonTimer, 1.0)), 0);
		TextureAsset(U"TutorialStr").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(tutorialButtonTimer, 1.0)), 0);
		TextureAsset(U"RankingStr").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(rankingButtonTimer, 1.0)), 0);
		TextureAsset(U"SettingStr").scaled(0.5).draw(fadeOutPosX * EaseInOutQuart(Min(settingButtonTimer, 1.0)), 0);
		break;
	}

	case NextGameState::Tutorial:
		draw();
		Rect(0, 0, Scene::Size()).draw(ColorF(0.0, t));
		break;
	}
}

void Title::draw()const {
	drawBackground();
	RectF(0, 0, Scene::Size()).draw(ColorF(0.0, BackgroundBlackMaxOpacity));

	TextureAsset(U"Title").scaled(0.5).draw();

	{
		const ScopedRenderStates2D renderState{ SamplerState::ClampLinear };

		switch (button) {
		case Button::Start:
			TextureAsset(U"StartButton").scaled(0.5).draw(ColorF(1.0, buttonAnimTimer / 2.0 + 0.4));
			TextureAsset(U"TutorialButton").scaled(0.5).draw();
			TextureAsset(U"RankingButton").scaled(0.5).draw();
			TextureAsset(U"SettingButton").scaled(0.5).draw();
			break;
		case Button::Tutorial:
			TextureAsset(U"StartButton").scaled(0.5).draw();
			TextureAsset(U"TutorialButton").scaled(0.5).draw(ColorF(1.0, buttonAnimTimer / 2.0 + 0.4));
			TextureAsset(U"RankingButton").scaled(0.5).draw();
			TextureAsset(U"SettingButton").scaled(0.5).draw();
			break;
		case Button::Ranking:
			TextureAsset(U"StartButton").scaled(0.5).draw();
			TextureAsset(U"TutorialButton").scaled(0.5).draw();
			TextureAsset(U"RankingButton").scaled(0.5).draw(ColorF(1.0, buttonAnimTimer / 2.0 + 0.4));
			TextureAsset(U"SettingButton").scaled(0.5).draw();
			break;
		case Button::Config:
			TextureAsset(U"StartButton").scaled(0.5).draw();
			TextureAsset(U"TutorialButton").scaled(0.5).draw();
			TextureAsset(U"RankingButton").scaled(0.5).draw();
			TextureAsset(U"SettingButton").scaled(0.5).draw(ColorF(1.0, buttonAnimTimer / 2.0 + 0.4));
			break;
		}

		//ボタンの文字
		TextureAsset(U"StartStr").scaled(0.5).draw();
		TextureAsset(U"TutorialStr").scaled(0.5).draw();
		TextureAsset(U"RankingStr").scaled(0.5).draw();
		TextureAsset(U"SettingStr").scaled(0.5).draw();

		//操作方法
		FontAsset(U"GameUI_BestTenDot")(U"矢印キーまたはWASDで移動／Enterで決定／Shiftで戻る").draw(Arg::bottomLeft = Vec2{ 0 + 6 + 3, Scene::Size().y - 6 + 3}, ColorF(0.25));
		FontAsset(U"GameUI_BestTenDot")(U"矢印キーまたはWASDで移動／Enterで決定／Shiftで戻る").draw(Arg::bottomLeft = Vec2{ 0 + 6    , Scene::Size().y - 6 }, ColorF(0.9));

		//FontAsset(U"GameUI_BestTenDot")(U"・矢印キーまたはWASDで移動").draw(Arg::bottomLeft = Vec2{ 0 + 3, Scene::Size().y - 3 + 3}, ColorF(0.25));
		//FontAsset(U"GameUI_BestTenDot")(U"・矢印キーまたはWASDで移動").draw(Arg::bottomLeft = Vec2{ 0    , Scene::Size().y - 3 }, ColorF(0.9));

		//FontAsset(U"GameUI_BestTenDot")(U"・Enterで決定").draw(Arg::bottomLeft = Vec2{ 0 + 350 + 3, Scene::Size().y - 3 + 3 }, ColorF(0.25));
		//FontAsset(U"GameUI_BestTenDot")(U"・Enterで決定").draw(Arg::bottomLeft = Vec2{ 0 + 350  , Scene::Size().y - 3 }, ColorF(0.9));

		//FontAsset(U"GameUI_BestTenDot")(U"・Shiftで戻る").draw(Arg::bottomLeft = Vec2{ 0 + 350 + 180 + 3, Scene::Size().y - 3 + 3 }, ColorF(0.25));
		//FontAsset(U"GameUI_BestTenDot")(U"・Shiftで戻る").draw(Arg::bottomLeft = Vec2{ 0 + 350 + 180    , Scene::Size().y - 3 }, ColorF(0.9));
	}

	switch (titleState) {
	case TitleState::Select:
		break;

	case TitleState::Ranking:
		RectF(0, 0, Scene::Size().x, Scene::Size().y).draw(ColorF(0.0, 0.6));
		LeaderBoard::Instance()->draw(Scene::CenterF());
		break;

	case TitleState::Config:
		configDraw();
		break;
	}
}
