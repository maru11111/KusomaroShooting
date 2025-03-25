#pragma once
# include<Siv3D.hpp> // Siv3D v0.6.14
#include "AudioManager.h"
#include "Ranking/LeaderBoard.h"

const ColorF activeColor = ColorF(0.9);
const ColorF inactiveColor = ColorF(0.5);
const ColorF shadowColor = ColorF(0.2);
const ColorF inactiveColor2 = ColorF(149 / 255.0, 36 / 255.0, 36 / 255.0, 0.9);
const ColorF configColor = ColorF(142 / 255.0, 55 / 255.0, 55 / 255.0);
const ColorF configActiveColor = ColorF(255 / 255.0, 202 / 255.0, 80 / 255.0);

enum class Stage {
	Morning,
	Noon,
	AfterNoon,
	Evening,
	Night,
	MidNight,
	Editor
};

struct GameData
{
	int lastClearScore=0;
	double lastClearTime=0;
	//int lastContinueScore = 0;
	bool isTutorial = false;
	Stage startStage = Stage::AfterNoon;
	//bool isNoContinueClear = true;
	double backgroundDrawTimer = 0;
	bool startFromTitle = false;
};

enum class State {
	Load,
	Title,
	Game,
	Result,
	StageEditor
};

enum class EnemyType {
	Empty,
	Bag,
	AccelBag,
	SineBag,
	FastBag,
	BagWithCan,
	BagWithCanStay,
	Can,
	Fish,
	Umbrella,
	HealUmbrella
};

/*TextureAsset、総フレーム数、1フレームにかかる時間を受け取って
スプライトアニメーションを再生*/
inline void drawSpriteAnimAt(String textureName, int numFrame, double oneFrameTime, Vec2 pos) {
	int n = (int)(Scene::Time() / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).drawAt(pos);
}
//rotateバージョン
inline void drawRotateSpriteAnimAt(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double angle) {
	int n = (int)(Scene::Time() / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).rotated(angle).drawAt(pos);
}
//rotateバージョン+Color
inline void drawRotateSpriteAnimAt(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double angle, ColorF color) {
	int n = (int)(Scene::Time() / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).rotated(angle).drawAt(pos, color);
}
//scale + not drawAt
inline void drawSpriteAnim(String textureName, int numFrame, double oneFrameTime, Vec2 pos, int scale) {
	int n = (int)(Scene::Time() / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(scale).draw(pos);
}
//外部のタイマーを使って再生(1フレーム目から再生できるように)
inline void drawSpriteAnimForTimerAt(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double timer) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).drawAt(pos);
}
//外部のタイマー + rotate
inline void drawRotateSpriteAnimForTimerAt(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double angle, double timer) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).rotated(angle).drawAt(pos);
}
//外部のタイマーを使って再生(1フレーム目から再生できるように)+scale
inline void drawSpriteAnimForTimerAt(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double timer, int scale) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(scale).drawAt(pos);
}
//外部のタイマーを使って再生(1フレーム目から再生できるように) + scale + not drawAt
inline void drawSpriteAnimForTimer(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double timer, int scale, bool flag) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(scale).draw(pos);
}
//総フレーム数、1フレームにかかる時間、timerから現在のフレームを返す
inline int currentFrame(int numFrame, double oneFrameTime, double timer) {
	return (int)(timer / oneFrameTime) % numFrame + 1;
}
//あるフレームのみを描画
inline void drawSpriteAt(String textureName, int numFrame, int n ,Vec2 pos) {
	n--;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).drawAt(pos);
}

//クソマロ文章エフェクト
struct KusomaroTextEffect : IEffect {

	double timer = 0;
	double time = 3.0;
	Vec2 pos;
	String str;
	double opacity = 1.0;
	double posYOffset = -20;

	explicit KusomaroTextEffect(const Vec2 pos_, String str_)
		: pos{ pos_ }
		, str{ str_ }

	{
		//文字を出す座標が画面外だったら
		//少し画面に映るようにする
		if (Scene::Size().y <= pos.y) pos.y = Scene::Size().y - 12.5;
		if (Scene::Size().x <= pos.x) pos.x = Scene::Size().x - 50;
		//効果音
		AudioManager::Instance()->playOneShot(U"PopMaroText");

		//Print << U"effect";
	}

	bool update(double t)override {
		timer += Scene::DeltaTime();
		double ease = Min(EaseOutQuart(timer / time), 1.0);
		FontAsset(U"GameUI_Kei")(str).drawAt(pos.movedBy(0+2.0, ease * posYOffset+2.0), ColorF(0.0, 0.5 - ease/2.0));
		FontAsset(U"GameUI_Kei")(str).drawAt(pos.movedBy(0, ease * posYOffset), ColorF(0.95, 1.0 - ease));
		return (timer < time);
	}
};
//ビーム文章エフェクト
struct BeamTextEffect : IEffect {

	double timer = 0;
	double time = 5.0;
	Vec2 pos;
	double initPosY;
	String str;
	String drawedStr = U"";
	double opacity = 1.0;
	double posYOffset = -20;
	bool isRedNext = false;

	explicit BeamTextEffect(const Vec2 pos_, String str_)
		: pos{ pos_ }
		, str{ str_ }
		, initPosY{ pos_.y }
	{
		//文字を出す座標が画面外だったら
		//少し画面に映るようにする
		if (Scene::Size().y <= pos.y) {
			pos.y = Scene::Size().y - 12.5;
			initPosY += pos.y;
		};
		if (Scene::Size().x <= pos.x) pos.x = Scene::Size().x - 50;
		//効果音
		AudioManager::Instance()->playOneShot(U"PopMaroText");
	}

	bool update(double t)override {
		timer += Scene::DeltaTime();
		double ease = Min(EaseOutQuad(timer / time), 1.0);

		drawedStr = U"";
		pos.y = initPosY;

		for (int i = 0; i < str.size(); i++) {
			if (str[i] == U'R') {
				isRedNext = true;
			}
			else if (str[i] == U'\n') {
				pos.y += FontAsset(U"GameUI_Kei").height();
				drawedStr = U"";
			}
			else if (isRedNext) {
				FontAsset(U"GameUI_Kei")(str[i]).drawAt(pos.movedBy(FontAsset(U"GameUI_Kei")(drawedStr).region().w+2.0, ease * posYOffset+2.0), ColorF(50/255.0, 0 / 255.0, 0 / 255.0, 0.5 - ease/2.0));
				FontAsset(U"GameUI_Kei")(str[i]).drawAt(pos.movedBy(FontAsset(U"GameUI_Kei")(drawedStr).region().w, ease * posYOffset), ColorF(0.9, 0.0, 0.0, 1.0 - ease));
				drawedStr += str[i];
				isRedNext = false;

			}
			else {
				FontAsset(U"GameUI_Kei")(str[i]).drawAt(pos.movedBy(FontAsset(U"GameUI_Kei")(drawedStr).region().w+2.0, ease * posYOffset+2.0), ColorF(0.3, 0.5 - ease/2.0));
				FontAsset(U"GameUI_Kei")(str[i]).drawAt(pos.movedBy(FontAsset(U"GameUI_Kei")(drawedStr).region().w, ease * posYOffset), ColorF(0.95, 1.0 - ease));
				drawedStr += str[i];
			}
		}
		return (timer < time);
	}
};

//コンフィグ
enum class ConfigState {
	BGM,
	SE,
	Maro
};
inline ConfigState configState = ConfigState::BGM;
inline bool isSetting = false;
inline bool isDrawGuide = false;
const double ConfigTriangleSize = 22.5;
inline double configAnimTimer = 0;

inline bool confirmInput() {
	if (KeyZ.down() || KeyN.down() || KeySpace.down() || KeyEnter.down()) return true;
	else return false;
}

inline void configUpdate() {
	configAnimTimer += Scene::DeltaTime();
	if (1.25 <= configAnimTimer) {
		configAnimTimer = 0;
	}

	switch (configState) {
	case ConfigState::BGM:
		//非設定状態
		if (not isSetting) {
			//移動
			if (KeyDown.down() || KeyS.down()) {
				configAnimTimer = 0;
				configState = ConfigState::SE;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
				break;
			}
			//設定状態に移行
			if (confirmInput()) {
				configAnimTimer = 0;
				isSetting = true;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}
		}
		//設定状態
		else {
			//設定終了
			if (confirmInput() || KeyShift.down()) {
				configAnimTimer = 0;
				isSetting = false;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}

			//設定変更
			if (KeyA.down() || KeyLeft.down()) {
				if (AudioManager::Instance()->bgmLevel != -5) {
					configAnimTimer = 0;
					AudioManager::Instance()->changeBgmVol(-1);
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				else {
					AudioManager::Instance()->playOneShot(U"Beep");
				}
			}
			if (KeyD.down() || KeyRight.down()) {
				if (AudioManager::Instance()->bgmLevel != 5) {
					configAnimTimer = 0;
					AudioManager::Instance()->changeBgmVol(1);
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				else {
					AudioManager::Instance()->playOneShot(U"Beep");
				}
			}
		}
		break;

	case ConfigState::SE:
		//非設定状態
		if (not isSetting) {
			//移動
			if (KeyUp.down() || KeyW.down()) {
				configAnimTimer = 0;
				configState = ConfigState::BGM;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
				break;
			}
			if (KeyDown.down() || KeyS.down()) {
				configAnimTimer = 0;
				configState = ConfigState::Maro;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
				break;
			}
			//設定状態に移行
			if (confirmInput()) {
				configAnimTimer = 0;
				isSetting = true;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}
		}
		//設定状態
		else {
			//設定状態を終了
			if (confirmInput() || KeyShift.down()) {
				configAnimTimer = 0;
				isSetting = false;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}

			//設定変更
			if (KeyA.down() || KeyLeft.down()) {
				configAnimTimer = 0;
				if (AudioManager::Instance()->seLevel != -5) {
					AudioManager::Instance()->changeSeVol(-1);
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				else {
					AudioManager::Instance()->playOneShot(U"Beep");
				}
			}
			if (KeyD.down() || KeyRight.down()) {
				if (AudioManager::Instance()->seLevel != 5) {
					configAnimTimer = 0;
					AudioManager::Instance()->changeSeVol(1);
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				else {
					AudioManager::Instance()->playOneShot(U"Beep");
				}
			}
		}
		break;

	case ConfigState::Maro:
		//非設定状態
		if (not isSetting) {
			//移動
			if (KeyUp.down() || KeyW.down()) {
				configAnimTimer = 0;
				configState = ConfigState::SE;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
				break;
			}
			//設定状態に移行
			if (confirmInput()) {
				configAnimTimer = 0;
				isSetting = true;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}
		}
		//設定状態
		else {
			//設定終了
			if (confirmInput() || KeyShift.down()) {
				configAnimTimer = 0;
				isSetting = false;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}

			//設定変更
			if (isDrawGuide) {
				if (KeyD.down() || KeyRight.down()) {
					configAnimTimer = 0;
					isDrawGuide = false;
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				if (KeyA.down() || KeyLeft.down()) {
					configAnimTimer = 0;
					AudioManager::Instance()->playOneShot(U"Beep");
				}
			}
			else {
				if (KeyA.down() || KeyLeft.down()) {
					configAnimTimer = 0;
					isDrawGuide = true;
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				if (KeyD.down() || KeyRight.down()) {
					configAnimTimer = 0;
					AudioManager::Instance()->playOneShot(U"Beep");
				}
			}
		}
		break;
	}
}

inline void configDraw() {
	//背景を暗く
	RectF(0, 0, Scene::Size()).draw(ColorF(0.0, 0.5));
	//コンフィググラ
	TextureAsset(U"Config").scaled(6).draw();

	switch (configState) {
	case ConfigState::BGM:
		if (not isSetting) {
			//三角
			Triangle(Scene::CenterF().movedBy(-200 - 27.5, -13 + 17.5), ConfigTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
			//BGMグラ
			TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->bgmLevel * 30, 0 });
		}
		else {
			//三角
			Triangle(Scene::CenterF().movedBy(-200 - 27.5, -13 + 17.5), ConfigTriangleSize).rotated(90_deg).draw(ColorF(activeColor));
			//BGMグラ
			TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->bgmLevel * 30, 0 }, ColorF(1.0, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
		}
		//SEグラ
		TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->seLevel * 30, 66 });

		//BGM
		FontAsset(U"GameUI_BestTenDot30")(U"BGM:").draw(Scene::CenterF().movedBy(-200 + 3.25, -13 + 3.25), inactiveColor2);
		FontAsset(U"GameUI_BestTenDot30")(U"BGM:").draw(Scene::CenterF().movedBy(-200, -13 + 0), activeColor);
		//SE
		//FontAsset(U"GameUI_BestTenDot30")(U"SE:").draw(Scene::CenterF().movedBy(-200 + 3.25, -13 + 65 + 3.25), inactiveColor2);
		FontAsset(U"GameUI_BestTenDot30")(U"SE:").draw(Scene::CenterF().movedBy(-200, -13 + 65 + 0), inactiveColor2);
		//マシュマロ文書
		FontAsset(U"GameUI_BestTenDot30")(U"操作方法表示:").draw(Scene::CenterF().movedBy(-200 - 3.25, -13 + 130 + 0), inactiveColor2);
		//ON OFF
		if (isDrawGuide) {
			//ON
			FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60 + 3.25, -13 + 130 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configActiveColor);
			//OFF
			FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configColor);
		}
		else {
			//ON
			FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configColor);
			//OFF
			FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140 + 3.25, -13 + 130 + 3.25), configColor);
			FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configActiveColor);
		}
		break;

	case ConfigState::SE:
		if (not isSetting) {
			//三角
			Triangle(Scene::CenterF().movedBy(-200 - 27.5, -13 + 65 + 17.5), ConfigTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
			//SEグラ
			TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->seLevel * 30, 66 });
		}
		else {
			//三角
			Triangle(Scene::CenterF().movedBy(-200 - 27.5, -13 + 65 + 17.5), ConfigTriangleSize).rotated(90_deg).draw(ColorF(activeColor));
			//SEグラ
			TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->seLevel * 30, 66 }, ColorF(1.0, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
		}
		//BGMグラ
		TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->bgmLevel * 30, 0 });


		//BGM
		FontAsset(U"GameUI_BestTenDot30")(U"BGM:").draw(Scene::CenterF().movedBy(-200, -13 + 0), inactiveColor2);
		//SE
		FontAsset(U"GameUI_BestTenDot30")(U"SE:").draw(Scene::CenterF().movedBy(-200 + 3.25, -13 + 65 + 3.25), inactiveColor2);
		FontAsset(U"GameUI_BestTenDot30")(U"SE:").draw(Scene::CenterF().movedBy(-200, -13 + 65 + 0), activeColor);
		//マシュマロ文書
		FontAsset(U"GameUI_BestTenDot30")(U"操作方法表示:").draw(Scene::CenterF().movedBy(-200 - 3.25, -13 + 130 + 0), inactiveColor2);
		//ON OFF
		if (isDrawGuide) {
			//ON
			FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60 + 3.25, -13 + 130 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configActiveColor);
			//OFF
			FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configColor);
		}
		else {
			//ON
			FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configColor);
			//OFF
			FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140 + 3.25, -13 + 130 + 3.25), configColor);
			FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configActiveColor);
		}
		break;

	case ConfigState::Maro:
		if (not isSetting) {
			//三角
			Triangle(Scene::CenterF().movedBy(-200 - 27.5, -13 + 130 + 17.5), ConfigTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
		}
		else {
			Triangle(Scene::CenterF().movedBy(-200 - 27.5, -13 + 130 + 17.5), ConfigTriangleSize).rotated(90_deg).draw(ColorF(activeColor));
		}
		//BGMグラ
		TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->bgmLevel * 30, 0 });
		//SEグラ
		TextureAsset(U"AudioButton").scaled(6).draw(Vec2{ AudioManager::Instance()->seLevel * 30, 66 });


		//BGM
		FontAsset(U"GameUI_BestTenDot30")(U"BGM:").draw(Scene::CenterF().movedBy(-200, -13 + 0), inactiveColor2);
		//SE
		FontAsset(U"GameUI_BestTenDot30")(U"SE:").draw(Scene::CenterF().movedBy(-200, -13 + 65 + 0), inactiveColor2);
		//マシュマロ文書
		FontAsset(U"GameUI_BestTenDot30")(U"操作方法表示:").draw(Scene::CenterF().movedBy(-200 - 3.25 + 3.25, -13 + 130 + 3.25), inactiveColor2);
		FontAsset(U"GameUI_BestTenDot30")(U"操作方法表示:").draw(Scene::CenterF().movedBy(-200 - 3.25, -13 + 130 + 0), activeColor);
		//ON OFF
		if (isDrawGuide) {
			if (isSetting) {
				//ON
				FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60 + 3.25, -13 + 130 + 3.25), ColorF(inactiveColor2, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
				FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), ColorF(configActiveColor, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
				//OFF
				FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configColor);
			}
			else {
				//ON
				FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60 + 3.25, -13 + 130 + 3.25), inactiveColor2);
				FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configActiveColor);
				//OFF
				FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configColor);
			}
		}
		else {
			if (isSetting) {
				//ON
				FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configColor);
				//OFF
				FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140 + 3.25, -13 + 130 + 3.25), ColorF(inactiveColor2, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));
				FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), ColorF(configActiveColor, (0.75 - Min(configAnimTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));

			}
			else {
				//ON
				FontAsset(U"GameUI_BestTenDot30")(U"ON").draw(Scene::CenterF().movedBy(60, -13 + 130 + 0), configColor);
				//OFF
				FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140 + 3.25, -13 + 130 + 3.25), configColor);
				FontAsset(U"GameUI_BestTenDot30")(U"OFF").draw(Scene::CenterF().movedBy(140, -13 + 130 + 0), configActiveColor);

			}
		}
		break;

	}
	//操作方法
	//FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 220 + 3, Scene::CenterF().y + 33 + 108 + 66 + 3 }, ColorF(0.3));
	//FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 220, Scene::CenterF().y + 33 + 108 + 66 }, ColorF(0.9));
	//FontAsset(U"GameUI_BestTenDot")(U"Enterで決定/").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 220-140 + 3, Scene::CenterF().y + 33 + 108 + 66 + 3 }, ColorF(0.3));
	//FontAsset(U"GameUI_BestTenDot")(U"Enterで決定/").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 220-140, Scene::CenterF().y + 33 + 108 + 66 }, ColorF(0.9));
	FontAsset(U"GameUI_BestTenDot")(U"Enterで決定 / Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 220 - 5 + 3, Scene::CenterF().y + 33 + 108 + 66 + 3 }, ColorF(0.3));
	FontAsset(U"GameUI_BestTenDot")(U"Enterで決定 / Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 220 - 5, Scene::CenterF().y + 33 + 108 + 66 }, ColorF(0.9));
}

using App = SceneManager<State, GameData>;
