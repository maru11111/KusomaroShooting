#include "Common.h"
#include "Load.h"
#include "Title.h"
#include "GameScene.h"
#include "StageEditor.h"
#include "Result.h"

//class PlayerBullet : public BaseBullet {
//public:
//
//	PlayerBullet(Vec2 vec_, double speed_, int power_)
//		: BaseBullet(vec_, speed_, power_)
//	{
//
//	}
//};

//class EnemyBullet : public BaseBullet {
//public:
//
//	EnemyBullet(Vec2 vec_, double speed_, int power_)
//		: BaseBullet(vec_, speed_, power_)
//	{
//
//	}
//};

void Main()
{
	// 
	const int scaling = 3;
	//Scene::Resize(Size(320, 214) * scaling);

	// 最近傍補間
	const ScopedRenderStates2D renderState{ SamplerState::ClampNearest };

	// ウィンドウリサイズ
	Window::Resize(Size(320, 214)*3);

	//オーディオマネージャー
	AudioManager::Init();

	//
	Window::SetTitle(U"クソマロシューティング？");

	//ロード画面で使うフォントだけロード
	FontAsset::Register(U"GameUI_Kei", 20, Resource(U"font/keifont.ttf"));
	FontAsset::Load(U"GameUI_Kei");


	App manager;
	manager.add<Load>(State::Load);
	manager.add<Title>(State::Title);
	manager.add<GameScene>(State::Game);
	manager.add<StageEditor>(State::StageEditor);
	manager.add<Result>(State::Result);

	//-から開始
	manager.init(State::Load, 1s);

	//manager.init(State::Title, 0s);
	//manager.init(State::Game, 0s);
	//manager.init(State::StageEditor, 0s);
	//manager.init(State::Result, 0s);

	while (System::Update())
	{
		ClearPrint();
		{
			if (not manager.update())
			{
				break;
			}
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
