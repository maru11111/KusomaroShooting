#pragma once
# include<Siv3D.hpp> // Siv3D v0.6.14
#include "AudioManager.h"

enum class State {
	//Title,
	Game,
	StageEditor
};

enum class EnemyType {
	Empty,
	Bag,
	FastBag,
	BagWithCan,
	Can,
	Fish,
	Umbrella
};

/*TextureAsset、総フレーム数、1フレームにかかる時間を受け取って
スプライトアニメーションを再生*/
inline void drawSpriteAnim(String textureName, int numFrame, double oneFrameTime, Vec2 pos) {
	int n = (int)(Scene::Time() / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).drawAt(pos);
}
//rotateバージョン
inline void drawRotateSpriteAnim(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double angle) {
	int n = (int)(Scene::Time() / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).rotated(angle).drawAt(pos);
}
//外部のタイマーを使って再生(1フレーム目から再生できるように)
inline void drawSpriteAnim(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double timer) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).drawAt(pos);
}
//外部のタイマーを使って再生(1フレーム目から再生できるように)+scale
inline void drawSpriteAnim(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double timer, int scale) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(scale).drawAt(pos);
}
//総フレーム数、1フレームにかかる時間、timerから現在のフレームを返す
inline int currentFrame(int numFrame, double oneFrameTime, double timer) {
	return (int)(timer / oneFrameTime) % numFrame + 1;
}
//あるフレームのみを描画
inline void drawSprite(String textureName, int numFrame, int n ,Vec2 pos) {
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
		AudioManager::Instance()->play(U"PopMaroText");
	}

	bool update(double t)override {
		//Print << U"timer" << timer;
		timer += Scene::DeltaTime();
		double ease = Min(EaseOutQuart(timer / time), 1.0);
		//FontAsset(U"GameUI_Kei")(str).drawAt(pos.movedBy(0, ease * posYOffset), ColorF(1.0, 1.0 - ease));
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
		AudioManager::Instance()->play(U"PopMaroText");
	}

	bool update(double t)override {
		timer += Scene::DeltaTime();
		double ease = Min(EaseOutQuart(timer / time), 1.0);

		drawedStr = U"";
		pos.y = initPosY;

		for (int i = 0; i < str.size(); i++) {
			if (str[i] == U'R') {
				isRedNext = true;
			}
			else if (str[i] == U'\n') {
				//pos.y += FontAsset(U"GameUI_Kei").height();
				drawedStr = U"";
			}
			else if (isRedNext) {
				//FontAsset(U"GameUI_Kei")(str[i]).drawAt(pos.movedBy(FontAsset(U"GameUI_Kei")(drawedStr).region().w, ease * posYOffset), ColorF(0.9, 0.0, 0.0, 1.0 - ease));
				drawedStr += str[i];
				isRedNext = false;

			}
			else {
				//FontAsset(U"GameUI_Kei")(str[i]).drawAt(pos.movedBy(FontAsset(U"GameUI_Kei")(drawedStr).region().w, ease * posYOffset), ColorF(1.0, 1.0 - ease));
				drawedStr += str[i];
			}
		}
		return (timer < time);
	}
};

using App = SceneManager<State>;
