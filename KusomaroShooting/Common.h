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
//外部のタイマーを使って再生(1フレーム目から再生できるように)
inline void drawSpriteAnim(String textureName, int numFrame, double oneFrameTime, Vec2 pos, double timer) {
	int n = (int)(timer / oneFrameTime) % numFrame;
	TextureAsset(textureName)(n * TextureAsset(textureName).size().x / (double)numFrame, 0, TextureAsset(textureName).size().x / (double)numFrame, TextureAsset(textureName).size().y).scaled(3).drawAt(pos);
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


using App = SceneManager<State>;
