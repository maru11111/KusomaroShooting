#include "stdafx.h"
#include "GameScene.h"
#include "Common.h"

/// @brief 最大のアルファ値を書き込むブレンドステートを返します。
BlendState MaxAlphaBlend()
{
	BlendState blend = BlendState::Default2D;
	blend.opAlpha = BlendOp::Max;
	blend.dstAlpha = Blend::DestAlpha;
	blend.srcAlpha = Blend::SrcAlpha;
	return blend;
}

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
	objects.player = std::make_unique<Player>(objects);

	//loadJson(U"stage/test.json");

	//初めのステージをロード
	loadJson(U"stage/stage1.json");

	//Debug
	//ボス
	//objects.enemies << std::make_unique<GarbageBox>(objects, Vec2( Scene::Size().x+100, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6));
}

void GameScene::drawHpBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY, double healEase, double damageEase)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(6).draw(backBarPosX, barPosY);

	//ヒールアニメーションバーのカットpxを計算
	const double healBarDrawStartPosX = frontBar.size().x - frontBar.size().x * healEase;
	//ヒールアニメーションを描画
	TextureAsset(U"PlayerHpHeal")(healBarDrawStartPosX, 0, frontBar.size().x - healBarDrawStartPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12, barPosY);

	//ダメージアニメーションのカットpxを計算
	const double damageBarDrawStartPosX = frontBar.size().x - frontBar.size().x * damageEase;
	//ダメージ時の減少アニメーションを描画
	TextureAsset(U"HpBarDamage")(damageBarDrawStartPosX, 0, frontBar.size().x - damageBarDrawStartPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12, barPosY);

	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawStartPosX = frontBar.size().x - frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(barDrawStartPosX, 0, frontBar.size().x - barDrawStartPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12, barPosY);
}

void GameScene::drawMaroBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY, double healEase)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(6).draw(backBarPosX, barPosY);

	//ヒールアニメーションバーのカットpxを計算
	const double healBarDrawStartPosX = frontBar.size().x - frontBar.size().x * healEase;
	//ヒールアニメーションを描画
	TextureAsset(U"PlayerHpHeal")(healBarDrawStartPosX, 0, frontBar.size().x - healBarDrawStartPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12, barPosY);

	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawStartPosX = frontBar.size().x - frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(barDrawStartPosX, 0, frontBar.size().x - barDrawStartPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12, barPosY);
}

void GameScene::drawBossBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY, BaseBoss *boss)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(6).draw(backBarPosX, barPosY);

	//ダメージアニメーションのカット後pxを計算
	const double damageBarDrawEndPosX = frontBar.size().x * ((double)boss->getPrevHpDamage()/(double)boss->getMaxHp()) * ((double)boss->getPrevHpDamage() - (double)boss->getCurrentHp())/(double)boss->getPrevHpDamage() * boss->getDamageEase();
	//Print << U"bossBar:" << damageBarDrawEndPosX;
	//ダメージ時の減少アニメーションを描画
	TextureAsset(U"HpBarDamage")(0, 0, damageBarDrawEndPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12 + (frontBar.size().x - damageBarDrawEndPosX - frontBar.size().x * ((double)boss->getCurrentHp() / (double)boss->getMaxHp())) * 6, barPosY);

	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawEndPosX = frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(0, 0, barDrawEndPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12 + (frontBar.size().x - barDrawEndPosX) * 6, barPosY);
	//Print << remainingParcentage;
}

void GameScene::drawMarshmallowUI() const {

	//Rect{ 0,0,Scene::Size().x,40 }.draw(ColorF(0.9, 0.3, 0.3));

	//Print << isHpAnimationStart;

	const double damageUIEffectOffsetX = (1.0 - EaseInOutQuart(Min(damageUIEffectTimer, 1.0))) * 2.5 * Sin(damageUIEffectTimer * Math::Pi * 13.0);
	const double damageUIEffectOffsetY = - (1.0 - EaseInOutCirc(Min(damageUIEffectTimer, 1.0))) * 1.5 * Cos(damageUIEffectTimer * Math::Pi * 13.0);

	//UIの背景
	int n = (int)(drawTimer / 0.08) % 65;
	bool flag = false;
	for (int i = 0; i < objects.enemies.size(); i++) {
		if (objects.enemies[i]->getId() == -1) {
			flag = true;
		}
	}
	if (flag==true && isHpAnimationStart) {
		TextureAsset(U"UIBackWithBox")(n * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0-6 + damageUIEffectOffsetX ,-6 -easeBossAppear * marshmallowUIOffset);
	}
	else {
		TextureAsset(U"UIBack")(n * TextureAsset(U"UIBack").size().x / 65, 0, TextureAsset(U"UIBack").size().x / 65.0, TextureAsset(U"UIBack").size().y).scaled(6).draw(-6 + damageUIEffectOffsetX,-6 -easeBossAppear * marshmallowUIOffset);
	}

	//HPバー
	drawHpBar(objects.player->getHpEase(), 1.0, TextureAsset(U"PlayerBarBack"), TextureAsset(U"PlayerHpFront"), 50 * 3 -4 + damageUIEffectOffsetX, 6 * 3 + 8 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY, objects.player->getHpHealEase(), objects.player->getDamageEase());

	//マシュマロバー
	drawMaroBar(objects.player->getMaroEase(), 1.0, TextureAsset(U"PlayerBarBack"), TextureAsset(U"MarshmallowBarFront"), 50 * 3-4  + damageUIEffectOffsetX, 24 * 3 + 8 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY, objects.player->getMaroAddEase());

	//ボスのHPバー
	for (int i = 0; i < objects.enemies.size(); i++) {
		if (objects.enemies[i]->getId() == -1) {
			BaseBoss *bossPtr = static_cast<BaseBoss*>(objects.enemies[i].get());
			if (isHpAnimationStart && not isHpAnimationEnd) {
				if (not isPlayHpAnimation) {
					AudioManager::Instance()->play(U"BossHpAnimation");
					isPlayHpAnimation = true;
				}
				easeBossHpAnimationTimer += Scene::DeltaTime();
				const double easeHpAnimation = Min(EaseInLinear(easeBossHpAnimationTimer/3.0), (double)bossPtr->getCurrentHp() / (double)bossPtr->getMaxHp());
				drawBossBar(easeHpAnimation, 1.0, TextureAsset(U"BossBarBack"), TextureAsset(U"BossBarFront"), (320 - TextureAsset(U"BossBarBack").size().x * 2 - 16) * 3 + damageUIEffectOffsetX, 26 * 3 - 6 - easeBossAppear * marshmallowUIOffset, bossPtr);
			}
		}
	}

	//Print << U"ease*mar" << easeBossAppear * marshmallowUIOffset;
	//次のマシュマロを表示
	TextureAsset(U"MarshmallowBox").scaled(6).drawAt((320 / 2 - 5) * 3 + damageUIEffectOffsetX, 24*3 + 8 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY);

	switch (objects.player->getNextMaro()) {
	case MaroType::Normal: TextureAsset(U"Marshmallow").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3 + damageUIEffectOffsetX, 24*3 + 6 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Up: TextureAsset(U"KusomaroUp").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5 - 4)*3 + damageUIEffectOffsetX, 24 * 3 + 6 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Down: TextureAsset(U"KusomaroDown").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 1)*3 + damageUIEffectOffsetX, 24*3 + 6 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Sine: TextureAsset(U"KusomaroSine").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5 + Math::Cos(drawTimer*Math::Pi*2))*3 - 6 + 6 + damageUIEffectOffsetX, 24*3 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Beam: TextureAsset(U"KusomaroBeam").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3 + damageUIEffectOffsetX, 24*3 + 6 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY); break;
	}


	{
		const ScopedRenderTarget2D renderTarget{ fontRenderTexture };

		//背景を透明に
		fontRenderTexture.clear(ColorF{ 1.0, 0.0 });

		//アルファ値を戻す
		const ScopedRenderStates2D renderState{ MaxAlphaBlend() };

		FontAsset(U"GameUI_Pixel")(U"HP").draw(165-4 + damageUIEffectOffsetX, 24 + 8 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY);

		//Print << U"easeBoss" << marshmallowUIOffset*easeBossAppear;
		//if (objects.player->getNumMarshmallows() < 10) FontAsset(U"GameUI_Pixel")(U"Marshmallow\n    ", objects.player->getNumMarshmallows(), U"/30").drawAt(74, 93);
		/*else*/ FontAsset(U"GameUI_Pixel")(U"MP:", objects.player->getNumMarshmallows(), U"/30").draw(165-4 + damageUIEffectOffsetX, 24 * 3 + 14 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY);

		FontAsset(U"GameUI_Pixel")(U"Next").draw(423 + damageUIEffectOffsetX, 12 + 8 - 6 - easeBossAppear * marshmallowUIOffset + damageUIEffectOffsetY);

		//ステージ名orボスの名前
		switch (gameState) {
		case GameState::Tutorial:break;
			FontAsset(U"GameUI_BestTenDot30")(U"Stage:Tutorial").draw(548 + damageUIEffectOffsetX, 25 - 6 - easeBossAppear * marshmallowUIOffset); break;

		case GameState::StageStart:
		case GameState::Stage:
			FontAsset(U"GameUI_BestTenDot30")(U"Stage:"+ stageName[(int)currentStage]).draw(548 + damageUIEffectOffsetX, 25 - 6 - easeBossAppear * marshmallowUIOffset); break;

		case GameState::BossAppear:
			if (bossAppearState == BossAppearState::AppearBoss) {
				for (int i = 0; i < objects.enemies.size(); i++) {
					if (objects.enemies[i]->getId() == -1) {
						FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + static_cast<BaseBoss*>(objects.enemies[i].get())->getName()).draw(548 + damageUIEffectOffsetX, 25 - 6 - easeBossAppear * marshmallowUIOffset);
					}
				}
			}
			break;
		case GameState::BossBattle:
			for (int i = 0; i < objects.enemies.size(); i++) {
				if (objects.enemies[i]->getId() == -1) {
					FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + static_cast<BaseBoss*>(objects.enemies[i].get())->getName()).draw(548 + damageUIEffectOffsetX, 25 - 6 - easeBossAppear * marshmallowUIOffset);
				}
			}
			break;
		}
		//FontAsset(U"GameUI_Pixel")(U"HP").draw(872, 84);
	}

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
	if (Key1.pressed()) currentStage = (Stage)0;
	if (Key2.pressed()) currentStage = (Stage)1;
	if (Key3.pressed()) currentStage = (Stage)2;
	if (Key4.pressed()) currentStage = (Stage)3;
	if (Key5.pressed()) currentStage = (Stage)4;
	if (Key6.pressed()) currentStage = (Stage)5;

	switch (gameState) {
	case GameState::Tutorial:
		break;

	case GameState::StageStart:
		{
			//描画に使うタイマー
			drawTimer += Scene::DeltaTime();

			stageStartTimer += Scene::DeltaTime();

			switch (stageStartState) {
			case StageStartState::Start:
				{
					stageStartEaseTimer += Scene::DeltaTime();
					if (currentFrame(7, 0.080, stageStartAnimTimer) != 7) stageStartAnimTimer += Scene::DeltaTime();
					const double stageStartEase = Min(EaseOutExpo(stageStartEaseTimer / 1.5), 1.0);
					stageNameTextPos = stageNameTextStartPos.lerp(stageNameTextMiddlePos, stageStartEase);
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
					if(currentStage == Stage::Morning) objects.player->toStartPos();
					//次に進む
					if (2.0 <= stageStartEaseTimer) {
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

		switch (currentStage) {
		case Stage::Morning:

			//空になったら次のステージ
			if (spawnEnemyData.empty() && objects.enemies.empty()) {
				//currentStage = Stage::Noon;
				//gameState = GameState::StageStart;
				////次のステージをロード
				//loadJson(U"stage/stage2.json");

				gameState = GameState::StageStart;
				gameState = GameState::BossAppear;
			}
			break;

		case Stage::Noon:
			break;

		case Stage::AfterNoon:
			break;

		case Stage::Evening:
			break;

		case Stage::Night:
			break;

		case Stage::MidNight:
			break;
		}

		//ヒットストップをするか判定
		if (objects.player->getIsHitStopStart()) {
			hitStopTimer = 0;
			isHitStopping = true;
			isDamageUIEffectPlaying = true;
			counterForSlowUpdate = 0;
		}

		//ヒットストップ時
		if (isHitStopping) {
			//タイマー
			hitStopTimer += Scene::DeltaTime();
			counterForSlowUpdate++;
			//3フレームに一回だけ更新(スローにする)
			if (counterForSlowUpdate % /*(int)(Max(*/3/* * (1.0 - Min(EaseOutQuart(hitStopTimer) / hitStopTime, 1.0)), 1.0))*/ == 0) {
				stageUpdate();
			}
			if (hitStopTime <= hitStopTimer) {
				isHitStopping = false;
			}
		}
		//ヒットストップなし
		else {
			stageUpdate();
		}
		break;

	case GameState::BossAppear:
		//タイマー更新
		gameStateTimer += Scene::DeltaTime();
		easeTimer1 += Scene::DeltaTime();
		easeTimer2 += Scene::DeltaTime();
		easeTimer3 += Scene::DeltaTime();

		if (not isSpawnBoss) {
			//ボス
			objects.enemies << std::make_unique<GarbageBox>(objects, Vec2(Scene::Size().x + 100, (Scene::Size().y - TextureAsset(U"UIBack").size().y * 6) / 2.0 + TextureAsset(U"UIBack").size().y * 6));
			isSpawnBoss = true;
		}

		//Print << U"bossAppearState:" << (int)bossAppearState;
		//Print << U"gaemeStateTimer:" << gameStateTimer;

		//BossAppearのステート
		switch (bossAppearState) {
		case BossAppearState::HideUI:
			
			if (gameStateTimer <= 1.0/1.5) {
				//UIを隠す
				easeBossAppear = EaseOutQuart(easeTimer1*1.0);
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
			//
			if (gameStateTimer <= 2.0) {
				//プレイヤーの位置を基準位置に
				objects.player->bossAppearStateUpdate(easeTimer2);
				//枠を出す
				ease = EaseOutQuint(gameStateTimer / 2.0);
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
			if (easeTimer4 <= 1.0) {
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
				isHpAnimationStart = true;
				easeTimer7 += Scene::DeltaTime();
				easeBossAppear = 1.0 - EaseOutExpo(Min(easeTimer7 / 1.2, 1.0));
			}

			if (5.8 <= easeTimer4) {
				//ゲームステートを変更
				nextState = GameState::BossBattle;
				isChangeGameState = true;
			}

			//2Dカメラ更新
			camera.update();

			break;
		}
		break;

	case GameState::BossBattle:
		//ヒットストップをするか判定
		if (objects.player->getIsHitStopStart()) {
			hitStopTimer = 0;
			isHitStopping = true;
			isDamageUIEffectPlaying = true;
			counterForSlowUpdate = 0;
		}

		//ヒットストップ時
		if (isHitStopping) {
			//タイマー
			hitStopTimer += Scene::DeltaTime();
			counterForSlowUpdate++;
			//3フレームに一回だけ更新(スローにする)
			if (counterForSlowUpdate % /*(int)(Max(*/3/* * (1.0 - Min(EaseOutQuart(hitStopTimer) / hitStopTime, 1.0)), 1.0))*/ == 0) {
				stageUpdate();
			}
			if (hitStopTime <= hitStopTimer) {
				isHitStopping = false;
			}
		}
		//ヒットストップなし
		else {
			stageUpdate();
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
}

void GameScene::collisionAndRemoveUpdate() {
//当たり判定
//プレイヤーと敵本体
//プレイヤー直接攻撃と敵本体
	if (objects.player->getIsAttackColOn()) {
		for (auto& enemy : objects.enemies) {
			if (enemy->collision().intersects(objects.player->attackCollision())) {
				//Print << U"damage!!!";
				bool isDamaged = enemy->damage(objects.player->getDamageAmount(), true);
				//ダメージエフェクト
				if (isDamaged) effect.add<DamageEffect>(enemy->getPos());
			}
		}
	}
	//プレイヤーの攻撃で倒された敵を破壊
	destroyObjects();
	//プレイヤー本体と敵本体
	for (auto& enemy : objects.enemies) {
		if (enemy->collision().intersects(objects.player->bodyCollision())) {
			objects.player->damage(enemy->getDamageAmount(), false);
		}
	}

	//マシュマロと敵
	for (auto& enemy : objects.enemies) {
		for (auto& maro : objects.marshmallows) {
			if (enemy->collision().intersects(maro->collision())) {
				//敵にヒットした判定をtrueに
				maro->setIsHit(true);
				//敵にダメージを与えられるか判定
				if (maro->isNotEnemyHit(enemy->getId())) {
					//IDを受け取る
					maro->addId(enemy->getId());
					//敵にダメージを与える
					enemy->damage(maro->getDamageAmount(), false);
					//エフェクトを追加
					if (maro->getType() == MaroType::Normal || maro->getType() == MaroType::Empty) {
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
					if (maro->getType() == MaroType::Empty) {
						//敵にダメージを与える
						enemy->damage(maro->getDamageAmount(), false);
						//エフェクトを追加
						effect.add<DamageEffect>(enemy->getPos());
					}
				}
			}
		}
	}

	for (auto& maro : objects.marshmallows) {
		//画面外に行ったクソマロはもう一度boxに
		//boxに戻さないよう変更
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

void GameScene::stageUpdate() {
	//描画に使うタイマー
	drawTimer += Scene::DeltaTime();

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
		//敵以外を更新
		objects.player->update();
		for (int i = 0; i < objects.marshmallows.size();i++) {
			if(objects.marshmallows[i]->getType() == MaroType::Empty) objects.marshmallows[i]->update();
		}
	}
	//敵が停止していない
	else {
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

	//
	collisionAndRemoveUpdate();
}

void GameScene::spawnEnemy() {
	SpawnEnemyData::spawnTimer += Scene::DeltaTime();

	for (int i = 0; i < spawnEnemyData.size(); i++) {
		if (spawnEnemyData[i].isSpawnable()) {
			switch (spawnEnemyData[i].type) {
			case EnemyType::Empty: /*何もしない*/; break;
			case EnemyType::Bag: objects.enemies << std::make_unique<GarbageBagNormal>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::FastBag: objects.enemies << std::make_unique<GarbageBagFast>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::BagWithCan: objects.enemies << std::make_unique<GarbageBagWithCan>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::Can: objects.enemies << std::make_unique<Can>(objects, spawnEnemyData[i].pos, Vec2{-1,0}); break;
			case EnemyType::Fish: objects.enemies << std::make_unique<Fish>(objects, spawnEnemyData[i].pos); break;
			case EnemyType::Umbrella: objects.enemies << std::make_unique<Umbrella>(objects, spawnEnemyData[i].pos); break;
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
			else if (str == U"FastBag") {
				n = (int)EnemyType::FastBag;
			}
			else if (str == U"BagWithCan") {
				n = (int)EnemyType::BagWithCan;
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
			else {
				throw Error(U"GameSceneForEditor:未定義の敵です");
			}
			data.type = (EnemyType)n;
			spawnEnemyData << data;
		}
	}
}

void GameScene::commonDraw()const {
	{
		const ScopedRenderTarget2D renderTarget{ renderTexture };

		//背景
		//Scene::Rect()
		//	.draw(Arg::top = ColorF{ 0.2, 0.5, 1.0 }, Arg::bottom = ColorF{ 0.5, 0.8, 1.0 });
		//const double skyPosX = -(int)(drawTimer / 0.0410) % (320 * 3);
		const double cloudBigPosX = -(int)(drawTimer /    0.0100) % (320 * 3);
		const double cloudNormalPosX = -(int)(drawTimer / 0.0125) % (320 * 3);
		const double cloudSmallPosX = -(int)(drawTimer /  0.0150) % (320 * 3);
		const double farBackMountainPosX = -(int)(drawTimer / 0.0250) % (320 * 3);
		const double backMountainPosX = -(int)(drawTimer /   0.0300) % (320 * 3);
		const double middleMountainPosX = -(int)(drawTimer / 0.0175) % (320 * 3);
		const double frontMountainPosX = -(int)(drawTimer /  0.0125) % (320 * 3);
		const double frontCityPosX = -(int)(drawTimer /  0.00425) % (320 * 3);
		const double middleCityPosX = -(int)(drawTimer / 0.00325) % (320 * 3);
		const double backCityPosX = -(int)(drawTimer /   0.00225) % (320 * 3);
		const Vec2 rainPos = { -(int)(drawTimer / 0.00200) % (320 * 3), (int)(drawTimer / 0.00200) % (214 * 3) };

		//空
		switch (currentStage) {
		case Stage::Morning: TextureAsset(U"SkyMorning").scaled(3).draw(0, 0); break;
		case Stage::Noon: TextureAsset(U"SkyNoon").scaled(3).draw(0, 0); break;
		case Stage::AfterNoon: TextureAsset(U"SkyAfterNoon").scaled(3).draw(0, 0); break;
		case Stage::Evening: TextureAsset(U"SkyEvening").scaled(3).draw(0, 0); break;
		case Stage::Night: TextureAsset(U"SkyNight").scaled(3).draw(0, 0); break;
		case Stage::MidNight: TextureAsset(U"SkyMidNight").scaled(3).draw(0, 0); break;
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
		TextureAsset(U"CloudSmall").scaled(3).draw(cloudSmallPosX, 0);
		TextureAsset(U"CloudSmall").scaled(3).draw(Scene::Size().x + cloudSmallPosX, 0);
		TextureAsset(U"CloudNormal").scaled(3).draw(cloudNormalPosX, 0);
		TextureAsset(U"CloudNormal").scaled(3).draw(Scene::Size().x + cloudNormalPosX, 0);
		TextureAsset(U"CloudBig").scaled(3).draw(cloudBigPosX, 0);
		TextureAsset(U"CloudBig").scaled(3).draw(Scene::Size().x + cloudBigPosX, 0);
		/*TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ 0, 0 });
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ Scene::Size().x, 0 });
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ 0, -Scene::Size().y });
		TextureAsset(U"Rain").scaled(3).draw(rainPos + Vec2{ Scene::Size().x, -Scene::Size().y });*/


		//背景とオブジェクトの間
		//後面プレイヤー攻撃エフェクト
		objects.player->drawEffectBack();
		//ビームの描画
		for (auto& maro : objects.marshmallows) {
			maro->backGroundDraw();
		}

		//オブジェクト
		objects.player->draw();

		for (int i = 0; i < objects.marshmallows.size(); i++) {
			objects.marshmallows[i]->draw();
		}

		for (auto& enemy : objects.enemies) {
			enemy->draw();
		}

		//UI
		drawMarshmallowUI();

		//エフェクトを再生
		//前面プレイヤー攻撃エフェクト
		objects.player->drawEffectFront();
		//gameSceneのエフェクト
		effect.update();


		//Debug
		//TextureAsset(U"Fish").draw(300,200);
		//スプライトシートを再生
		/*int n = (int)(drawTimer / 0.035) % 21;
		TextureAsset(U"UiAttackEffect")(n * TextureAsset(U"UiAttackEffect").size().x / 21.0, 0, TextureAsset(U"UiAttackEffect").size().x / 21.0, TextureAsset(U"UiAttackEffect").size().y).scaled(3).drawAt(objects.player->getPos());*/
		//TextureAsset(U"Umbrella").scaled(3).draw(300,200);

	}

	// シーン転送時の拡大縮小方法を最近傍法にする
	Scene::SetTextureFilter(TextureFilter::Nearest);
	//文字以外のレンダーテクスチャをdraw
	renderTexture.draw();

	// シーン転送時の拡大縮小方法をバイリニア法にする
	Scene::SetTextureFilter(TextureFilter::Linear);
	//文字のレンダーテクスチャをdraw
	fontRenderTexture.draw();
}

void GameScene::draw() const {

	switch (gameState) {
	case GameState::Tutorial:
		break;

	case GameState::StageStart:
		commonDraw();
		
		//if (stageStartTimer <= 1.0) {
		//	FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)currentStage + 1, U": ", stageName[(int)currentStage]).drawAt(stageNameTextPos);
		//	FontAsset(U"GameUI_BestTenDot")(U"～始まりの朝～").drawAt(stageNameTextPos + Vec2{ 0, 34 });
		//}
		//else if (stageStartTimer < 1.5) {
		//	FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)currentStage + 1, U": ", stageName[(int)currentStage]).drawAt(stageNameTextMiddlePos);
		//	FontAsset(U"GameUI_BestTenDot")(U"～始まりの朝～").drawAt(stageNameTextMiddlePos + Vec2{ 0, 34 });
		//}
		//else if (1.5 <= stageStartTimer) {
		//	FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)currentStage + 1, U": ", stageName[(int)currentStage]).drawAt(stageNameTextPos);
		//	FontAsset(U"GameUI_BestTenDot")(U"～始まりの朝～").drawAt(stageNameTextPos + Vec2{ 0, 34 });
		//}

		switch (stageStartState) {
		case StageStartState::Start:
			drawSpriteAnim(U"StageNameIn", 7, 0.080, Scene::CenterF(), stageStartAnimTimer);
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)currentStage + 1, U": ", stageName[(int)currentStage]).drawAt(stageNameTextPos, ColorF(1.0, 1.0 * Min(0.05+EaseInQuart(stageStartEaseTimer * 2.0), 1.0)) );
			FontAsset(U"GameUI_BestTenDot")(U"～始まりの朝～").drawAt(stageNameTextPos + Vec2{ 0, 34 }, ColorF(1.0, 1.0 * Min(0.05+EaseInQuart(stageStartEaseTimer * 2.0), 1.0)) );
			break;

		case StageStartState::Middle:
			drawSpriteAnim(U"StageNameIn", 7, 0.080, Scene::CenterF(), stageStartAnimTimer);
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)currentStage + 1, U": ", stageName[(int)currentStage]).drawAt(stageNameTextMiddlePos);
			FontAsset(U"GameUI_BestTenDot")(U"～始まりの朝～").drawAt(stageNameTextMiddlePos + Vec2{ 0, 34 });
			break;

		case StageStartState::End:
			drawSpriteAnim(U"StageNameOut", 6, 0.100, Scene::CenterF(), stageStartAnimTimer);
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)currentStage + 1, U": ", stageName[(int)currentStage]).drawAt(stageNameTextPos, ColorF(1.0, 1.0 * (1.0 - Min(EaseInQuart(stageStartEaseTimer * 2.0), 1.0))));
			FontAsset(U"GameUI_BestTenDot")(U"～始まりの朝～").drawAt(stageNameTextPos + Vec2{ 0, 34 }, ColorF(1.0, 1.0 * (1.0-Min(EaseInQuart(stageStartEaseTimer * 2.0), 1.0))) );
			break;
		}
		break;

	case GameState::Stage:
		commonDraw();

		break;

	case GameState::BossAppear:
		switch (bossAppearState) {
		case BossAppearState::HideUI:
			commonDraw();
			break;

		case BossAppearState::DrawRect:
			commonDraw();
			RectF(0, 0, Scene::Size().x, ease * maxTopRectHeight).draw(ColorF(0, 0, 0, 0.9));
			RectF(0, Scene::Size().y - ease * maxBottomRectHeight, Scene::Size().x, maxBottomRectHeight).draw(ColorF(0, 0, 0, 0.85));
			break;

		case BossAppearState::AppearBoss:
			{
				// 2D カメラの設定から Transformer2D を作成
				const auto t = camera.createTransformer();

				//描画
				commonDraw();
				RectF(0, 0, Scene::Size().x, ease * maxTopRectHeight).draw(ColorF(0, 0, 0, 0.9));
				RectF(0, Scene::Size().y - ease * maxBottomRectHeight, Scene::Size().x, maxBottomRectHeight).draw(ColorF(0, 0, 0, 0.85));

			}
			break;
		}
		break;

	case GameState::BossBattle:
		commonDraw();
		break;
	}
}
