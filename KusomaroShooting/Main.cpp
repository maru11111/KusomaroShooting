#include "Common.h"
#include "GameScene.h"
#include "StageEditor.h"

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

	//オーディオマネージャー
	AudioManager::Init();

	//登録
	//画像
	TextureAsset::Register(U"UiNormal", Resource(U"texture/UiNormal_33_28_3sheet.png"));
	TextureAsset::Register(U"Marshmallow", Resource(U"texture/mashumaro_27_30.png"));
	TextureAsset::Register(U"KusomaroBeam", Resource(U"texture/kusomaroBeam.png"));
	TextureAsset::Register(U"KusomaroUp", Resource(U"texture/kusomaroUp.png"));
	TextureAsset::Register(U"KusomaroDown", Resource(U"texture/kusomaroDown.png"));
	TextureAsset::Register(U"KusomaroSine", Resource(U"texture/kusomaroSine.png"));
	TextureAsset::Register(U"GarbageBag", Resource(U"texture/GarbageBag.png"));
	TextureAsset::Register(U"GarbageBagWithCan", Resource(U"texture/GarbageBagWithCan.png"));
	TextureAsset::Register(U"GarbageBagFastForEditor", Resource(U"texture/GarbageBagFastForEditor.png"));
	TextureAsset::Register(U"GarbageBagWithCanForEditor", Resource(U"texture/GarbageBagWithCanForEditor.png"));
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
	TextureAsset::Register(U"RedCan", Resource(U"texture/RedCan.png"));
	TextureAsset::Register(U"GrapeCan", Resource(U"texture/GrapeCan.png"));
	TextureAsset::Register(U"OrangeCan", Resource(U"texture/OrangeCan.png"));
	TextureAsset::Register(U"WhiteCan", Resource(U"texture/WhiteCan.png"));
	TextureAsset::Register(U"Fish", Resource(U"texture/Fish.png"));
	TextureAsset::Register(U"FishForEditor", Resource(U"texture/FishForEditor.png"));
	TextureAsset::Register(U"FishWait", Resource(U"texture/FishWait.png"));
	TextureAsset::Register(U"UiAttackEffect", Resource(U"texture/UiAttackEffect.png"));
	TextureAsset::Register(U"Umbrella", Resource(U"texture/Umbrella.png"));
	TextureAsset::Register(U"BackGroundCity", Resource(U"texture/BackGroundCity.png"));
	TextureAsset::Register(U"BackGroundCityBack", Resource(U"texture/BackGroundCityBack.png"));
	TextureAsset::Register(U"BackGroundCityMiddle", Resource(U"texture/BackGroundCityMiddle.png"));
	TextureAsset::Register(U"BackGroundMountain", Resource(U"texture/BackGroundMountain.png"));
	TextureAsset::Register(U"BackGroundMountainBack", Resource(U"texture/BackGroundMountainBack.png"));
	TextureAsset::Register(U"BackGroundMountainMiddle", Resource(U"texture/BackGroundMountainMiddle.png"));
	TextureAsset::Register(U"BackGroundMountainFront", Resource(U"texture/BackGroundMountainFront.png"));
	TextureAsset::Register(U"Empty", Resource(U"texture/Empty.png"));
	TextureAsset::Register(U"GarbageBox", Resource(U"texture/GarbageBox.png"));
	TextureAsset::Register(U"GarbageBoxOpen", Resource(U"texture/GarbageBoxOpen.png"));

	FontAsset::Register(U"GameUI_Kei", 20, Resource(U"font/keifont.ttf"));
	FontAsset::Register(U"GameUI_BestTen", 30, Resource(U"font/BestTen-CRT.otf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_BestTenDot", 25, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_BestTenDot30", 30, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_Pixel", 25, Resource(U"font/PixelMplus10-Regular.ttf"), FontStyle::Bitmap);
	FontAsset::Register(U"GameUI_Pixel6", 30, Resource(U"font/PixelMplus10-Regular.ttf"), FontStyle::Bitmap);

	AudioManager::Instance()->setAudio(U"MidBoss", Resource(U"audio/MidBoss.wav"), AudioType::BGM);
	AudioManager::Instance()->setAudio(U"KaraKara", Resource(U"audio/KaraKara.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"ReceiveDamage", Resource(U"audio/ReceiveDamage.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Throw", Resource(U"audio/Throw.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"HitHigh", Resource(U"audio/HitHigh.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"HitLow", Resource(U"audio/HitLow.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"CloseRangeAttack", Resource(U"audio/CloseRangeAttack.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Can", Resource(U"audio/Can.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Beam", Resource(U"audio/Beam.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"BeamStart", Resource(U"audio/BeamStart.wav"), AudioType::SE);
	//ロード
	//画像
	TextureAsset::Load(U"UiNormal");
	TextureAsset::Load(U"Marshmallow");
	TextureAsset::Load(U"GarbageBag");
	TextureAsset::Load(U"GarbageBagWithCan");
	TextureAsset::Load(U"GarbageBagFastForEditor");
	TextureAsset::Load(U"GarbageBagWithCanForEditor");
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
	TextureAsset::Load(U"RedCan");
	TextureAsset::Load(U"GrapeCan");
	TextureAsset::Load(U"OrangeCan");
	TextureAsset::Load(U"WhiteCan");
	TextureAsset::Load(U"Fish");
	TextureAsset::Load(U"FishForEditor");
	TextureAsset::Load(U"FishWait");
	TextureAsset::Load(U"UiAttackEffect");
	TextureAsset::Load(U"Umbrella");
	TextureAsset::Load(U"BackGroundCity");
	TextureAsset::Load(U"BackGroundCityBack");
	TextureAsset::Load(U"BackGroundCityMiddle");
	TextureAsset::Load(U"BackGroundMountain");
	TextureAsset::Load(U"BackGroundMountainBack");
	TextureAsset::Load(U"BackGroundMountainMiddle");
	TextureAsset::Load(U"BackGroundMountainFront");
	TextureAsset::Load(U"Empty");
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
	manager.add<StageEditor>(State::StageEditor);

	//ゲームシーンから開始
	//manager.init(State::Game, 0);
	manager.init(State::StageEditor, 0);

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
