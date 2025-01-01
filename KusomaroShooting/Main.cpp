#include "Common.h"
#include "GameScene.h"


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
	Window::Resize(Size(320, 214) * scaling);

	//登録
	//画像
	TextureAsset::Register(U"UiNormal", Resource(U"texture/UiNormal_33_28_3sheet.png"));
	TextureAsset::Register(U"Marshmallow", Resource(U"texture/mashumaro_27_30.png"));
	TextureAsset::Register(U"KusomaroBeam", Resource(U"texture/kusomaroBeam.png"));
	TextureAsset::Register(U"KusomaroUp", Resource(U"texture/kusomaroUp.png"));
	TextureAsset::Register(U"KusomaroDown", Resource(U"texture/kusomaroDown.png"));
	TextureAsset::Register(U"KusomaroSine", Resource(U"texture/kusomaroSine.png"));
	TextureAsset::Register(U"GarbageBag", Resource(U"texture/GarbageBag.png"));
	TextureAsset::Register(U"PlayerBarBack", Resource(U"texture/PlayerHpBarBack.png"));
	TextureAsset::Register(U"BossBarBack", Resource(U"texture/BossHpBarBack.png"));
	TextureAsset::Register(U"PlayerHpFront", Resource(U"texture/PlayerHpBarFront.png"));
	TextureAsset::Register(U"MarshmallowBarFront", Resource(U"texture/MarshmallowBarFront.png"));
	TextureAsset::Register(U"BossBarFront", Resource(U"texture/BossHpBarFront.png"));
	TextureAsset::Register(U"MarshmallowBox", Resource(U"texture/MarshmallowBox.png"));
	TextureAsset::Register(U"UIBack", Resource(U"texture/UIBack.png"));
	TextureAsset::Register(U"UiDamage", Resource(U"texture/UIDamage.png"));
	TextureAsset::Register(U"UiNormalAndBlink", Resource(U"texture/UiNormalAndBlink.png"));
	TextureAsset::Register(U"DamageEffect", Resource(U"texture/DamageEffect.png"));
	TextureAsset::Register(U"UiBeam", Resource(U"texture/UiBeam.png"));
	TextureAsset::Register(U"UiBeamUimama", Resource(U"texture/UiBeamUimama.png"));

	FontAsset::Register(U"GameUI_Kei", 20, Resource(U"font/keifont.ttf"));
	FontAsset::Register(U"GameUI_BestTen", 30, Resource(U"font/BestTen-CRT.otf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_BestTenDot", 25, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_BestTenDot30", 30, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_Pixel", 25, Resource(U"font/PixelMplus10-Regular.ttf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_Pixel6", 30, Resource(U"font/PixelMplus10-Regular.ttf"), FontStyle::Bitmap);

	//ロード
	//画像
	TextureAsset::Load(U"UiNormal");
	TextureAsset::Load(U"Marshmallow");
	TextureAsset::Load(U"GarbageBag");
	TextureAsset::Load(U"KusomaroUp");
	TextureAsset::Load(U"KusomaroDown");
	TextureAsset::Load(U"KusomaroSine");
	TextureAsset::Load(U"KusomaroBeam");
	TextureAsset::Load(U"PlayerBarBack");
	TextureAsset::Load(U"BossBarBack");
	TextureAsset::Load(U"PlayerHpFront");
	TextureAsset::Load(U"MarshmallowBarFront");
	TextureAsset::Load(U"BossBarFront");
	TextureAsset::Load(U"MarshmallowBox");
	TextureAsset::Load(U"UIBack");
	TextureAsset::Load(U"UiDamage");
	TextureAsset::Load(U"UiNormalAndBlink");
	TextureAsset::Load(U"DamageEffect");
	TextureAsset::Load(U"UiBeam");
	TextureAsset::Load(U"UiBeamUimama");

	//フォント
	FontAsset::Load(U"GameUI_Kei");
	FontAsset::Load(U"GameUI_BestTen");
	FontAsset::Load(U"GameUI_BestTenDot");
	FontAsset::Load(U"GameUI_BestTenDot30");
	FontAsset::Load(U"GameUI_Pixel");
	FontAsset::Load(U"GameUI_Pixel6");

	App manager;
	//manager.add<Title>(State::Title);
	manager.add<GameScene>(State::Game);

	//ゲームシーンから開始
	manager.init(State::Game, 0);

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
