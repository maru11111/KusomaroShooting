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
	TextureAsset::Register(U"PlayerHpHeal", Resource(U"texture/PlayerHpBarHeal.png"));
	TextureAsset::Register(U"HpBarDamage", Resource(U"texture/HPBarDamage.png"));
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
	TextureAsset::Register(U"UIBackWithBox", Resource(U"texture/UIBackWithBox.png"));
	TextureAsset::Register(U"Cloud", Resource(U"texture/Cloud.png"));
	TextureAsset::Register(U"CloudBig", Resource(U"texture/CloudBig.png"));
	TextureAsset::Register(U"CloudNormal", Resource(U"texture/CloudNormal.png"));
	TextureAsset::Register(U"CloudSmall", Resource(U"texture/CloudSmall.png"));
	TextureAsset::Register(U"CloudBigMorning", Resource(U"texture/CloudBigMorning.png"));
	TextureAsset::Register(U"CloudNormalMorning", Resource(U"texture/CloudNormalMorning.png"));
	TextureAsset::Register(U"CloudSmallMorning", Resource(U"texture/CloudSmallMorning.png"));
	TextureAsset::Register(U"CloudBigRain", Resource(U"texture/CloudBigRain.png"));
	TextureAsset::Register(U"CloudNormalRain", Resource(U"texture/CloudNormalRain.png"));
	TextureAsset::Register(U"CloudSmallRain", Resource(U"texture/CloudSmallRain.png"));
	TextureAsset::Register(U"CloudBigMidNight", Resource(U"texture/CloudBigMidNight.png"));
	TextureAsset::Register(U"CloudNormalMidNight", Resource(U"texture/CloudNormalMidNight.png"));
	TextureAsset::Register(U"CloudSmallMidNight", Resource(U"texture/CloudSmallMidNight.png"));
	TextureAsset::Register(U"StageNameIn", Resource(U"texture/StageNameIn.png"));
	TextureAsset::Register(U"StageNameOut", Resource(U"texture/StageNameOut.png"));
	TextureAsset::Register(U"Rain", Resource(U"texture/Rain.png"));
	TextureAsset::Register(U"SkyMorning", Resource(U"texture/SkyMorning.png"));
	TextureAsset::Register(U"SkyNoon", Resource(U"texture/SkyNoon.png"));
	TextureAsset::Register(U"SkyAfterNoon", Resource(U"texture/SkyAfterNoon.png"));
	TextureAsset::Register(U"SkyEvening", Resource(U"texture/SkyEvening.png"));
	TextureAsset::Register(U"SkyNight", Resource(U"texture/SkyNight.png"));
	TextureAsset::Register(U"SkyMidNight", Resource(U"texture/SkyMidNight.png"));
	TextureAsset::Register(U"UimmNormalForUI", Resource(U"texture/UimmNormalForUI.png"));
	TextureAsset::Register(U"UimmDamageForUI", Resource(U"texture/UimmDamageForUI.png"));
	TextureAsset::Register(U"DamageScreenEffect", Resource(U"texture/DamageScreenEffect.png"));

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
	AudioManager::Instance()->setAudio(U"DashAttack", Resource(U"audio/DashAttack.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"ReadyToDashAttack", Resource(U"audio/ReadyToDashAttack.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"ReadyToRollingAttackLong", Resource(U"audio/ReadyToRollingAttackLong.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"RollingAttack", Resource(U"audio/RollingAttack.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"RollingAttackLong", Resource(U"audio/RollingAttackLong.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"ReadyToThrowCan", Resource(U"audio/ReadyToThrowCan.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Open", Resource(U"audio/Open.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Close", Resource(U"audio/Close.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"BossHpAnimation", Resource(U"audio/BossHpAnimation.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"BossAppearPull", Resource(U"audio/BossAppearPull.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Kusomaro", Resource(U"audio/Kusomaro.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Can1", Resource(U"audio/Can1.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Can2", Resource(U"audio/Can2.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Can3", Resource(U"audio/Can3.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"AppearBoss", Resource(U"audio/swing20.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"PopMaroText", Resource(U"audio/PopMaroText.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Eat", Resource(U"audio/Eat.mp3"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Heal", Resource(U"audio/Heal.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"AddMaro", Resource(U"audio/AddMaro.wav"), AudioType::SE);
	AudioManager::Instance()->setAudio(U"Beep", Resource(U"audio/Beep.wav"), AudioType::SE);

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
	TextureAsset::Load(U"BackGroundSky");
	TextureAsset::Load(U"Empty");
	TextureAsset::Load(U"UIBackWithBox");
	TextureAsset::Load(U"StageNameIn");
	TextureAsset::Load(U"StageNameOut");
	TextureAsset::Load(U"Rain");
	TextureAsset::Load(U"SkyMorning");
	TextureAsset::Load(U"SkyNoon");
	TextureAsset::Load(U"SkyAfterNoon");
	TextureAsset::Load(U"SkyEvening");
	TextureAsset::Load(U"SkyNight");
	TextureAsset::Load(U"SkyMidNight");
	TextureAsset::Load(U"CloudSmall");
	TextureAsset::Load(U"CloudSmallMorning");
	TextureAsset::Load(U"CloudSmallRain");
	TextureAsset::Load(U"CloudNormal");
	TextureAsset::Load(U"CloudNormalMorning");
	TextureAsset::Load(U"CloudNormalRain");
	TextureAsset::Load(U"CloudBig");
	TextureAsset::Load(U"CloudBigMorning");
	TextureAsset::Load(U"CloudBigRain");
	TextureAsset::Load(U"CloudSmallMidNight");
	TextureAsset::Load(U"CloudSmallRainMidNight");
	TextureAsset::Load(U"CloudNormalMidNight");
	TextureAsset::Load(U"UimmNormalForUI");
	TextureAsset::Load(U"UimmDamageForUI");
	TextureAsset::Load(U"DamageScreenEffect");

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
	manager.init(State::Game, 0);
	//manager.init(State::StageEditor, 0);

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
