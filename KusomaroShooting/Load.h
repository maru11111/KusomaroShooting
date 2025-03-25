#pragma once
#include "Common.h"

class Load : public App::Scene {
public:
	bool isReady=false;
	bool registerFlag = false;
	double animTimer = 0;
	Vec2 strPos = Vec2{ Scene::Size().movedBy(-FontAsset(U"GameUI_Kei")(U"Now Loading...").region().w - 10, -5 -25) };

	int assetNum=0;
	int isReadyAssetNum=0;

	Load(const InitData& init)
		: IScene{init}
	{}

	void update()override {
		//Print << U"update";
		animTimer += Scene::DeltaTime();

		if (not registerFlag) {
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
			TextureAsset::Register(U"GarbageBagAccelForEditor", Resource(U"texture/GarbageBagAccelForEditor.png"));
			TextureAsset::Register(U"GarbageBagSineForEditor", Resource(U"texture/GarbageBagSineForEditor.png"));
			TextureAsset::Register(U"GarbageBagFastForEditor", Resource(U"texture/GarbageBagFastForEditor.png"));
			TextureAsset::Register(U"GarbageBagWithCanForEditor", Resource(U"texture/GarbageBagWithCanForEditor.png"));
			TextureAsset::Register(U"GarbageBagWithCanStayForEditor", Resource(U"texture/GarbageBagWithCanStayForEditor.png"));
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
			TextureAsset::Register(U"HealUmbrella", Resource(U"texture/HealUmbrella.png"));
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
			TextureAsset::Register(U"CloudSmallEvening", Resource(U"texture/CloudSmallEvening.png"));
			TextureAsset::Register(U"CloudBigEvening", Resource(U"texture/CloudBigEvening.png"));
			TextureAsset::Register(U"CloudNormalEvening", Resource(U"texture/CloudNormalEvening.png"));
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
			TextureAsset::Register(U"LowHpFrame", Resource(U"texture/LowHpFrame.png"));
			TextureAsset::Register(U"UimmCrisisForUI", Resource(U"texture/UimmCrisisForUI.png"));
			TextureAsset::Register(U"HealMaro", Resource(U"texture/HealMaro.png"));
			TextureAsset::Register(U"HealEffect", Resource(U"texture/HealEffect.png"));
			TextureAsset::Register(U"Batu", Resource(U"texture/Batu.png"));
			TextureAsset::Register(U"ClearStill", U"texture/ClearStill.png");
			TextureAsset::Register(U"Trophy", Resource(U"texture/Trophy.png"));
			TextureAsset::Register(U"Pause", Resource(U"texture/Pause.png"));
			TextureAsset::Register(U"Config", Resource(U"texture/Config.png"));
			TextureAsset::Register(U"AudioButton", Resource(U"texture/AudioButton.png"));
			TextureAsset::Register(U"TutorialMove", Resource(U"texture/TutorialMove.png"));
			TextureAsset::Register(U"TutorialAttack", Resource(U"texture/TutorialAttack.png"));
			TextureAsset::Register(U"TutorialMaro1", Resource(U"texture/TutorialMaro1.png"));
			TextureAsset::Register(U"TutorialMaro2", Resource(U"texture/TutorialMaro2.png"));
			TextureAsset::Register(U"TutorialScore", Resource(U"texture/TutorialScore.png"));
			TextureAsset::Register(U"TutorialPause", Resource(U"texture/TutorialPause.png"));
			TextureAsset::Register(U"Question", Resource(U"texture/Question.png"));
			TextureAsset::Register(U"WASD", Resource(U"texture/WASD.png"));
			TextureAsset::Register(U"NM", Resource(U"texture/NM.png"));
			TextureAsset::Register(U"MouseL", Resource(U"texture/MouseL.png"));
			TextureAsset::Register(U"MouseR", Resource(U"texture/MouseR.png"));
			TextureAsset::Register(U"Manual", Resource(U"texture/Manual.png"));
			TextureAsset::Register(U"Title", Resource(U"texture/Title.png"));
			TextureAsset::Register(U"TitleButton", Resource(U"texture/TitleButton.png"));
			TextureAsset::Register(U"TutorialButton", Resource(U"texture/TutorialButton.png"));
			TextureAsset::Register(U"StartButton", Resource(U"texture/StartButton.png"));
			TextureAsset::Register(U"RankingButton", Resource(U"texture/RankingButton.png"));
			TextureAsset::Register(U"SettingButton", Resource(U"texture/SettingButton.png"));
			TextureAsset::Register(U"TutorialStr", Resource(U"texture/TutorialStr.png"));
			TextureAsset::Register(U"StartStr", Resource(U"texture/StartStr.png"));
			TextureAsset::Register(U"RankingStr", Resource(U"texture/RankingStr.png"));
			TextureAsset::Register(U"SettingStr", Resource(U"texture/SettingStr.png"));


			FontAsset::Register(U"GameUI_BestTen14", 14, Resource(U"font/BestTen-CRT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTen", 30, Resource(U"font/BestTen-CRT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTenDot20", 20, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTenDot", 25, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTenDot30", 30, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTenDot45", 45, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTenDot60", 60, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_BestTenDot90", 90, Resource(U"font/BestTen-DOT.otf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_Pixel", 25, Resource(U"font/PixelMplus10-Regular.ttf"), FontStyle::Bitmap);
			FontAsset::Register(U"GameUI_Pixel6", 30, Resource(U"font/PixelMplus10-Regular.ttf"), FontStyle::Bitmap);

			AudioManager::Instance()->setAudio(U"MidBoss", Resource(U"audio/MidBoss.wav"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"KaraKara", Resource(U"audio/KaraKara.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"ReceiveDamage", Resource(U"audio/ReceiveDamage.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"ReceiveDamageDying", Resource(U"audio/ReceiveDamageDying.wav"), AudioType::SE);
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
			AudioManager::Instance()->setAudio(U"ThrowCan", Resource(U"audio/swing39.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"AppearBoss", Resource(U"audio/swing20.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"PopMaroText", Resource(U"audio/PopMaroText.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Eat", Resource(U"audio/Eat.mp3"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Heal", Resource(U"audio/Heal.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"AddMaro", Resource(U"audio/AddMaro.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Beep", Resource(U"audio/Beep.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"AddScore", Resource(U"audio/AddScore.wav"), AudioType::SE, Loop::Yes);
			AudioManager::Instance()->setAudio(U"AddScoreEnd", Resource(U"audio/AddScoreEnd.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"LowHp", Resource(U"audio/LowHp.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"ChangeButton", Resource(U"audio/ChangeButton.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Select", Resource(U"audio/Select.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Rain", Resource(U"audio/rain00.wav"), AudioType::SE, Loop::Yes);
			AudioManager::Instance()->setAudio(U"Wind", Resource(U"audio/windBlows.mp3"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Noon", Resource(U"audio/iwashiro_tsuki_no_senaka_Noon.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"Evening", Resource(U"audio/iwashiro_soda_biyori_Evening.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"AfterNoon", Resource(U"audio/iwashiro_shizori_zakura_AfterNoon.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"Morning", Resource(U"audio/iwashiro_happy_aim_8bit_Morning.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"Night", Resource(U"audio/iwashiro_gaufre_Night.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"ClearMovieAudio", Resource(U"audio/ClearMovieAudio.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"ResultWindBlow", Resource(U"audio/ResultWindBlow.wav"), AudioType::SE, Loop::Yes);
			AudioManager::Instance()->setAudio(U"DefeatBoss", Resource(U"audio/DefeatBoss.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Cancel", Resource(U"audio/Cancel.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Paper", Resource(U"audio/Paper.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"Tutorial", Resource(U"audio/iwashiro_tuto_and_rial_Tutorial.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"Title", Resource(U"audio/iwashiro_hello_hello_Title.ogg"), AudioType::BGM);
			AudioManager::Instance()->setAudio(U"FadeOut", Resource(U"audio/fadeOut.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"FadeIn", Resource(U"audio/fadeIn.wav"), AudioType::SE);
			AudioManager::Instance()->setAudio(U"GameStart", Resource(U"audio/GameStart.wav"), AudioType::SE);

			//ロード
			//画像
			TextureAsset::LoadAsync(U"UiNormal");
			TextureAsset::LoadAsync(U"Marshmallow");
			TextureAsset::LoadAsync(U"GarbageBag");
			TextureAsset::LoadAsync(U"GarbageBagWithCan");
			TextureAsset::LoadAsync(U"GarbageBagSineForEditor");
			TextureAsset::LoadAsync(U"GarbageBagAccelForEditor");
			TextureAsset::LoadAsync(U"GarbageBagFastForEditor");
			TextureAsset::LoadAsync(U"GarbageBagWithCanForEditor");
			TextureAsset::LoadAsync(U"GarbageBagWithCanStayForEditor");
			TextureAsset::LoadAsync(U"KusomaroUp");
			TextureAsset::LoadAsync(U"KusomaroDown");
			TextureAsset::LoadAsync(U"KusomaroSine");
			TextureAsset::LoadAsync(U"KusomaroBeam");
			TextureAsset::LoadAsync(U"PlayerBarBack");
			TextureAsset::LoadAsync(U"BossBarBack");
			TextureAsset::LoadAsync(U"PlayerHpFront");
			TextureAsset::LoadAsync(U"MarshmallowBarFront");
			TextureAsset::LoadAsync(U"BossBarFront");
			TextureAsset::LoadAsync(U"MarshmallowBox");
			TextureAsset::LoadAsync(U"UIBack");
			TextureAsset::LoadAsync(U"UiDamage");
			TextureAsset::LoadAsync(U"UiNormalAndBlink");
			TextureAsset::LoadAsync(U"DamageEffect");
			TextureAsset::LoadAsync(U"UiBeam");
			TextureAsset::LoadAsync(U"UiBeamUimama");
			TextureAsset::LoadAsync(U"RedCan");
			TextureAsset::LoadAsync(U"GrapeCan");
			TextureAsset::LoadAsync(U"OrangeCan");
			TextureAsset::LoadAsync(U"WhiteCan");
			TextureAsset::LoadAsync(U"Fish");
			TextureAsset::LoadAsync(U"FishForEditor");
			TextureAsset::LoadAsync(U"FishWait");
			TextureAsset::LoadAsync(U"UiAttackEffect");
			TextureAsset::LoadAsync(U"Umbrella");
			TextureAsset::LoadAsync(U"HealUmbrella");
			TextureAsset::LoadAsync(U"BackGroundCity");
			TextureAsset::LoadAsync(U"BackGroundCityBack");
			TextureAsset::LoadAsync(U"BackGroundCityMiddle");
			TextureAsset::LoadAsync(U"BackGroundMountain");
			TextureAsset::LoadAsync(U"BackGroundMountainBack");
			TextureAsset::LoadAsync(U"BackGroundMountainMiddle");
			TextureAsset::LoadAsync(U"BackGroundMountainFront");
			TextureAsset::LoadAsync(U"BackGroundSky");
			TextureAsset::LoadAsync(U"Empty");
			TextureAsset::LoadAsync(U"UIBackWithBox");
			TextureAsset::LoadAsync(U"StageNameIn");
			TextureAsset::LoadAsync(U"StageNameOut");
			TextureAsset::LoadAsync(U"Rain");
			TextureAsset::LoadAsync(U"SkyMorning");
			TextureAsset::LoadAsync(U"SkyNoon");
			TextureAsset::LoadAsync(U"SkyAfterNoon");
			TextureAsset::LoadAsync(U"SkyEvening");
			TextureAsset::LoadAsync(U"SkyNight");
			TextureAsset::LoadAsync(U"SkyMidNight");
			TextureAsset::LoadAsync(U"CloudSmall");
			TextureAsset::LoadAsync(U"CloudSmallMorning");
			TextureAsset::LoadAsync(U"CloudSmallRain");
			TextureAsset::LoadAsync(U"CloudNormal");
			TextureAsset::LoadAsync(U"CloudNormalMorning");
			TextureAsset::LoadAsync(U"CloudNormalRain");
			TextureAsset::LoadAsync(U"CloudBig");
			TextureAsset::LoadAsync(U"CloudBigMorning");
			TextureAsset::LoadAsync(U"CloudBigRain");
			TextureAsset::LoadAsync(U"CloudSmallMidNight");
			TextureAsset::LoadAsync(U"CloudSmallRainMidNight");
			TextureAsset::LoadAsync(U"CloudNormalMidNight");
			TextureAsset::LoadAsync(U"UimmNormalForUI");
			TextureAsset::LoadAsync(U"UimmDamageForUI");
			TextureAsset::LoadAsync(U"DamageScreenEffect");
			TextureAsset::LoadAsync(U"LowHpFrame");
			TextureAsset::LoadAsync(U"UimmCrisisForUI");
			TextureAsset::LoadAsync(U"HealMaro");
			TextureAsset::LoadAsync(U"HealEffect");
			TextureAsset::LoadAsync(U"Batu");
			TextureAsset::LoadAsync(U"ClearStill");
			TextureAsset::LoadAsync(U"Trophy");
			TextureAsset::LoadAsync(U"Pause");
			TextureAsset::LoadAsync(U"Config");
			TextureAsset::LoadAsync(U"AudioButton");
			TextureAsset::LoadAsync(U"TutorialMove");
			TextureAsset::LoadAsync(U"TutorialAttack");
			TextureAsset::LoadAsync(U"TutorialMaro1");
			TextureAsset::LoadAsync(U"TutorialMaro2");
			TextureAsset::LoadAsync(U"TutorialScore");
			TextureAsset::LoadAsync(U"TutorialPause");
			TextureAsset::LoadAsync(U"Question");
			TextureAsset::LoadAsync(U"WASD");
			TextureAsset::LoadAsync(U"NM");
			TextureAsset::LoadAsync(U"MouseL");
			TextureAsset::LoadAsync(U"MouseR");
			TextureAsset::LoadAsync(U"Manual");
			TextureAsset::LoadAsync(U"Title");
			TextureAsset::LoadAsync(U"TitleButton");
			TextureAsset::LoadAsync(U"TutorialButton");
			TextureAsset::LoadAsync(U"StartButton");
			TextureAsset::LoadAsync(U"RankingButton");
			TextureAsset::LoadAsync(U"SettingButton");
			TextureAsset::LoadAsync(U"TutorialStr");
			TextureAsset::LoadAsync(U"StartStr");
			TextureAsset::LoadAsync(U"RankingStr");
			TextureAsset::LoadAsync(U"SettingStr");
			TextureAsset::LoadAsync(U"CloudSmallEvening");
			TextureAsset::LoadAsync(U"CloudBigEvening");
			TextureAsset::LoadAsync(U"CloudNormalEvening");
			TextureAsset::LoadAsync(U"CloudBigMidNight");
			TextureAsset::LoadAsync(U"Cloud");
			TextureAsset::LoadAsync(U"GarbageBoxOpen");
			TextureAsset::LoadAsync(U"HpBarDamage");
			TextureAsset::LoadAsync(U"PlayerHpHeal");
			TextureAsset::LoadAsync(U"GarbageBox");
			//フォント
			FontAsset::LoadAsync(U"GameUI_BestTen14");
			FontAsset::LoadAsync(U"GameUI_BestTen");
			FontAsset::LoadAsync(U"GameUI_BestTenDot20");
			FontAsset::LoadAsync(U"GameUI_BestTenDot");
			FontAsset::LoadAsync(U"GameUI_BestTenDot30");
			FontAsset::LoadAsync(U"GameUI_BestTenDot45");
			FontAsset::LoadAsync(U"GameUI_BestTenDot60");
			FontAsset::LoadAsync(U"GameUI_BestTenDot90");
			FontAsset::LoadAsync(U"GameUI_Pixel");
			FontAsset::LoadAsync(U"GameUI_Pixel6");
			
			//アセットの合計を調べる
			for (auto&& [name, info] : AudioAsset::Enumerate())
			{
				assetNum++;
			}
			for (auto&& [name, info] : TextureAsset::Enumerate())
			{
				assetNum++;
			}
			for (auto&& [name, info] : FontAsset::Enumerate())
			{
				assetNum++;
			}
			registerFlag = true;
		}

		bool flag = true;
		isReadyAssetNum = 0;
		for (auto&& [name, info] : AudioAsset::Enumerate())
		{
			if (not AudioAsset::IsReady(name)) {
				//Print << name;
				flag = false;
			}
			else {
				isReadyAssetNum++;
			}
		}
		for (auto&& [name, info] : TextureAsset::Enumerate())
		{
			if (not TextureAsset::IsReady(name)) {
				//Print << name;
				flag = false;
			}
			else {
				isReadyAssetNum++;
			}
		}
		for (auto&& [name, info] : FontAsset::Enumerate())
		{
			if (not FontAsset::IsReady(name)) {
				//Print << name;
				flag = false;
			}
			else {
				isReadyAssetNum++;
			}
		}

		//ロードが終了していたら
		if (flag) {
			//changeScene(State::Title, 3s);
			changeScene(State::StageEditor, 3s);
		}
	}

	void updateFadeIn(double t)override {
		animTimer += Scene::DeltaTime();
	}

	void drawFadeIn(double t)const override {
		RectF(0, 0, Scene::Size()).draw(ColorF(0.0));
		if (Periodic::Sawtooth0_1(2s, animTimer) <= 1.0 / 4.0) {
			FontAsset(U"GameUI_Kei")(U"Now Loading").draw(Arg::bottomLeft = strPos);
		}
		else if (Periodic::Sawtooth0_1(2s, animTimer) <= 2.0 / 4.0) {
			FontAsset(U"GameUI_Kei")(U"Now Loading.").draw(Arg::bottomLeft = strPos);
		}
		else if (Periodic::Sawtooth0_1(2s, animTimer) <= 3.0 / 4.0) {
			FontAsset(U"GameUI_Kei")(U"Now Loading..").draw(Arg::bottomLeft = strPos);
		}
		else {
			FontAsset(U"GameUI_Kei")(U"Now Loading...").draw(Arg::bottomLeft = strPos);
		}

		RectF(Arg::bottomLeft = strPos.movedBy(0, +20), FontAsset(U"GameUI_Kei")(U"Now Loading...").region().w, 9).stretched(4).draw(ColorF(1.0, 0.25));
		RectF(Arg::bottomLeft = strPos.movedBy(0, +20), FontAsset(U"GameUI_Kei")(U"Now Loading...").region().w * ((double)isReadyAssetNum/(double)assetNum), 9).draw(ColorF(0.9));
	}

	void draw()const override {
		Rect(0, 0, Scene::Size()).draw(ColorF(0.0));
		if (Periodic::Sawtooth0_1(2s, animTimer) <= 1.0 / 4.0) {
			FontAsset(U"GameUI_Kei")(U"Now Loading").draw(Arg::bottomLeft = strPos);
		}
		else if (Periodic::Sawtooth0_1(2s, animTimer) <= 2.0 / 4.0) {
			FontAsset(U"GameUI_Kei")(U"Now Loading.").draw(Arg::bottomLeft = strPos);
		}
		else if (Periodic::Sawtooth0_1(2s, animTimer) <= 3.0 / 4.0) {
			FontAsset(U"GameUI_Kei")(U"Now Loading..").draw(Arg::bottomLeft = strPos);
		}
		else {
			FontAsset(U"GameUI_Kei")(U"Now Loading...").draw(Arg::bottomLeft = strPos);
		}
		RectF(Arg::bottomLeft = strPos.movedBy(0, +20), FontAsset(U"GameUI_Kei")(U"Now Loading...").region().w, 9).stretched(4).draw(ColorF(1.0, 0.25));
		RectF(Arg::bottomLeft = strPos.movedBy(0, +20), FontAsset(U"GameUI_Kei")(U"Now Loading...").region().w * ((double)isReadyAssetNum / (double)assetNum), 9).draw(ColorF(0.9));
	}
};
