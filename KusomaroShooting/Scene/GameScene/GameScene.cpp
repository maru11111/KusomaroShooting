#include "stdafx.h"
#include "GameScene.h"
#include "../Util/Common.h"
#include "GameSceneUI.h"

struct DamageScreenEffect : IEffect {
	bool update(double t)override {
		drawSpriteAnimForTimer(U"DamageScreenEffect", 7, 0.040, { 0, 0 }, t, 3, true);
		return currentFrame(7, 0.040, t) != 7;
	}
};

struct DamageEffect : IEffect
{
	Vec2 pos;
	int n=0;
	double timer=0;

	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit DamageEffect(const Vec2 pos_)
		: pos{ pos_ } {}

	bool update(double t) override
	{
		//スプライトシートを再生
		timer += Scene::DeltaTime();
		if (0.085 <= timer) {
			n++;
			timer -= 0.085;
		}
		TextureAsset(U"DamageEffect")(n * TextureAsset(U"DamageEffect").size().x / 6.0, 0, TextureAsset(U"DamageEffect").size().x / 6.0, TextureAsset(U"DamageEffect").size().y).scaled(3).drawAt(pos);

		//
		// << pos;

		//trueの間継続
		return (n < 5);
	}
};


GameScene::GameScene(const InitData& init)
	: IScene{ init }
{
	// UIマネージャー生成。自身のポインタを渡す。（この辺も後でどうにかしたい...）
	gameUI = new GameSceneUI(this);

	objects.player = std::make_unique<Player>(objects);

	if (getData().isTutorial) {
		gameState = GameState::Tutorial;
		//currentBGMName = U"Tutorial";
		AudioManager::Instance()->play(U"Tutorial", 2.5s);
	}
	else {
		changeStage(getData().startStage);

		//スコア
		//currentScore = getData().lastContinueScore;
		scoreAnimTimer = 1;
	}

	//Debug
	//ボス
	//objects.enemies << std::make_unique<GarbageBox>(objects, Vec2( Scene::Size().x+100, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6));
	
}

void GameScene::update() {

	//Print << U"GameState" << (int)gameState;
	//Print << kusomaroTexts.size();
	//Debug
	//if (MouseL.down()) {
		//effect.add<KusomaroTextEffect>(Cursor::Pos(), kusomaroTexts[Random(0, (int)kusomaroTexts.size()-1)]);
		//effect.add<BeamTextEffect>(Cursor::Pos(), beamTexts[Random(0, (int)beamTexts.size() - 1)]);
	//}
	//AudioManager::Instance()->play(U"MidBoss");
	//Debug
	//Print << getData().damageCount;
	
	//Debug
	if (Key1.pressed()) currentStage = (Stage)0;
	if (Key2.pressed()) currentStage = (Stage)1;
	if (Key3.pressed()) currentStage = (Stage)2;
	if (Key4.pressed()) currentStage = (Stage)3;
	if (Key5.pressed()) currentStage = (Stage)4;
	if (Key6.pressed()) currentStage = (Stage)5;

	//クリアタイム時間を測る(後でポーズ画面では行わないようにする)
	if(gameState!=GameState::Pause) currentTime += Scene::DeltaTime();

	//Debug
	if (KeyShift.pressed() && Key0.pressed()) {
		AudioManager::Instance()->stopAllAudio();
		changeScene(State::Result, 0s);
	}
	if (KeyShift.pressed() && KeyE.pressed()) {
		AudioManager::Instance()->stopAllAudio();
		changeScene(State::StageEditor, 0s);
	}

	switch (gameState) {
	case GameState::Tutorial:

		switch (tutorialState) {
		case TutorialState::Move:
			// 移動チュートリアル
			//背景を動かす
			getData().backgroundDrawTimer += Scene::DeltaTime();
			//次へ
			if ( KeyRight.down() || KeyD.down() || confirmInput()) {
				AudioManager::Instance()->playOneShot(U"Paper");
				tutorialState = TutorialState::Attack;
			}
			break;

		case TutorialState::Attack:
			//// 攻撃チュートリアル
			//背景を動かす
			getData().backgroundDrawTimer += Scene::DeltaTime();
			//戻る
			if (KeyLeft.down() || KeyA.down()) {
				tutorialState = TutorialState::Move;
				AudioManager::Instance()->playOneShot(U"Paper");
			}
			//次へ
			if (KeyRight.down() || KeyD.down() || confirmInput()) {
				AudioManager::Instance()->playOneShot(U"Paper");
				tutorialState = TutorialState::Maro1;
			}
			break;

		case TutorialState::Maro1:
			// マシュマロの説明１
			//背景を動かす
			getData().backgroundDrawTimer += Scene::DeltaTime();
			//戻る
			if (KeyLeft.down() || KeyA.down()) {
				tutorialState = TutorialState::Attack;
				AudioManager::Instance()->playOneShot(U"Paper");
			}
			//次へ
			if (KeyRight.down() || KeyD.down() || confirmInput()) {
				AudioManager::Instance()->playOneShot(U"Paper");
				tutorialState = TutorialState::Maro2;
			}
			break;

		case TutorialState::Maro2:
			// マシュマロの説明２
			//背景を動かす
			getData().backgroundDrawTimer += Scene::DeltaTime();
			//戻る
			if (KeyLeft.down() || KeyA.down()) {
				tutorialState = TutorialState::Maro1;
				AudioManager::Instance()->playOneShot(U"Paper");
			}
			//次へ
			if (KeyRight.down() || KeyD.down() || confirmInput()) {
				AudioManager::Instance()->playOneShot(U"Paper");
				tutorialState = TutorialState::Score;
			}
			break;

		case TutorialState::Score:
			// スコア(ランキング)の説明
			//背景を動かす
			getData().backgroundDrawTimer += Scene::DeltaTime();
			//戻る
			if (KeyLeft.down() || KeyA.down()) {
				tutorialState = TutorialState::Maro2;
				AudioManager::Instance()->playOneShot(U"Paper");
			}
			//次へ
			if (KeyRight.down() || KeyD.down() || confirmInput()) {
				AudioManager::Instance()->playOneShot(U"Paper");
				tutorialState = TutorialState::Pause;
			}
			break;

		case TutorialState::Pause:
			//ポーズの説明
			//背景を動かす
			getData().backgroundDrawTimer += Scene::DeltaTime();
			//戻る
			if (KeyLeft.down() || KeyA.down()) {
				tutorialState = TutorialState::Score;
				AudioManager::Instance()->playOneShot(U"Paper");
			}
			//次へ
			if (KeyRight.down() || KeyD.down() || confirmInput()) {
				AudioManager::Instance()->playOneShot(U"Paper");
				tutorialState = TutorialState::Try;
			}
			break;

		case TutorialState::Try:
			//お試し部屋
			//ポーズ入力受付
			if (KeyShift.down() && not objects.player->getIsBeamAttacking()) {
				prevGameState = gameState;
				gameState = GameState::Pause;
				AudioManager::Instance()->pauseAllAudio();
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}

			//説明書を開く
			if (Rect(Scene::Size().x - 10 - TextureAsset(U"Question").size().x * 5, 0 + 10 + TextureAsset(U"UIBack").size().y * 6, TextureAsset(U"Question").size() * 5).leftClicked()) {
				tutorialState = TutorialState::Move;
				AudioManager::Instance()->playOneShot(U"Select");
				break;
			}

			//更新
			updateWithHitStop();
			
			//敵を出す
			if (not objects.player->getIsBeamAttacking()) {
				spawnTimer += Scene::DeltaTime();
			}
			if (5.0 <= spawnTimer) {
				objects.enemies << std::make_unique<GarbageBagNormal>(objects, Vec2(Scene::Size().x+10, Random(TextureAsset(U"UIBack").size().y*6+50, Scene::Size().y-50)));
				spawnTimer -= 5.0;
			}
			break;
		}
		break;

	case GameState::StageStart:
		{
			//描画に使うタイマー
		    if (not isFadingIn) getData().backgroundDrawTimer += Scene::DeltaTime();

			stageStartTimer += Scene::DeltaTime();

			//初めのスコアを保存
			stageStartScore = currentScore;

			switch (stageStartState) {
			case StageStartState::Start:
				{
				//Print << U"backGroundOpacity" << backGroundOpacity;
					stageStartEaseTimer += Scene::DeltaTime();
					if (currentFrame(7, 0.080, stageStartAnimTimer) != 7) stageStartAnimTimer += Scene::DeltaTime();
					const double stageStartEase = Min(EaseOutExpo(stageStartEaseTimer / 1.5), 1.0);
					stageNameTextPos = stageNameTextStartPos.lerp(stageNameTextMiddlePos, stageStartEase);
					//背景切替演出
					//空切替
					if (currentStage != Stage::Morning) backGroundOpacity = Math::Lerp(1.0, 0.0, stageStartTimer / 1.5);
					//雨切替
					if (currentStage == Stage::AfterNoon) rainOpacity = Math::Lerp(0.0, 1.0, stageStartTimer / 1.5);
					if (currentStage == Stage::Evening) rainOpacity = Math::Lerp(1.0, 0.0, stageStartTimer / 1.5);
					//次に進む
					if (1.5 <= stageStartEaseTimer) {
						stageStartState = StageStartState::Middle;
						stageStartTimer = 0;
						stageStartEaseTimer = 0;
					}
					break;
				}

			case StageStartState::Middle:
				{
					//次に進む
					if (0.25 <= stageStartTimer) {
						stageStartState = StageStartState::End;
						stageStartAnimTimer = 0;
						stageStartTimer = 0;
					}
					break;
				}

			case StageStartState::End:
				{
					stageStartEaseTimer += Scene::DeltaTime();
					if (currentFrame(6, 0.100, stageStartAnimTimer) != 6) stageStartAnimTimer += Scene::DeltaTime();
					const double stageStartEase = Min(EaseInQuart(stageStartEaseTimer * 2.0), 1.0);
					stageNameTextPos = stageNameTextMiddlePos.lerp(stageNameTextEndPos, stageStartEase);
					//プレイヤーが登場
					if(currentStage == getData().startStage) objects.player->toStartPos();
					//次に進む
					if (1.0 <= stageStartTimer) {
						stageStartState = StageStartState::Start;
						stageStartTimer = 0;
						stageStartEaseTimer = 0;
						stageStartAnimTimer = 0;
						gameState = GameState::Stage;
					}
					break;
				}
			}
		}
		break;

	case GameState::Stage:

		if (objects.player->getHp() != 0) {
			//ポーズ入力受付
			if (KeyShift.down() && not objects.player->getIsBeamAttacking()) {
				prevGameState = gameState;
				gameState = GameState::Pause;
				AudioManager::Instance()->pauseAllAudio();
				AudioManager::Instance()->playOneShot(U"Select");
			}
		}

		switch (currentStage) {
		case Stage::Morning:

			//空になったら次のステージ
			if (spawnEnemyData.empty() && objects.enemies.empty()) {
				changeStage(Stage::Noon);
				
			}
			break;

		case Stage::Noon:
			//空になったら次のステージ
			if (spawnEnemyData.empty() && objects.enemies.empty()) {

				changeStage(Stage::AfterNoon);
			}
			break;

		case Stage::AfterNoon:
			//空になったら次のステージ
			if (spawnEnemyData.empty() && objects.enemies.empty()) {
				changeStage(Stage::Evening);
			}
			break;

		case Stage::Evening:
			//空になったら次のステージ
			if (spawnEnemyData.empty() && objects.enemies.empty() ) {
				changeStage(Stage::Night);
			}
			break;

		case Stage::Night:
			//空になったら次のステージ
			if (spawnEnemyData.empty() && objects.enemies.empty()) {
				changeStage(Stage::MidNight);
			}
			break;

		case Stage::MidNight:
			break;

		case Stage::Editor:
			break;
		}

		//ヒットストップありupdate
		updateWithHitStop();

		break;

	case GameState::BossAppear:
		//タイマー更新
		gameStateTimer += Scene::DeltaTime();
		easeTimer1 += Scene::DeltaTime();
		easeTimer2 += Scene::DeltaTime();
		easeTimer3 += Scene::DeltaTime();
		//背景をゆっくり動かす
		getData().backgroundDrawTimer += Scene::DeltaTime()/2.0;

		if (not isSpawnBoss) {
			//ボス追加
			objects.enemies << std::make_unique<GarbageBox>(objects, Vec2(Scene::Size().x + 100, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6));
			isSpawnBoss = true;
			//ボス保存
			for (int i = 0; i < objects.enemies.size(); i++) {
				if (objects.enemies[i]->getId() == -1) {
					bossPtr = static_cast<GarbageBox*>(objects.enemies[i].get());
				}
			}
		}

		//Print << U"bossAppearState:" << (int)bossAppearState;
		//Print << U"gaemeStateTimer:" << gameStateTimer;

		//BossAppearのステート
		switch (bossAppearState) {
		case BossAppearState::ChangeBackGround:

			//背景を徐々に出す
			if (currentStage != Stage::Morning) backGroundOpacity = Math::Lerp(1.0, 0.0, gameStateTimer / (1.0 / 1.5));

			//風が吹く
			if(not AudioAsset(U"Wind").isPlaying())AudioManager::Instance()->play(U"Wind", 2.0s);

			//次に移る
			if (4.0 <= gameStateTimer) {
				//タイマーリセット
				gameStateTimer = 0;
				easeTimer1 = 0;
				easeTimer2 = 0;
				//ボス出現演出のステートを次に進める
				bossAppearState = BossAppearState::HideUI;
			}
			break;

		case BossAppearState::HideUI:
			
			if (gameStateTimer <= 1.0/1.5) {
				//UIを隠す
				easeBossAppear = EaseOutQuart(Min(easeTimer1*1.0, 1.0));
				//プレイヤーの位置を基準位置に
				objects.player->bossAppearStateUpdate(easeTimer2);
			}
			else {
				//タイマーリセット
				gameStateTimer = 0;
				easeTimer3 = 0;
				//ボス出現演出のステートを次に進める
				bossAppearState = BossAppearState::DrawRect;
			}
			break;

		case BossAppearState::DrawRect:
			//風がやむ
			if(2.0 <= gameStateTimer)AudioManager::Instance()->stop(U"Wind", 0.25s);

			if (gameStateTimer <= 4.0) {
				//プレイヤーの位置を基準位置に
				objects.player->bossAppearStateUpdate(easeTimer2);
				//枠を出す
				ease = Min(EaseOutQuint(gameStateTimer / 2.0), 1.0);
			}
			else {
				//タイマーリセット
				easeTimer1 = 0;
				easeTimer3 = 0;
				//ボス出現演出のステートを次に進める
				bossAppearState = BossAppearState::AppearBoss;
			}
			break;

		case BossAppearState::AppearBoss:
			easeTimer4 += Scene::DeltaTime();
			
			////
			//if (gameStateTimer <= 1.0) {
			//	//プレイヤーの位置を基準位置に
			//	objects.player->bossAppearStateUpdate(easeTimer2);
			//	//枠を出す
			//	//ease = EaseOutQuint(gameStateTimer / 3.0);
			//}
			
			//敵だけ更新
			for (int i = 0; i < objects.enemies.size(); i++) {
				objects.enemies[i]->update();
			}

			//少しズームイン
			if (easeTimer4 <= 1.0) {;
				double ease4 = EaseOutExpo(Min(easeTimer4 / 0.08, 1.0));
				camera.setTargetScale(Math::Lerp(1, 1.04, ease4));
			}


			//溜めながら少しズームアウト
			if (1.9 <= easeTimer4 && easeTimer4 <= 3.5) {
				easeTimer5 += Scene::DeltaTime();
				double ease4 = EaseInLinear(Min(easeTimer5/1.0, 1.0));
				camera.setTargetScale(Math::Lerp(1.04, 1.0, ease4));
			}
				
			//Pullが終わったらボスに向けてカメラを拡大
			if (3.55 <= easeTimer4) {
				double ease3 = EaseOutExpo(Min(easeTimer3 / 0.1, 1.0));
				camera.setTargetCenter(Math::Lerp(Scene::Center() + Vec2{0,-1}, Vec2(Scene::Size().x - 480, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6-50), ease3));
				camera.setTargetScale(Math::Lerp(1, 1.2, ease3));
			}
			else {
				easeTimer3 = 0;
			}

			//カメラ、枠を戻す
			if (4.5 <= easeTimer4) {
				easeTimer6 += Scene::DeltaTime();
				double ease6 = EaseOutBack(Min(easeTimer6 / 0.15, 1.0));
				camera.setTargetCenter(Math::Lerp(Vec2(Scene::Size().x - 480, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6-50), Scene::Center() + Vec2{ 0,-1 }, ease6));
				camera.setTargetScale(Math::Lerp(1.2, 1.0, ease6));
				//枠を戻す
				ease = 1 - EaseOutExpo(Min(easeTimer6 / 1.0, 1.0));
			}
			//UIを戻す
			if (4.8 <= easeTimer4) {
				//bgmを再生
				//currentBGMName = U"MidBoss";
				if(not AudioAsset(U"MidBoss").isPlaying() ) AudioManager::Instance()->play(U"MidBoss", 1s);
				isHpAnimationStart = true;
				easeTimer7 += Scene::DeltaTime();
				easeBossAppear = 1.0 - EaseOutExpo(Min(easeTimer7 / 1.2, 1.0));
			}

			if (5.8 <= easeTimer4) {
				//ゲームステートを変更
				nextState = GameState::BossBattle;
				isChangeGameState = true;
			}

			//ボスのhpバーupdate
			if (bossPtr != nullptr) {
				if (isHpAnimationStart && not isHpAnimationEnd) {
					if (not isPlayHpAnimation) {
						AudioManager::Instance()->play(U"BossHpAnimation");
						isPlayHpAnimation = true;
					}
					easeBossHpAnimationTimer += Scene::DeltaTime();
				}
			}


			//2Dカメラ更新
			camera.update();

			break;
		}
		break;

	case GameState::BossBattle:
		//ボス討伐前
		if (bossPtr->getCurrentHp() != 0) {

			//ポーズ入力受付
			if (KeyShift.down() && not objects.player->getIsBeamAttacking()) {
				prevGameState = gameState;
				gameState = GameState::Pause;
				AudioManager::Instance()->pauseAllAudio();
				AudioManager::Instance()->playOneShot(U"Select");
			}

			//ヒットストップありupdate
			updateWithHitStop();
			//ボスのhpバーupdate
			if (bossPtr != nullptr) {
				if (isHpAnimationStart && not isHpAnimationEnd) {
					if (not isPlayHpAnimation) {
						AudioManager::Instance()->playOneShot(U"BossHpAnimation");
						isPlayHpAnimation = true;
					}
					easeBossHpAnimationTimer += Scene::DeltaTime();
				}
			}
		}
		//ボス討伐後
		else {
			//初期化
			if (not isInitDefeatBossState) {
				hitStopTimer = 0;
				slowTimer = 0;
				isInitDefeatBossState = true;
				AudioManager::Instance()->stop(AudioManager::Instance()->currentBGMName, 1s);
			}
			//
			switch (defeatBossState) {
			case DefeatBossState::Slow:
				hitStopTimer += Scene::DeltaTime();
				slowTimer += Scene::DeltaTime();
				//ヒットストップ
				if (hitStopTimer <= dyingHitStopTime) {
					//スローにする
					if ((dyingSlowInterval / Max(3.0 * Min(EaseInCubic(hitStopTimer / 2.5), 1.0), 1.0)) <= slowTimer) {
						bossFallingUpdate();
						slowTimer -= dyingSlowInterval / Max(3.0 * Min(EaseInCubic(hitStopTimer / 2.5), 1.0), 1.0);
					}
				}
				//ヒットストップ終了後
				else {
					defeatBossState = DefeatBossState::ToMountain;
				}
				break;

			case DefeatBossState::ToMountain:
				backGroundSpeedEase = std::lerp(1.0, 6.0, Min(defeatBossStateTimer/4.0, 1.0));
				defeatBossStateTimer += Scene::DeltaTime();
				cityTimer += Scene::DeltaTime()/4.0 * backGroundSpeedEase;
				toMountainUpdate();
				if (Scene::Size().x+29 <= objects.player->getPos().x) {
					//スコアを計算
					getData().lastClearScore = currentScore + 10000/(double)getData().damageCount;
					//フラグリセット
					getData().startFromTitle = false;
					//開始ステージリセット
					getData().startStage = Stage::Morning;
					//ダメージを受けた回数リセット
					getData().damageCount = 0;
					changeScene(State::Result, 1.0s);
				}
				break;
			}
		}
		break;

	case GameState::Pause:
		//三角形アニメーション用タイマー
		pauseTriangleTimer += Scene::DeltaTime();
		if (1.25 <= pauseTriangleTimer) {
			pauseTriangleTimer = 0;
		}

		//ボタン
		switch (pauseState) {
		case PauseState::GoBack:
			//ポーズ解除
			if (KeyShift.down()) {
				gameState = prevGameState;
				AudioManager::Instance()->playAllPauseAudio(0s);
				AudioManager::Instance()->playOneShot(U"Cancel");
				break;
			}

			if (KeyRight.down() || KeyD.down()) {
				pauseTriangleTimer = 0;
				pauseState = PauseState::Retry;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
			}
			if (confirmInput()) {
				gameState = prevGameState;
				AudioManager::Instance()->playAllPauseAudio(0s);
				AudioManager::Instance()->playOneShot(U"Cancel");
			}
			break;

		case PauseState::Retry:
			//ポーズ解除
			if (KeyShift.down()) {
				gameState = prevGameState;
				AudioManager::Instance()->playAllPauseAudio(0s);
				AudioManager::Instance()->playOneShot(U"Cancel");
				break;
			}

			if (KeyLeft.down() || KeyA.down()) {
				pauseTriangleTimer = 0;
				pauseState = PauseState::GoBack;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
			}
			if (KeyRight.down() || KeyD.down()) {
				pauseTriangleTimer = 0;
				pauseState = PauseState::Title;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
			}
			if (confirmInput()) {
				getData().startStage = Stage::Morning;
				SpawnEnemyData::spawnTimer = 0;
				//getData().lastContinueScore = 0;
				//getData().isNoContinueClear = true;
				AudioManager::Instance()->stopAllAudio();
				AudioManager::Instance()->play(U"Select");
				getData().startFromTitle = false;
				getData().damageCount = 0;
				changeScene(State::Game, 1.0s);
			}
			break;

		case PauseState::Title:
			//ポーズ解除
			if (KeyShift.down()) {
				gameState = prevGameState;
				AudioManager::Instance()->playAllPauseAudio(0s);
				AudioManager::Instance()->playOneShot(U"Cancel");
				break;
			}

			if (KeyLeft.down() || KeyA.down()) {
				pauseTriangleTimer = 0;
				pauseState = PauseState::Retry;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
			}
			if (KeyRight.down() || KeyD.down()) {
				pauseTriangleTimer = 0;
				pauseState = PauseState::Config;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
			}

			if (confirmInput()) {
				getData().startStage = Stage::Morning;
				SpawnEnemyData::spawnTimer = 0;
				//getData().lastContinueScore = 0;
				//getData().isNoContinueClear = true;
				AudioAsset(AudioManager::Instance()->currentBGMName).stop();
				AudioManager::Instance()->play(U"Select");
				getData().isTutorial = false;
				getData().startFromTitle = false;
				getData().damageCount = 0;
				changeScene(State::Title, 1.0s);

			}
			break;

		case PauseState::Config:
			//ポーズ解除
			if (KeyShift.down()) {
				gameState = prevGameState;
				AudioManager::Instance()->playAllPauseAudio(0s);
				AudioManager::Instance()->playOneShot(U"Cancel");
				break;
			}

			if (KeyLeft.down() || KeyA.down()) {
				pauseTriangleTimer = 0;
				pauseState = PauseState::Title;
				AudioManager::Instance()->playOneShot(U"ChangeButton");
			}

			if (confirmInput()) {
				pauseState = PauseState::ConfigMode;
				AudioManager::Instance()->playOneShot(U"Select");
			}
			break;

		case PauseState::ConfigMode:
			//前の画面に戻る
			if ( (KeyShift.down() ) && not isSetting) {
				pauseState = PauseState::Config;
				AudioManager::Instance()->playOneShot(U"Cancel");
				break;
			}
			//更新
			configUpdate();
			break;
		}
		break;
	}

	//ゲームステートを変更
	if (isChangeGameState) {
		//フラグを元に戻す
		isChangeGameState = false;
		//タイマーリセット
		gameStateTimer = 0;
		easeTimer1 = 0;
		easeTimer2 = 0;
		easeTimer3 = 0;
		easeTimer4 = 0;
		easeTimer5 = 0;
		easeTimer6 = 0;
		easeTimer7 = 0;
		//ゲームステート変更
		if (gameState == nextState) throw Error(U"equal state");
		gameState = nextState;
	}

	//フェードイン中のフラグをfalseに
	isFadingIn = false;
}

void GameScene::bossFallingUpdate() {
	//背景を動かす
	getData().backgroundDrawTimer += Scene::DeltaTime()/4.0;
	//更新
	objects.player->defeatBossUpdate();
	for (int i = 0; i < objects.marshmallows.size(); i++) {
		objects.marshmallows[i]->update();
	}
	for (int i = 0; i < objects.enemies.size(); i++) {
		if(objects.enemies[i]->getId() != -1)objects.enemies[i]->update();
	}
	bossPtr->dyingUpdate();
}

void GameScene::toMountainUpdate() {
	//背景を速く動かす
	getData().backgroundDrawTimer += Scene::DeltaTime() / 4.0 * backGroundSpeedEase;
	//更新
	//山に向かう
	if(frontCityPosX < -Scene::Size().x/2.0 && middleCityPosX < -Scene::Size().x/2.0  && backCityPosX < -Scene::Size().x/2.0 ) objects.player->toMountainUpdate();

	for (int i = 0; i < objects.marshmallows.size(); i++) {
		objects.marshmallows[i]->update();
	}
	for (int i = 0; i < objects.enemies.size(); i++) {
		if (objects.enemies[i]->getId() != -1)objects.enemies[i]->update();
	}
	bossPtr->dyingUpdate();
}

void GameScene::collisionAndRemoveUpdate() {
//当たり判定
//プレイヤーと敵本体
//プレイヤー直接攻撃と敵本体
	if (objects.player->getIsAttackColOn()) {
		for (auto& enemy : objects.enemies) {
			if (enemy->collision().intersects(objects.player->attackCollision())) {
				//ダメージエフェクト
				if (not enemy->isInvincibility()) effect.add<DamageEffect>(enemy->getPos());
				bool isDead = enemy->damage(objects.player->getDamageAmount(), true);
				//スコアを加算
				if(isDead && currentStage!=Stage::MidNight && gameState != GameState::Tutorial) addScore(enemy->score);
			}
		}
	}
	//プレイヤーの攻撃で倒された敵を破壊
	destroyObjects();
	//プレイヤー本体と敵本体
	for (int i = 0; i < objects.enemies.size();i++) {
		if (objects.enemies[i]->collision().intersects(objects.player->bodyCollision())) {
			//ダメージ時画面エフェクト
			if (not objects.player->isInvincibility() && objects.enemies[i]->name != U"HealUmbrella")effect.add<DamageScreenEffect>();
			//ダメージ
			if (gameState != GameState::Tutorial && objects.enemies[i]->name != U"HealUmbrella") {
				//ダメージを受けた回数をカウント
				if (not objects.player->isInvincibility())getData().damageCount++;
				objects.player->damage(objects.enemies[i]->getDamageAmount(), false);
			}
			//回復
			else if (objects.enemies[i]->name == U"HealUmbrella") {
				objects.player->heal(1);
				//削除
				objects.enemies.erase(objects.enemies.begin() + i);
			}
			//チュートリアル
			else if(gameState == GameState::Tutorial) {
				//チュートリアル中は死なない
				if(objects.player->getHp()- objects.enemies[i]->getDamageAmount() <= 0) objects.player->damage(0, false);
				else objects.player->damage(objects.enemies[i]->getDamageAmount(), false);
			}
		}
	}

	//マシュマロと敵
	for (auto& enemy : objects.enemies) {
		for (auto& maro : objects.marshmallows) {
			if (enemy->collision().intersects(maro->collision())) {
				//敵にヒットした判定をtrueに
				if (enemy->name != U"HealUmbrella")
					maro->setIsHit(true);
				//敵にダメージを与えられるか判定
				if (maro->isNotEnemyHit(enemy->getId())) {
					//IDを受け取る
					maro->addId(enemy->getId());
					//敵にダメージを与える
					bool isDead;
					if (bossPtr != nullptr && maro->getType() == MaroType::Empty && enemy.get() == bossPtr) {
						//ビームはボスに大ダメ―ジ
						isDead = enemy->damage(maro->getDamageAmount()*4, false);
					}
					else {
						isDead = enemy->damage(maro->getDamageAmount(), false);
					}
					//スコアを加算
					if (enemy->name == U"HealUmbrella") {
						//攻撃できない
					}
					else if (isDead && currentStage != Stage::MidNight && gameState!=GameState::Tutorial) addScore(enemy->score);
					//エフェクトを追加
					if (enemy->name == U"HealUmbrella") {
						//攻撃できない
					}
					else if (maro->getType() == MaroType::Normal || maro->getType() == MaroType::Empty) {
						effect.add<DamageEffect>(enemy->getPos());
					}
					else if(maro->getType() == MaroType::Beam) {
						effect.add<BeamTextEffect>(maro->getPos(), BaseBullet::beamTexts[Random(0, (int)BaseBullet::beamTexts.size() - 1)]);
					}
					else {
						effect.add<KusomaroTextEffect>(maro->getPos(), BaseBullet::kusomaroTexts[Random(0, (int)BaseBullet::kusomaroTexts.size() - 1)]);
					}
				}
				else {
					//ビームは多段ヒットあり
					bool isDead;
					if (enemy->name == U"HealUmbrella") {
						//攻撃できない
					}
					else if (maro->getType() == MaroType::Empty) {
						//敵にダメージを与える
						if (enemy.get() == bossPtr) {
							//ビームはボスに大ダメ―ジ
							if(maro->getIsHit())
							//ヒットするごとにダメージ増加
							isDead = enemy->damage(maro->getDamageAmount()*4*(maro->hitNum+1), false);
						}
						else {
							isDead = enemy->damage(maro->getDamageAmount(), false);
						}
						//エフェクトを追加
						effect.add<DamageEffect>(enemy->getPos());
						//スコアを加算
						if (isDead && currentStage != Stage::MidNight && gameState != GameState::Tutorial) addScore(enemy->score);
					}
				}
			}
		}
	}

	for (auto& maro : objects.marshmallows) {
		//画面外に行ったクソマロはもう一度boxに
		//→boxに戻さないよう変更
		//if (maro->isOffScreen() && (maro->getType() != MaroType::Normal || maro->getType() != MaroType::Empty)) objects.player->setKusomaro(maro->getType());
		//敵に当たらず画面外に行った時にもクソマロ文章開示
		if (not maro->getIsHit() && maro->isOffScreen()) {
			//エフェクトを追加
			if (maro->getType() == MaroType::Normal) {
				//エフェクトなし
			}
			else if (maro->getType() == MaroType::Beam) {
				effect.add<BeamTextEffect>(maro->getPos(), BaseBullet::beamTexts[Random(0, (int)BaseBullet::beamTexts.size() - 1)]);
			}
			else if (maro->getType() != MaroType::Empty) {
				effect.add<KusomaroTextEffect>(maro->getPos(), BaseBullet::kusomaroTexts[Random(0, (int)BaseBullet::kusomaroTexts.size() - 1)]);
			}


		}
		//Print << U"isOffScreen" << maro->isOffScreen();
	}

	//不要なオブジェクトを破壊
	destroyObjects();
}

void GameScene::updateWithHitStop() {

	//プレイヤー生存中
	if (objects.player->getHp() != 0) {
		//ヒットストップをするか判定
		if (objects.player->getIsHitStopStart() || isHitStopStart) {
			hitStopTimer = 0;
			isHitStopping = true;
			isDamageUIEffectPlaying = true;
			slowTimer = 0;
			isPlayAfterPause = false;
			isHitStopStart = false;
		}

		//ヒットストップ時
		if (isHitStopping) {
			//タイマー
			hitStopTimer += Scene::DeltaTime();
			slowTimer += Scene::DeltaTime();
			//3フレームに一回だけ更新(スローにする)
			if (slowInterval <= slowTimer) {
				stageUpdate();
			}
			//-秒で音声を元に戻す
			if (0.10 <= hitStopTimer && not isPlayAfterPause) {
				AudioManager::Instance()->playAllPauseAudio(0s);
				isPlayAfterPause = true;
			}

			if (hitStopTime <= hitStopTimer) {
				isHitStopping = false;
			}
		}
		//ヒットストップなし
		else {
			stageUpdate();
		}

		//共通処理
		if (isPlayScoreAnim) {
			scoreAnimTimer += Scene::DeltaTime();
			//次のフレームで終了するなら(厳密には違うがおそらくそうなるので無視)
			if (not 1.0 <= scoreAnimTimer && 1.0 <= scoreAnimTimer + Scene::DeltaTime()) AudioManager::Instance()->play(U"AddScoreEnd");
			if (1.0 <= scoreAnimTimer) {
				isPlayScoreAnim = false;
				AudioAsset(U"AddScore").stop();
			}
		}
	}
	//プレイヤ―死亡後
	else {
		if (not isInitDyingVar) {
			hitStopTimer = 0;
			slowTimer = 0;
			isInitDyingVar = true;
			AudioManager::Instance()->stopAllSE();
			AudioManager::Instance()->stop(AudioManager::Instance()->currentBGMName, 0.5s);
			AudioManager::Instance()->play(U"ReceiveDamageDying");
		}

		hitStopTimer += Scene::DeltaTime();
		slowTimer += Scene::DeltaTime();
		//ヒットストップ
		if (hitStopTimer <= dyingHitStopTime) {
			//スローにする
			if (( dyingSlowInterval/Max(3.0*Min(EaseInCubic(hitStopTimer/2.5), 1.0), 1.0) ) <= slowTimer) {
				dyingUpdate();
				slowTimer -= dyingSlowInterval / Max(3.0 * Min(EaseInCubic(hitStopTimer / 2.5), 1.0), 1.0);
			}
		}
		//ヒットストップ終了後
		else {
			if (not isFadingIn) getData().backgroundDrawTimer += Scene::DeltaTime() / 4.0;
			objects.player->dyingUpdate();
			for (int i = 0; i < objects.marshmallows.size(); i++) {
				objects.marshmallows[i]->update();
			}
			for (int i = 0; i < objects.enemies.size(); i++) {
				objects.enemies[i]->update();
			}
		}
		//ボタン出現後
		if (1.5 <= hitStopTimer) {
			gameOverTimer += Scene::DeltaTime();
		
			switch (selectedButton) {
			case SelectedButton::ReStart:
				//決定
				if (confirmInput()) {
					getData().startStage = Stage::Morning;
					SpawnEnemyData::spawnTimer = 0;
					//getData().lastContinueScore = 0;
					//getData().isNoContinueClear = true;
					AudioAsset(AudioManager::Instance()->currentBGMName).stop();
					AudioManager::Instance()->play(U"Select");
					getData().startFromTitle = false;
					getData().damageCount = 0;
					changeScene(State::Game, 1.0s);
				}

				//移動
				if (KeyRight.down() || KeyD.down()) {
					selectedButton = SelectedButton::Continue;
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}

				break;

			case SelectedButton::Continue:
				//決定
				if (confirmInput()) {
					getData().startStage = currentStage;
					SpawnEnemyData::spawnTimer = 0;
					//getData().lastContinueScore = stageStartScore;
					//getData().isNoContinueClear = false;
					AudioAsset(AudioManager::Instance()->currentBGMName).stop();
					AudioManager::Instance()->play(U"Select");
					getData().startFromTitle = false;
					changeScene(State::Game, 1.0s);
				}

				//移動
				if (KeyRight.down() || KeyD.down()) {
					selectedButton = SelectedButton::BackToTitle;
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				if (KeyLeft.down() || KeyA.down()) {
					selectedButton = SelectedButton::ReStart;
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				break;

			case SelectedButton::BackToTitle:
				//決定
				if (confirmInput()) {
					SpawnEnemyData::spawnTimer = 0;
					AudioAsset(AudioManager::Instance()->currentBGMName).stop();
					getData().startFromTitle = false;
					AudioManager::Instance()->play(U"Select");
					getData().damageCount = 0;
					changeScene(State::Title, 1.0s);
				}

				//移動
				if (KeyLeft.down() || KeyA.down()) {
					selectedButton = SelectedButton::Continue;
					AudioManager::Instance()->playOneShot(U"ChangeButton");
				}
				break;
			}

		}
	}
}

void GameScene::stageUpdate() {
	//ダメージ時のUIエフェクト用タイマー
	if (isDamageUIEffectPlaying) {
		damageUIEffectTimer += Scene::DeltaTime();
		if (1.0 <= damageUIEffectTimer) {
			damageUIEffectTimer = 0;
			isDamageUIEffectPlaying = false;
		}
	}

	//ビーム中なら
	if (objects.player->getIsBeamAttacking()) {
		isEnemyTimeStopped = true;
	}
	else {
		isEnemyTimeStopped = false;
	}

	//プレイヤーとビーム以外は停止
	if (isEnemyTimeStopped) {
		//背景をゆっくり動かす
		getData().backgroundDrawTimer += Scene::DeltaTime()/2.0;
		//敵以外を更新
		objects.player->update();
		for (int i = 0; i < objects.marshmallows.size();i++) {
			if(objects.marshmallows[i]->getType() == MaroType::Empty) objects.marshmallows[i]->update();
		}
	}
	//敵が停止していない
	else {
		//背景を動かす
		if(not isFadingIn) getData().backgroundDrawTimer += Scene::DeltaTime();
		//更新
		objects.player->update();
		for (int i = 0; i < objects.marshmallows.size(); i++) {
			objects.marshmallows[i]->update();
		}
		/*enemy内でenemiesに要素を入れる場合があるので普通のforにする。
		  範囲forの場合だと走査中に要素が入れられたときにnullptrでエラー*/
		for (int i = 0; i < objects.enemies.size(); i++) {
			objects.enemies[i]->update();
		}
		//敵を追加
		spawnEnemy();
	}

	//当たり判定と削除
	collisionAndRemoveUpdate();
}

void GameScene::dyingUpdate() {
	//背景を動かす
	getData().backgroundDrawTimer += Scene::DeltaTime()/4.0;
	//更新
	objects.player->dyingUpdate();
	for (int i = 0; i < objects.marshmallows.size(); i++) {
		objects.marshmallows[i]->update();
	}
	for (int i = 0; i < objects.enemies.size(); i++) {
		objects.enemies[i]->update();
	}
}

void GameScene::spawnEnemy() {
	SpawnEnemyData::spawnTimer += Scene::DeltaTime();

	for (int i = 0; i < spawnEnemyData.size(); i++) {
		if (spawnEnemyData[i].isSpawnable()) {
			switch (spawnEnemyData[i].type) {
			case EnemyType::Empty: /*何もしない*/; break;
			case EnemyType::Bag: objects.enemies << std::make_unique<GarbageBagNormal>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::SineBag: objects.enemies << std::make_unique<GarbageBagSine>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::AccelBag: objects.enemies << std::make_unique<GarbageBagAccel>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::FastBag: objects.enemies << std::make_unique<GarbageBagFast>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::BagWithCan: objects.enemies << std::make_unique<GarbageBagWithCan>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::BagWithCanStay: objects.enemies << std::make_unique<GarbageBagWithCanStay>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::Can: objects.enemies << std::make_unique<Can>(objects, spawnEnemyData[i].pos, Vec2{-1,0}); break;
			case EnemyType::Fish: objects.enemies << std::make_unique<Fish>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::Umbrella: objects.enemies << std::make_unique<Umbrella>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::HealUmbrella: objects.enemies << std::make_unique<HealUmbrella>(objects, spawnEnemyData[i].pos); break;
			}
		}
	}

	//一度出現した敵のデータは削除
	spawnEnemyData.remove_if([](const SpawnEnemyData& spawnEnemyData) {return spawnEnemyData.isSpawnable(); });


	//Debug
	//if (KeyE.down())objects.enemies << std::make_unique<GarbageBagWithCan>(objects, Vec2{ 900, 300 });
	////if (KeyE.down())objects.enemies << std::make_unique<Can>(objects, Vec2{ 900, 300 }, (objects.player->getPos()-Vec2(900,300)).normalized());
	//if (KeyE.down())objects.enemies << std::make_unique<Fish>(objects, Vec2{ 900, 300 });
	//if (KeyE.down())objects.enemies << std::make_unique<Umbrella>(objects, Vec2{ 700, 0 });
	//if (KeyR.down())objects.player->damage(1);
	//if (KeyT.down())isTimeStopped = !isTimeStopped;

	///*if (KeyE.down()) {
	//	for (int i = 0; i < 7; i++) {
	//		const Size SceneSize = { 320 * 3, 214 * 3 };
	//		const double GridSize = (SceneSize.y - TextureAsset(U"UIBack").size().y * 3) / 8.0;
	//		objects.enemies << std::make_unique<GarbageBagWithCan>(objects, Vec2{ 800, TextureAsset(U"UIBack").size().y*6+i*GridSize+GridSize/2.0});
	//	}
	//}*/


	////DebugSpawn
	//int n = (int)(Scene::Time() / 1.0) % 10;
	//Print << n;
	//if (n == 0) {
	//	int n = Random(0, 5);
	//	Vec2 vec = RandomVec2();
	//	if (vec.x < 0)vec.x *= -1;
	//	if (vec.y < 0)vec.y *= -1;
	//	Vec2 pos = RandomVec2(RectF(800, 0 + TextureAsset(U"UIBack").size().y * 6, Scene::Size().x, Scene::Size().y));
	//	switch (n) {
	//	case 0: objects.enemies << std::make_unique<GarbageBagNormal>(objects, pos); break;
	//	case 1: objects.enemies << std::make_unique<GarbageBagFast>(objects, pos); break;
	//	case 2: objects.enemies << std::make_unique<GarbageBagWithCan>(objects, pos); break;
	//	case 3: objects.enemies << std::make_unique<Can>(objects, pos, vec); break;
	//	case 4: objects.enemies << std::make_unique<Fish>(objects, pos); break;
	//	case 5: objects.enemies << std::make_unique<Umbrella>(objects, pos); break;
	//	}
	//}
}

void GameScene::addScore(int score) {
	if (isPlayHpAnimation) prevScore = prevScore + (currentScore - prevScore) * Min(EaseOutCubic(scoreAnimTimer), 1.0);
	else prevScore = currentScore;
	currentScore += score;
	isPlayScoreAnim = true;
	scoreAnimTimer = 0;
	if (not AudioAsset(U"AddScore").isPlaying()) AudioManager::Instance()->play(U"AddScore");
	//Print << U"ADDDDDDDDDDD";
}

void GameScene::destroyObjects() {
	objects.marshmallows.remove_if([](const auto& maro) { return maro->isDestroy(); });
	objects.enemies.remove_if([](const auto& enemy) {return enemy->isDestroy(); });
}

Objects& GameScene::getObj() {
	return objects;
}

void GameScene::loadJson(String path)const {
	JSON json = JSON::Load(path);
	if (not json) throw Error{ path + U"がありません" };
	{
		for (const auto& object : json[U"Enemies"].arrayView())
		{
			SpawnEnemyData data;
			data.time = object[U"Time"].get<int>();
			data.pos = object[U"Pos"].get<Vec2>();
			String str = object[U"Type"].get<String>();
			int n;
			if (str == U"Bag") {
				n = (int)EnemyType::Bag;
			}
			else if (str == U"AccelBag") {
				n = (int)EnemyType::AccelBag;
			}
			else if (str == U"SineBag") {
				n = (int)EnemyType::SineBag;
			}
			else if (str == U"FastBag") {
				n = (int)EnemyType::FastBag;
			}
			else if (str == U"BagWithCan") {
				n = (int)EnemyType::BagWithCan;
			}
			else if (str == U"BagWithCanStay") {
				n = (int)EnemyType::BagWithCanStay;
			}
			else if (str == U"Can") {
				n = (int)EnemyType::Can;
			}
			else if (str == U"Fish") {
				n = (int)EnemyType::Fish;
			}
			else if (str == U"Umbrella") {
				n = (int)EnemyType::Umbrella;
			}
			else if (str == U"HealUmbrella") {
				n = (int)EnemyType::HealUmbrella;
			}
			else {
				throw Error(U"GameSceneForEditor:未定義の敵です");
			}
			data.type = (EnemyType)n;
			spawnEnemyData << data;
		}
	}
}

void GameScene::changeStage(Stage nextStage) {

	//bgmを止める
	if (getData().startStage != nextStage){
		AudioManager::Instance()->stop(AudioManager::Instance()->currentBGMName, 0.5s);
	}
	else {
		changeStageTimer = 4;
	}
	//-秒経ったら次のステージ
	changeStageTimer += Scene::DeltaTime();
	if (2.0 <= changeStageTimer) {
		changeStageTimer = 0;
		//SEを停止
		AudioManager::Instance()->stopAllSE();

		switch (nextStage) {
		case Stage::Morning:
			//ステージをロード
			loadJson(Resource(U"stage/stageMorning.json"));
			//ステージを変更
			currentStage = nextStage;
			//ゲームステートを変更
			gameState = GameState::StageStart;
			//背景の不透明度をリセット
			backGroundOpacity = 1.0;
			//タイマーリセット
			SpawnEnemyData::spawnTimer = 0;
			//bgm
			//currentBGMName = U"Morning";
			if(not AudioAsset(U"Morning").isPlaying()) AudioManager::Instance()->play(U"Morning", 2.5s);
			break;

		case Stage::Noon:
			//ステージをロード
			loadJson(Resource(U"stage/stageNoon.json"));
			//ステージを変更
			currentStage = nextStage;
			//ゲームステートを変更
			gameState = GameState::StageStart;
			//背景の不透明度をリセット
			backGroundOpacity = 1.0;
			//タイマーリセット
			SpawnEnemyData::spawnTimer = 0;
			//bgm
			//currentBGMName = U"Noon";
			AudioManager::Instance()->play(U"Noon", 2.5s);
			break;

		case Stage::AfterNoon:
			//ステージをロード
			loadJson(Resource(U"stage/stageAfterNoon.json"));
			//ステージを変更
			currentStage = nextStage;
			//ゲームステートを変更
			gameState = GameState::StageStart;
			//背景の不透明度をリセット
			backGroundOpacity = 1.0;
			//タイマーリセット
			SpawnEnemyData::spawnTimer = 0;
			//bgm
			//currentBGMName = U"AfterNoon";
			AudioManager::Instance()->play(U"AfterNoon", 2.5s);
			break;

		case Stage::Evening:
			//ステージをロード
			loadJson(Resource(U"stage/stageEvening.json"));
			//ステージを変更
			currentStage = nextStage;
			//ゲームステートを変更
			gameState = GameState::StageStart;
			//背景の不透明度をリセット
			backGroundOpacity = 1.0;
			//タイマーリセット
			SpawnEnemyData::spawnTimer = 0;
			//bgm
			//currentBGMName = U"Evening";
			AudioManager::Instance()->play(U"Evening", 2.5s);
			break;

		case Stage::Night:
			//ステージをロード
			loadJson(Resource(U"stage/stageNight.json"));
			//ステージを変更
			currentStage = nextStage;
			//ゲームステートを変更
			gameState = GameState::StageStart;
			//背景の不透明度をリセット
			backGroundOpacity = 1.0;
			//タイマーリセット
			SpawnEnemyData::spawnTimer = 0;
			//bgm
			//currentBGMName = U"Night";
			AudioManager::Instance()->play(U"Night", 2.5s);
			break;

		case Stage::MidNight:
			//ステージを変更
			currentStage = nextStage;
			//ゲームステートを変更
			gameState = GameState::BossAppear;
			//背景の不透明度をリセット
			backGroundOpacity = 1.0;
			break;
		}
	}
}

void GameScene::drawBackground()const {
	//背景
//Scene::Rect()
//	.draw(Arg::top = ColorF{ 0.2, 0.5, 1.0 }, Arg::bottom = ColorF{ 0.5, 0.8, 1.0 });
//const double skyPosX = -(int)(getData().backgroundDrawTimer / 0.0410) % (320 * 3);
	const double cloudBigPosX = -(int)(getData().backgroundDrawTimer / 0.0100) % (320 * 3);
	const double cloudNormalPosX = -(int)(getData().backgroundDrawTimer / 0.0125) % (320 * 3);
	const double cloudSmallPosX = -(int)(getData().backgroundDrawTimer / 0.0150) % (320 * 3);
	const double farBackMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0250) % (320 * 3);
	const double backMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0300) % (320 * 3);
	const double middleMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0175) % (320 * 3);
	const double frontMountainPosX = -(int)(getData().backgroundDrawTimer / 0.0125) % (320 * 3);

	if (defeatBossState == DefeatBossState::ToMountain) {
		//cityは0から始まる前提
		frontCityPosX = prevFrontCityPosX - (int)(cityTimer / 0.004125);
		middleCityPosX = prevMiddleCityPosX - (int)(cityTimer / 0.003125);
		backCityPosX = prevBackCityPosX - (int)(cityTimer / 0.002125);
	}
	else {
		frontCityPosX = -(int)(getData().backgroundDrawTimer / 0.004125) % (320 * 3);
		middleCityPosX = -(int)(getData().backgroundDrawTimer / 0.003125) % (320 * 3);
		backCityPosX = -(int)(getData().backgroundDrawTimer / 0.002125) % (320 * 3);

		prevFrontCityPosX = frontCityPosX;
		prevMiddleCityPosX = middleCityPosX;
		prevBackCityPosX = backCityPosX;
	}

	const Vec2 rainPos = { -(int)(getData().backgroundDrawTimer / 0.00200) % (320 * 3), (int)(getData().backgroundDrawTimer / 0.00200) % (214 * 3) };

	//空
	switch (currentStage) {
	case Stage::Morning:
		TextureAsset(U"SkyMorning").scaled(3).draw(0, 0);
		break;

	case Stage::Noon:
		TextureAsset(U"SkyNoon").scaled(3).draw(0, 0);
		TextureAsset(U"SkyMorning").scaled(3).draw(0, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::AfterNoon:
		TextureAsset(U"SkyAfterNoon").scaled(3).draw(0, 0);
		TextureAsset(U"SkyNoon").scaled(3).draw(0, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::Evening:
		TextureAsset(U"SkyEvening").scaled(3).draw(0, 0);
		TextureAsset(U"SkyAfterNoon").scaled(3).draw(0, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::Night:
		TextureAsset(U"SkyNight").scaled(3).draw(0, 0);
		TextureAsset(U"SkyEvening").scaled(3).draw(0, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::MidNight:
		TextureAsset(U"SkyMidNight").scaled(3).draw(0, 0);
		TextureAsset(U"SkyNight").scaled(3).draw(0, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::Editor:
		TextureAsset(U"SkyEvening").scaled(3).draw(0, 0);
		TextureAsset(U"SkyAfterNoon").scaled(3).draw(0, 0, ColorF(1.0, backGroundOpacity));
	}
	//TextureAsset(U"BackGroundSky").scaled(3).draw(Scene::Size().x + skyPosX, 0);
	//TextureAsset(U"BackGroundMountain").scaled(3).draw(farBackMountainPosX, 0);
	//TextureAsset(U"BackGroundMountain").scaled(3).draw(Scene::Size().x + farBackMountainPosX, 0);
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

	//雲
	switch (currentStage) {
	case Stage::Morning:
		TextureAsset(U"CloudSmallMorning").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmallMorning").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormalMorning").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormalMorning").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBigMorning").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBigMorning").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		break;

	case Stage::Noon:
		TextureAsset(U"CloudSmall").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmall").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormal").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormal").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBig").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBig").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		TextureAsset(U"CloudSmallMorning").scaled(3).draw(cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudSmallMorning").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalMorning").scaled(3).draw(cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalMorning").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigMorning").scaled(3).draw(cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigMorning").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::AfterNoon:
		TextureAsset(U"CloudSmallRain").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmallRain").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormalRain").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormalRain").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBigRain").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBigRain").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		TextureAsset(U"CloudSmall").scaled(3).draw(cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudSmall").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormal").scaled(3).draw(cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormal").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBig").scaled(3).draw(cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBig").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::Evening:
		TextureAsset(U"CloudSmallEvening").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmallEvening").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormalEvening").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormalEvening").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBigEvening").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBigEvening").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		TextureAsset(U"CloudSmallRain").scaled(3).draw(cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudSmallRain").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalRain").scaled(3).draw(cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalRain").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigRain").scaled(3).draw(cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigRain").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::Night:
		TextureAsset(U"CloudSmallRain").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmallRain").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormalRain").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormalRain").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBigRain").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBigRain").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		TextureAsset(U"CloudSmallEvening").scaled(3).draw(cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudSmallEvening").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalEvening").scaled(3).draw(cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalEvening").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigEvening").scaled(3).draw(cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigEvening").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::MidNight:
		TextureAsset(U"CloudSmallMidNight").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmallMidNight").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormalMidNight").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormalMidNight").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBigMidNight").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBigMidNight").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		TextureAsset(U"CloudSmallRain").scaled(3).draw(cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudSmallRain").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalRain").scaled(3).draw(cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudNormalRain").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigRain").scaled(3).draw(cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		TextureAsset(U"CloudBigRain").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0, ColorF(1.0, backGroundOpacity));
		break;

	case Stage::Editor:
		TextureAsset(U"CloudSmallMorning").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmallMorning").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormalMorning").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormalMorning").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBigMorning").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBigMorning").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);

		//TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ 0, 0 }, ColorF(1.0, 0.9));
		//TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ Scene::Size().x, 0 }, ColorF(1.0, 0.9));
		//TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ 0, -Scene::Size().y }, ColorF(1.0, 0.9));
		//TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ Scene::Size().x, -Scene::Size().y }, ColorF(1.0, 0.9));
	}

	if (currentStage == Stage::AfterNoon || currentStage == Stage::Evening) {
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ 0, 0 }, ColorF(1.0, rainOpacity));
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ Scene::Size().x, 0 }, ColorF(1.0, rainOpacity));
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ 0, -Scene::Size().y }, ColorF(1.0, rainOpacity));
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ Scene::Size().x, -Scene::Size().y }, ColorF(1.0, rainOpacity));
	}
}

void GameScene::commonDraw()const {
	{
		const ScopedRenderTarget2D renderTarget{ renderTexture };

		drawBackground();

		//背景とオブジェクトの間
		//後面プレイヤー攻撃エフェクト
		objects.player->drawEffectBack();
		//ビームの描画
		for (auto& maro : objects.marshmallows) {
			maro->backGroundDraw();
		}

		//オブジェクト
		//プレイヤー生存時
		if (objects.player->getHp() != 0) {
			objects.player->draw();
			for (int i = 0; i < objects.marshmallows.size(); i++) {
				objects.marshmallows[i]->draw();
			}

			for (auto& enemy : objects.enemies) {
				enemy->draw();
			}
		}
		//プレイヤー死亡時
		else {
			objects.player->dyingDraw();
			for (int i = 0; i < objects.marshmallows.size(); i++) {
				objects.marshmallows[i]->draw();
			}

			for (auto& enemy : objects.enemies) {
				enemy->draw();
			}
		}

		//エフェクトを再生
		//前面プレイヤー攻撃エフェクト
		objects.player->drawEffectFront();
		//gameSceneのエフェクト
		effect.update();

		//瀕死時画面エフェクト
		if (objects.player->getIsCrisis()) {
			TextureAsset(U"LowHpFrame").scaled(3).draw(ColorF(1.0, 1.0 - Periodic::Sawtooth0_1(0.9135s)));
		}

		//Debug
		//TextureAsset(U"Fish").draw(300,200);
		//スプライトシートを再生
		/*int n = (int)(getData().backgroundDrawTimer / 0.035) % 21;
		TextureAsset(U"UiAttackEffect")(n * TextureAsset(U"UiAttackEffect").size().x / 21.0, 0, TextureAsset(U"UiAttackEffect").size().x / 21.0, TextureAsset(U"UiAttackEffect").size().y).scaled(3).drawAt(objects.player->getPos());*/
		//TextureAsset(U"Umbrella").scaled(3).draw(300,200);

	}
	//文字以外のレンダーテクスチャをdraw
	renderTexture.draw();
}

void GameScene::updateFadeIn(double t) {
	//チュートリアル中でない
	if (not getData().isTutorial) {
		//タイトルから開始している
		if (getData().startFromTitle) {
			fadeInTimer += Scene::DeltaTime();
			fadeInTimerEase += Scene::DeltaTime();
			isFadingIn = true;
			//減速
			if (0.5 <= fadeInTimer) {
				const double ease = EaseInOutQuad(Min((fadeInTimerEase - 0.5) / 3.0, 1.0));
				getData().backgroundDrawTimer += Scene::DeltaTime() * Max(2.5 * (1 - ease), 1.0);
			}
			else {
				getData().backgroundDrawTimer += Scene::DeltaTime() * 2.5;
			}
			//if (fadeInTimer <= 1.0) objects.player->toStartPos();
			update();
		}
		else {
			getData().backgroundDrawTimer += Scene::DeltaTime();
		}
	}
	else {
		getData().backgroundDrawTimer += Scene::DeltaTime();
	}
}

void GameScene::drawFadeIn(double t)const {
	draw();
	if (getData().isTutorial)RectF(0, 0, Scene::Size()).draw(ColorF(0.0, 1.0 - t));
}

void GameScene::draw() const {
	switch (gameState) {
	case GameState::Tutorial:
		// 共通描画
		commonDraw();

		switch (tutorialState) {
		case TutorialState::Move:
			// 移動
			break;

		case TutorialState::Attack:
			// 攻撃方法
			break;

		case TutorialState::Maro1:
			// マシュマロの説明1
			break;

		case TutorialState::Maro2:
			//マシュマロの説明2
			break;

		case TutorialState::Score:
			// スコア(ランキング)の説明
			break;

		case TutorialState::Pause:
			//ポーズ画面の説明
			break;

		case TutorialState::Try:
			//お試し部屋
			break;
		}
		break;

	case GameState::StageStart:
		commonDraw();

		switch (stageStartState) {
		case StageStartState::Start:
			break;

		case StageStartState::Middle:
			break;

		case StageStartState::End:

			break;
		}
		break;

	case GameState::Stage:
		commonDraw();
		break;

	case GameState::BossAppear:
		switch (bossAppearState) {
		case BossAppearState::ChangeBackGround:
			commonDraw();
			break;

		case BossAppearState::HideUI:
			commonDraw();
			break;

		case BossAppearState::DrawRect:
			commonDraw();
			break;

		case BossAppearState::AppearBoss:
		{
			// 2D カメラの設定から Transformer2D を作成
			const auto t = camera.createTransformer();

			//描画
			commonDraw();
		}
		break;
		}
		break;

	case GameState::BossBattle:
		commonDraw();
		break;

	case GameState::Pause:
		commonDraw();

		//背景を暗く
		RectF(0, 0, Scene::Size()).draw(ColorF(0.0, 0.3));

		//ボタン
		switch (pauseState) {
		case PauseState::GoBack:
			break;

		case PauseState::Retry:
			break;

		case PauseState::Title:
			break;

		case PauseState::Config:
			break;

		case PauseState::ConfigMode:
			break;
		}
		break;
	}

	// UIを描画
	gameUI->draw();
}
