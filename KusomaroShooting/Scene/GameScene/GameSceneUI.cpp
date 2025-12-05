#include "stdafx.h"
#include "GameSceneUI.h"
#include "GameScene.h"

GameSceneUI::GameSceneUI(GameScene *gameScene_)
	: gameScene(gameScene_)
{}

/// @brief 最大のアルファ値を書き込むブレンドステートを返します。
BlendState MaxAlphaBlend()
{
	BlendState blend = BlendState::Default2D;
	blend.opAlpha = BlendOp::Max;
	blend.dstAlpha = Blend::DestAlpha;
	blend.srcAlpha = Blend::SrcAlpha;
	return blend;
}

void GameSceneUI::drawHpBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY, double healEase, double damageEase)const {
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

void GameSceneUI::drawMaroBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY, double healEase)const {
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

void GameSceneUI::drawBossBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY, BaseBoss* boss)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(6).draw(backBarPosX, barPosY);

	//ダメージアニメーションのカット後pxを計算
	const double damageBarDrawEndPosX = frontBar.size().x * ((double)boss->getPrevHpDamage() / (double)boss->getMaxHp()) * ((double)boss->getPrevHpDamage() - (double)boss->getCurrentHp()) / (double)boss->getPrevHpDamage() * boss->getDamageEase();
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

void GameSceneUI::drawUIUimm(double damageUIEffectOffsetX, double damageUIEffectOffsetY)const {
	int n = (int)(gameScene->getBackDrawTimer() / 0.08) % 65;
	//プレイヤーが瀕死なら
	if (gameScene->objects.player->getIsCrisis()) {
		//赤く点滅
		const ScopedColorMul2D colorMul{ ColorF(1.0, 0.8 + 0.2 * Periodic::Sawtooth0_1(0.9135s), 0.8 + 0.2 * Periodic::Sawtooth0_1(0.9135s), 1.0) };
		if (gameScene->objects.player->isInvincibility()) drawSpriteAnim(U"UimmDamageForUI", 3, 0.250, { 0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY }, 6);
		else if (gameScene->objects.player->getIsCrisis()) TextureAsset(U"UimmCrisisForUI")(n * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset);
		else TextureAsset(U"UimmNormalForUI")(n * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset);

	}
	else {
		if (gameScene->objects.player->isInvincibility()) drawSpriteAnim(U"UimmDamageForUI", 3, 0.250, { 0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY }, 6);
		else if (gameScene->objects.player->getIsCrisis()) TextureAsset(U"UimmCrisisForUI")(n * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset);
		else TextureAsset(U"UimmNormalForUI")(n * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset);

	}
}

void GameSceneUI::drawMarshmallowUI() const {

	//Rect{ 0,0,Scene::Size().x,40 }.draw(ColorF(0.9, 0.3, 0.3));

	//Print << isHpAnimationStart;

	const double damageUIEffectOffsetX = (1.0 - EaseInOutQuart(Min(gameScene->damageUIEffectTimer, 1.0))) * 2.5 * Sin(gameScene->damageUIEffectTimer * Math::Pi * 13.0);
	const double damageUIEffectOffsetY = -(1.0 - EaseInOutCirc(Min(gameScene->damageUIEffectTimer, 1.0))) * 1.5 * Cos(gameScene->damageUIEffectTimer * Math::Pi * 13.0);

	//UIの背景
	int n = (int)(gameScene->getBackDrawTimer() / 0.08) % 65;
	//ボス戦中
	if (gameScene->isHpAnimationStart) {
		TextureAsset(U"UIBackWithBox")(n * TextureAsset(U"UIBackWithBox").size().x / 65, 0, TextureAsset(U"UIBackWithBox").size().x / 65.0, TextureAsset(U"UIBackWithBox").size().y).scaled(6).draw(0 - 6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset);
		drawUIUimm(damageUIEffectOffsetX, damageUIEffectOffsetY);
	}
	//ボス戦以外
	else {
		TextureAsset(U"UIBack")(n * TextureAsset(U"UIBack").size().x / 65, 0, TextureAsset(U"UIBack").size().x / 65.0, TextureAsset(U"UIBack").size().y).scaled(6).draw(-6 + damageUIEffectOffsetX, -6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset);
		drawUIUimm(damageUIEffectOffsetX, damageUIEffectOffsetY);
	}


	//HPバー
	drawHpBar(gameScene->objects.player->getHpEase(), 1.0, TextureAsset(U"PlayerBarBack"), TextureAsset(U"PlayerHpFront"), 50 * 3 - 4 + damageUIEffectOffsetX, 6 * 3 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY, gameScene->objects.player->getHpHealEase(), gameScene->objects.player->getDamageEase());

	//マシュマロバー
	drawMaroBar(gameScene->objects.player->getMaroEase(), 1.0, TextureAsset(U"PlayerBarBack"), TextureAsset(U"MarshmallowBarFront"), 50 * 3 - 4 + damageUIEffectOffsetX, 24 * 3 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY, gameScene->objects.player->getMaroAddEase());

	//ボスのHPバー
	if (gameScene->bossPtr != nullptr) {
		if (gameScene->isHpAnimationStart && not gameScene->isHpAnimationEnd) {
			const double easeHpAnimation = Min(EaseInLinear(gameScene->easeBossHpAnimationTimer / 3.0), (double)gameScene->bossPtr->getCurrentHp() / (double)gameScene->bossPtr->getMaxHp());
			drawBossBar(easeHpAnimation, 1.0, TextureAsset(U"BossBarBack"), TextureAsset(U"BossBarFront"), (320 - TextureAsset(U"BossBarBack").size().x * 2 - 16) * 3 + damageUIEffectOffsetX, 26 * 3 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, gameScene->bossPtr);
		}
	}

	//Print << U"ease*mar" << easeBossAppear * marshmallowUIOffset;
	//次のマシュマロを表示
	TextureAsset(U"MarshmallowBox").scaled(6).drawAt((320 / 2 - 5) * 3 + damageUIEffectOffsetX, 24 * 3 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY);

	switch (gameScene->objects.player->getNextMaro()) {
	case MaroType::Normal: TextureAsset(U"Marshmallow").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5) * 3 + damageUIEffectOffsetX, 24 * 3 + 6 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Heal: TextureAsset(U"HealMaro").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5) * 3 + damageUIEffectOffsetX, 24 * 3 + 6 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Up: TextureAsset(U"KusomaroUp").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5 - 4) * 3 + damageUIEffectOffsetX, 24 * 3 + 6 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Down: TextureAsset(U"KusomaroDown").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 1) * 3 + damageUIEffectOffsetX, 24 * 3 + 6 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Sine: TextureAsset(U"KusomaroSine").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5 + Math::Cos(gameScene->getBackDrawTimer() * Math::Pi * 2)) * 3 - 6 + 6 + damageUIEffectOffsetX, 24 * 3 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY); break;
	case MaroType::Beam: TextureAsset(U"KusomaroBeam").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5) * 3 + damageUIEffectOffsetX, 24 * 3 + 6 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY); break;
	}


	{
		const ScopedRenderTarget2D renderTarget{ gameScene->fontRenderTexture };

		//背景を透明に
		gameScene->fontRenderTexture.clear(ColorF{ 1.0, 0.0 });

		//アルファ値を戻す
		const ScopedRenderStates2D renderState{ MaxAlphaBlend() };

		FontAsset(U"GameUI_Pixel")(U"HP").draw(165 - 4 + damageUIEffectOffsetX + 2.0, 24 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY + 2.5, ColorF(0.25, 0.25, 0.25));
		FontAsset(U"GameUI_Pixel")(U"HP").draw(165 - 4 + damageUIEffectOffsetX, 24 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY);

		//Print << U"easeBoss" << marshmallowUIOffset*easeBossAppear;
		//if (objects.player->getNumMarshmallows() < 10) FontAsset(U"GameUI_Pixel")(U"Marshmallow\n    ", objects.player->getNumMarshmallows(), U"/30").drawAt(74, 93);
		/*else*/
		FontAsset(U"GameUI_Pixel")(U"MP:", gameScene->objects.player->getNumMarshmallows(), U"/30").draw(165 - 4 + damageUIEffectOffsetX + 2.0, 24 * 3 + 14 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY + 2.0, ColorF(0.25, 0.25, 0.25));
		FontAsset(U"GameUI_Pixel")(U"MP:", gameScene->objects.player->getNumMarshmallows(), U"/30").draw(165 - 4 + damageUIEffectOffsetX, 24 * 3 + 14 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY);

		FontAsset(U"GameUI_Pixel")(U"Next").draw(423 + damageUIEffectOffsetX + 2.0, 12 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY + 2.0, ColorF(0.25, 0.25, 0.25));
		FontAsset(U"GameUI_Pixel")(U"Next").draw(423 + damageUIEffectOffsetX, 12 + 8 - 6 + 1.5 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + damageUIEffectOffsetY);

		//ステージ名orボスの名前
		switch (gameScene->gameState) {
		case GameScene::GameState::Tutorial:
			FontAsset(U"GameUI_BestTenDot30")(U"Stage:おためし部屋").draw(548 + damageUIEffectOffsetX + 3.0, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
			FontAsset(U"GameUI_BestTenDot30")(U"Stage:おためし部屋").draw(548 + damageUIEffectOffsetX, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
			//
			FontAsset(U"GameUI_BestTenDot")(U"・操作の練習ができる部屋です").draw(548 + 14 + damageUIEffectOffsetX + 3.0, 25 - 6 + 54 + 3.0 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
			FontAsset(U"GameUI_BestTenDot")(U"・操作の練習ができる部屋です").draw(548 + 14 + damageUIEffectOffsetX, 25 - 6 + 54 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
			break;

		case GameScene::GameState::StageStart:
		case GameScene::GameState::Stage:
			FontAsset(U"GameUI_BestTenDot30")(U"Stage:" + gameScene->stageName[(int)gameScene->currentStage]).draw(548 + damageUIEffectOffsetX + 3.0, 25 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
			FontAsset(U"GameUI_BestTenDot30")(U"Stage:" + gameScene->stageName[(int)gameScene->currentStage]).draw(548 + damageUIEffectOffsetX, 25 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
			//スコア表示
			FontAsset(U"GameUI_BestTenDot")(U"Score:", (int)Min(gameScene->prevScore + ((double)gameScene->currentScore - gameScene->prevScore) * Min(EaseOutLinear(gameScene->scoreAnimTimer), 1.0), (double)gameScene->currentScore)).draw(548 + 14 + damageUIEffectOffsetX + 3.0, 25 - 6 + 54 + 3.0 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
			FontAsset(U"GameUI_BestTenDot")(U"Score:", (int)Min(gameScene->prevScore + ((double)gameScene->currentScore - gameScene->prevScore) * Min(EaseOutLinear(gameScene->scoreAnimTimer), 1.0), (double)gameScene->currentScore)).draw(548 + 14 + damageUIEffectOffsetX, 25 - 6 + 54 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
			break;

		case GameScene::GameState::BossAppear:
			if (gameScene->bossAppearState == GameScene::BossAppearState::AppearBoss) {
				if (gameScene->bossPtr != nullptr) {
					FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX + 3.0, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
					FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
				}
			}
			break;
		case GameScene::GameState::BossBattle:
			if (gameScene->bossPtr != nullptr) {
				FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX + 3.0, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
				FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
			}
			break;

		case GameScene::GameState::Pause:
			switch (gameScene->prevGameState) {
			case GameScene::GameState::Tutorial:
				FontAsset(U"GameUI_BestTenDot30")(U"Stage:おためし部屋").draw(548 + damageUIEffectOffsetX + 3.0, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
				FontAsset(U"GameUI_BestTenDot30")(U"Stage:おためし部屋").draw(548 + damageUIEffectOffsetX, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
				//
				FontAsset(U"GameUI_BestTenDot")(U"・操作の練習ができる部屋です").draw(548 + 14 + damageUIEffectOffsetX + 3.0, 25 - 6 + 54 + 3.0 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
				FontAsset(U"GameUI_BestTenDot")(U"・操作の練習ができる部屋です").draw(548 + 14 + damageUIEffectOffsetX, 25 - 6 + 54 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
				break;

			case GameScene::GameState::StageStart:
			case GameScene::GameState::Stage:
				FontAsset(U"GameUI_BestTenDot30")(U"Stage:" + gameScene->stageName[(int)gameScene->currentStage]).draw(548 + damageUIEffectOffsetX + 3.0, 25 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
				FontAsset(U"GameUI_BestTenDot30")(U"Stage:" + gameScene->stageName[(int)gameScene->currentStage]).draw(548 + damageUIEffectOffsetX, 25 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
				//スコア表示
				FontAsset(U"GameUI_BestTenDot")(U"Score:", (int)Min(gameScene->prevScore + ((double)gameScene->currentScore - gameScene->prevScore) * Min(EaseOutLinear(gameScene->scoreAnimTimer), 1.0), (double)gameScene->currentScore)).draw(548 + 14 + damageUIEffectOffsetX + 3.0, 25 - 6 + 54 + 3.0 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
				FontAsset(U"GameUI_BestTenDot")(U"Score:", (int)Min(gameScene->prevScore + ((double)gameScene->currentScore - gameScene->prevScore) * Min(EaseOutLinear(gameScene->scoreAnimTimer), 1.0), (double)gameScene->currentScore)).draw(548 + 14 + damageUIEffectOffsetX, 25 - 6 + 54 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
				break;

			case GameScene::GameState::BossAppear:
				if (gameScene->bossAppearState == GameScene::BossAppearState::AppearBoss) {
					if (gameScene->bossPtr != nullptr) {
						FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX + 3.0, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
						FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
					}
				}
				break;
			case GameScene::GameState::BossBattle:
				if (gameScene->bossPtr != nullptr) {
					FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX + 3.0, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset + 3.0, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
					FontAsset(U"GameUI_BestTenDot30")(U"Boss:" + gameScene->bossPtr->getName()).draw(548 + damageUIEffectOffsetX, 25 - 6 - gameScene->easeBossAppear * gameScene->marshmallowUIOffset, ColorF(0.95));
				}
				break;
			}
			break;
		}
		//FontAsset(U"GameUI_Pixel")(U"HP").draw(872, 84);
	}

}

void GameSceneUI::commonUIDraw()const {
	// マシュマロUI
	drawMarshmallowUI();

	//文字のレンダーテクスチャをdraw
	gameScene->fontRenderTexture.draw();

	//プレイヤー生存時
	if (gameScene->objects.player->getHp() != 0) {
	}
	//プレイヤー死亡時
	else {

		if (1.5 <= gameScene->hitStopTimer) {
			//Print << 1.0 * Clamp(EaseOutCubic(gameOverTimer), 0.0, 1.0);
			//不透明度を上げる
			const ScopedColorMul2D colorMul{ ColorF(1.0, 1.0 * Min(EaseOutCubic(gameScene->gameOverTimer / 1.5), 1.0)) };

			Rect(0, 0, Scene::Size()).draw(ColorF(0, 0.6));
			//GameOver 
			FontAsset(U"GameUI_BestTenDot90")(U"Game Over").drawAt(Scene::CenterF().movedBy(3, -50 + 3), shadowColor);
			FontAsset(U"GameUI_BestTenDot90")(U"Game Over").drawAt(Scene::CenterF().movedBy(0, -50), ColorF(0.90));

			switch (gameScene->selectedButton) {
			case GameScene::SelectedButton::ReStart:

				Triangle(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-110, 2.25), 35).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(0.9135s, Scene::Time() + 0.9135 / 2)));

				FontAsset(U"GameUI_BestTenDot45")(U"Restart").drawAt(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(10 + 3, 0 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot45")(U"Restart").drawAt(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(10, 0), activeColor);

				FontAsset(U"GameUI_BestTenDot45")(U"Continue").drawAt(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-10 + 3, 0 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot45")(U"Continue").drawAt(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-10, 0), inactiveColor);

				FontAsset(U"GameUI_BestTenDot30")(U"BackToTitle").draw(Arg::bottomRight = Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-10 - 10 + 1.5, -10 - 10 + 1.5), shadowColor);
				FontAsset(U"GameUI_BestTenDot30")(U"BackToTitle").draw(Arg::bottomRight = Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-10 - 10, -10 - 10), inactiveColor);

				FontAsset(U"GameUI_BestTenDot")(U"初めのステージからやり直します。").drawAt(Scene::CenterF().movedBy(0 + 3, 100 + 3), ColorF(0.2));
				FontAsset(U"GameUI_BestTenDot")(U"初めのステージからやり直します。").drawAt(Scene::CenterF().movedBy(0, 100), activeColor);
				break;

			case GameScene::SelectedButton::Continue:

				Triangle(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-140, 2.25), 35).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(0.9135s, Scene::Time() + 0.9135 / 2)));

				FontAsset(U"GameUI_BestTenDot45")(U"Restart").drawAt(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(10 + 3, 0 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot45")(U"Restart").drawAt(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(10, 0), inactiveColor);

				FontAsset(U"GameUI_BestTenDot45")(U"Continue").drawAt(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-10 + 3, 0 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot45")(U"Continue").drawAt(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-10, 0), activeColor);

				FontAsset(U"GameUI_BestTenDot30")(U"BackToTitle").draw(Arg::bottomRight = Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-10 - 10 + 1.5, -10 - 10 + 1.5), shadowColor);
				FontAsset(U"GameUI_BestTenDot30")(U"BackToTitle").draw(Arg::bottomRight = Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-10 - 10, -10 - 10), inactiveColor);

				FontAsset(U"GameUI_BestTenDot")(U"現在のステージの初めからやり直します。\n  ただし、スコアはリセットされます。").drawAt(Scene::CenterF().movedBy(0 + 3, 100 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot")(U"現在のステージの初めからやり直します。\n  ただし、スコアはリセットされます。").drawAt(Scene::CenterF().movedBy(0, 100), activeColor);
				break;

			case GameScene::SelectedButton::BackToTitle:

				Triangle(Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-205 - 10, -23 - 10), 25).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(0.9135s, Scene::Time() + 0.9135 / 2)));

				FontAsset(U"GameUI_BestTenDot45")(U"Restart").drawAt(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(10 + 3, 0 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot45")(U"Restart").drawAt(Scene::CenterF().movedBy(-Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(10, 0), inactiveColor);

				FontAsset(U"GameUI_BestTenDot45")(U"Continue").drawAt(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-10 + 3, 0 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot45")(U"Continue").drawAt(Scene::CenterF().movedBy(Scene::CenterF().x / 2.0, Scene::CenterF().y / 2.0).movedBy(-10, 0), inactiveColor);

				FontAsset(U"GameUI_BestTenDot30")(U"BackToTitle").draw(Arg::bottomRight = Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-10 - 10 + 1.5, -10 - 10 + 1.5), shadowColor);
				FontAsset(U"GameUI_BestTenDot30")(U"BackToTitle").draw(Arg::bottomRight = Vec2{ Scene::Size().x, Scene::Size().y }.movedBy(-10 - 10, -10 - 10), activeColor);

				FontAsset(U"GameUI_BestTenDot")(U"タイトルに戻ります。").drawAt(Scene::CenterF().movedBy(0 + 3, 100 + 3), shadowColor);
				FontAsset(U"GameUI_BestTenDot")(U"タイトルに戻ります。").drawAt(Scene::CenterF().movedBy(0, 100), activeColor);
				break;
			}

		}
	}
	//操作方法
	if (isDrawGuide && gameScene->gameState != GameScene::GameState::Tutorial) TextureAsset(U"Manual").scaled(0.5).draw(ColorF(1.0, 0.5));
}

void GameSceneUI::draw()const {
	switch (gameScene->gameState) {
	case GameScene::GameState::Tutorial:
		switch (gameScene->tutorialState) {
		case GameScene::TutorialState::Move:
			commonUIDraw();

			Rect(0, 0, Scene::Size()).draw(ColorF(0.0, 0.6));
			TextureAsset(U"TutorialMove").scaled(0.5).draw();

			//三角
			Triangle(Scene::CenterF().movedBy(360, 0), 60).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			break;

		case GameScene::TutorialState::Attack:
			commonUIDraw();
			Rect(0, 0, Scene::Size()).draw(ColorF(0.0, 0.6));
			TextureAsset(U"TutorialAttack").scaled(0.5).draw();

			//左三角
			Triangle(Scene::CenterF().movedBy(-360, 0), 60).rotated(-90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			//右三角
			Triangle(Scene::CenterF().movedBy(360, 0), 60).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			break;

		case GameScene::TutorialState::Maro1:
			commonUIDraw();
			Rect(0, 0, Scene::Size()).draw(ColorF(0.0, 0.6));
			TextureAsset(U"TutorialMaro1").scaled(0.5).draw();
			//左三角
			Triangle(Scene::CenterF().movedBy(-360, 0), 60).rotated(-90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			//右三角
			Triangle(Scene::CenterF().movedBy(360, 0), 60).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			break;

		case GameScene::TutorialState::Maro2:
			commonUIDraw();
			Rect(0, 0, Scene::Size()).draw(ColorF(0.0, 0.6));
			TextureAsset(U"TutorialMaro2").scaled(0.5).draw();

			//左三角
			Triangle(Scene::CenterF().movedBy(-360, 0), 60).rotated(-90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			//右三角
			Triangle(Scene::CenterF().movedBy(360, 0), 60).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			break;

		case GameScene::TutorialState::Score:
			commonUIDraw();
			Rect(0, 0, Scene::Size()).draw(ColorF(0.0, 0.6));
			TextureAsset(U"TutorialScore").scaled(0.5).draw();

			//左三角
			Triangle(Scene::CenterF().movedBy(-360, 0), 60).rotated(-90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			//右三角
			Triangle(Scene::CenterF().movedBy(360, 0), 60).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			break;

		case GameScene::TutorialState::Pause:
			commonUIDraw();
			Rect(0, 0, Scene::Size()).draw(ColorF(0.0, 0.6));

			TextureAsset(U"TutorialPause").scaled(0.5).draw();

			//左三角
			Triangle(Scene::CenterF().movedBy(-360, 0), 60).rotated(-90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
			break;

		case GameScene::TutorialState::Try:
			commonUIDraw();
			TextureAsset(U"Manual").scaled(0.5).draw(ColorF(1.0, 0.8));
			// ↓ボタンクラスを作って処理と分割したい
			if (Rect(Scene::Size().x - 10 - TextureAsset(U"Question").size().x * 5, 0 + 10 + TextureAsset(U"UIBack").size().y * 6, TextureAsset(U"Question").size() * 5).mouseOver()) TextureAsset(U"Question").scaled(5).draw(Scene::Size().x - 10 - TextureAsset(U"Question").size().x * 5, 0 + 4 + TextureAsset(U"UIBack").size().y * 6, ColorF(1.0, 1));
			else TextureAsset(U"Question").scaled(5).draw(Scene::Size().x - 10 - TextureAsset(U"Question").size().x * 5, 0 + 4 + TextureAsset(U"UIBack").size().y * 6, ColorF(1.0, 0.6));
			//Rect(Scene::Size().x - 10 - TextureAsset(U"Question").size().x * 5, 0 + 10 + TextureAsset(U"UIBack").size().y * 6, TextureAsset(U"Question").size()*5).draw(ColorF(1.0, 0.6));
			break;
		}
		break;

	case GameScene::GameState::StageStart:
		commonUIDraw();

		switch (gameScene->stageStartState) {
		case GameScene::StageStartState::Start:
			drawSpriteAnimForTimerAt(U"StageNameIn", 7, 0.080, Scene::CenterF(), gameScene->stageStartAnimTimer);
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)gameScene->currentStage + 1, U": ", gameScene->stageName[(int)gameScene->currentStage]).drawAt(gameScene->stageNameTextPos + Vec2{ 3.0, 3.0 }, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0, 1.0 * Min(0.05 + EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0)));
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)gameScene->currentStage + 1, U": ", gameScene->stageName[(int)gameScene->currentStage]).drawAt(gameScene->stageNameTextPos, ColorF(0.95, 1.0 * Min(0.05 + EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0)));
			FontAsset(U"GameUI_BestTenDot")(U"～", gameScene->stageSubName[(int)gameScene->currentStage], U"～").drawAt(gameScene->stageNameTextPos + Vec2{ 0 + 3.0, 34 + 3.0 }, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0, 1.0 * Min(0.05 + EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0)));
			FontAsset(U"GameUI_BestTenDot")(U"～", gameScene->stageSubName[(int)gameScene->currentStage], U"～").drawAt(gameScene->stageNameTextPos + Vec2{ 0, 34 }, ColorF(0.95, 1.0 * Min(0.05 + EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0)));
			break;

		case GameScene::StageStartState::Middle:
			drawSpriteAnimForTimerAt(U"StageNameIn", 7, 0.080, Scene::CenterF(), gameScene->stageStartAnimTimer);
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)gameScene->currentStage + 1, U": ", gameScene->stageName[(int)gameScene->currentStage]).drawAt(gameScene->stageNameTextMiddlePos + Vec2{ 3.0, 3.0 }, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)gameScene->currentStage + 1, U": ", gameScene->stageName[(int)gameScene->currentStage]).drawAt(gameScene->stageNameTextMiddlePos, ColorF(0.95));
			FontAsset(U"GameUI_BestTenDot")(U"～", gameScene->stageSubName[(int)gameScene->currentStage], U"～").drawAt(gameScene->stageNameTextMiddlePos + Vec2{ 0 + 3.0, 34 + 3.0 }, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0));
			FontAsset(U"GameUI_BestTenDot")(U"～", gameScene->stageSubName[(int)gameScene->currentStage], U"～").drawAt(gameScene->stageNameTextMiddlePos + Vec2{ 0, 34 }, ColorF(0.95));
			break;

		case GameScene::StageStartState::End:
			drawSpriteAnimForTimerAt(U"StageNameOut", 6, 0.100, Scene::CenterF(), gameScene->stageStartAnimTimer);
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)gameScene->currentStage + 1, U": ", gameScene->stageName[(int)gameScene->currentStage]).drawAt(gameScene->stageNameTextPos + Vec2{ 3.0, 3.0 }, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0, 1.0 * (1.0 - Min(EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0))));
			FontAsset(U"GameUI_BestTenDot30")(U"Stage", (int)gameScene->currentStage + 1, U": ", gameScene->stageName[(int)gameScene->currentStage]).drawAt(gameScene->stageNameTextPos, ColorF(0.95, 1.0 * (1.0 - Min(EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0))));
			FontAsset(U"GameUI_BestTenDot")(U"～", gameScene->stageSubName[(int)gameScene->currentStage], U"～").drawAt(gameScene->stageNameTextPos + Vec2{ 0 + 3.0, 34 + 3.0 }, ColorF(139 / 255.0, 26 / 255.0, 26 / 255.0, 1.0 * (1.0 - Min(EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0))));
			FontAsset(U"GameUI_BestTenDot")(U"～", gameScene->stageSubName[(int)gameScene->currentStage], U"～").drawAt(gameScene->stageNameTextPos + Vec2{ 0, 34 }, ColorF(0.95, 1.0 * (1.0 - Min(EaseInQuart(gameScene->stageStartEaseTimer * 2.0), 1.0))));
			break;
		}
		break;

	case GameScene::GameState::Stage:
		commonUIDraw();
		break;

	case GameScene::GameState::BossAppear:
		switch (gameScene->bossAppearState) {
		case GameScene::BossAppearState::ChangeBackGround:
			commonUIDraw();
			break;

		case GameScene::BossAppearState::HideUI:
			commonUIDraw();
			break;

		case GameScene::BossAppearState::DrawRect:
			commonUIDraw();
			RectF(0, 0, Scene::Size().x, gameScene->ease * gameScene->maxTopRectHeight).draw(ColorF(0, 0, 0, 0.9));
			RectF(0, Scene::Size().y - gameScene->ease * gameScene->maxBottomRectHeight, Scene::Size().x, gameScene->maxBottomRectHeight).draw(ColorF(0, 0, 0, 0.85));
			break;

		case GameScene::BossAppearState::AppearBoss:// 要検証
		{
			// 2D カメラの設定から Transformer2D を作成
			const auto t = gameScene->camera.createTransformer();

			//描画
			commonUIDraw();
			RectF(0, 0, Scene::Size().x, gameScene->ease * gameScene->maxTopRectHeight).draw(ColorF(0, 0, 0, 0.9));
			RectF(0, Scene::Size().y - gameScene->ease * gameScene->maxBottomRectHeight, Scene::Size().x, gameScene->maxBottomRectHeight).draw(ColorF(0, 0, 0, 0.85));
		}
		break;
		}
		break;

	case GameScene::GameState::BossBattle:
		commonUIDraw();
		break;

	case GameScene::GameState::Pause:
		commonUIDraw();

		//ポーズ画像
		TextureAsset(U"Pause").scaled(6).draw();

		//ボタン
		switch (gameScene->pauseState) {
		case GameScene::PauseState::GoBack:
			//三角
			Triangle(Vec2(Scene::Size().x / 5.0 * 1 + 30 - FontAsset(U"GameUI_BestTenDot30")(U"戻る").region().w / 2.0 - gameScene->PauseTriangleSize / 2.0 - 15, Scene::CenterF().y + 36 + 33 + 3), gameScene->PauseTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(gameScene->pauseTriangleTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));

			//FontAsset(U"GameUI_BestTenDot30")(U"ポーズ").drawAt(Scene::CenterF().movedBy(0, 33), activeColor);
			FontAsset(U"GameUI_BestTenDot30")(U"戻る").drawAt(Vec2(Scene::Size().x / 5.0 * 1 + 30 + 3.25, Scene::CenterF().y + 36 + 33 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"戻る").drawAt(Vec2(Scene::Size().x / 5.0 * 1 + 30, Scene::CenterF().y + 36 + 33), activeColor);

			FontAsset(U"GameUI_BestTenDot30")(U"リトライ").drawAt(Vec2(Scene::Size().x / 5.0 * 2, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"タイトル").drawAt(Vec2(Scene::Size().x / 5.0 * 3, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30 + 3, Scene::CenterF().y + 33 + 108 + 3 }, ColorF(0.3));
			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30    , Scene::CenterF().y + 33 + 108 }, ColorF(0.9));
			break;

		case GameScene::PauseState::Retry:
			//三角
			Triangle(Vec2(Scene::Size().x / 5.0 * 2 - FontAsset(U"GameUI_BestTenDot30")(U"リトライ").region().w / 2.0 - gameScene->PauseTriangleSize / 2.0 - 15, Scene::CenterF().y + 36 + 33 + 3), gameScene->PauseTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(gameScene->pauseTriangleTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));

			FontAsset(U"GameUI_BestTenDot30")(U"戻る").drawAt(Vec2(Scene::Size().x / 5.0 * 1 + 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"リトライ").drawAt(Vec2(Scene::Size().x / 5.0 * 2 + 3.25, Scene::CenterF().y + 36 + 33 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"リトライ").drawAt(Vec2(Scene::Size().x / 5.0 * 2, Scene::CenterF().y + 36 + 33), activeColor);

			FontAsset(U"GameUI_BestTenDot30")(U"タイトル").drawAt(Vec2(Scene::Size().x / 5.0 * 3, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30 + 3, Scene::CenterF().y + 33 + 108 + 3 }, ColorF(0.3));
			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30    , Scene::CenterF().y + 33 + 108 }, ColorF(0.9));
			break;

		case GameScene::PauseState::Title:
			//三角
			Triangle(Vec2(Scene::Size().x / 5.0 * 3 - FontAsset(U"GameUI_BestTenDot30")(U"タイトル").region().w / 2.0 - gameScene->PauseTriangleSize / 2.0 - 15, Scene::CenterF().y + 36 + 33 + 3), gameScene->PauseTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(gameScene->pauseTriangleTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));

			FontAsset(U"GameUI_BestTenDot30")(U"戻る").drawAt(Vec2(Scene::Size().x / 5.0 * 1 + 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"リトライ").drawAt(Vec2(Scene::Size().x / 5.0 * 2, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"タイトル").drawAt(Vec2(Scene::Size().x / 5.0 * 3 + 3.25, Scene::CenterF().y + 36 + 33 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"タイトル").drawAt(Vec2(Scene::Size().x / 5.0 * 3, Scene::CenterF().y + 36 + 33), activeColor);

			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30 + 3, Scene::CenterF().y + 33 + 108 + 3 }, ColorF(0.3));
			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30    , Scene::CenterF().y + 33 + 108 }, ColorF(0.9));
			break;

		case GameScene::PauseState::Config:
			//三角
			Triangle(Vec2(Scene::Size().x / 5.0 * 4 - 30 - FontAsset(U"GameUI_BestTenDot30")(U"設定").region().w / 2.0 - gameScene->PauseTriangleSize / 2.0 - 15, Scene::CenterF().y + 36 + 33 + 3), gameScene->PauseTriangleSize).rotated(90_deg).draw(ColorF(activeColor, (0.75 - Min(gameScene->pauseTriangleTimer * (4.0 / 5.0) * 0.75, 0.75) + 0.2)));

			FontAsset(U"GameUI_BestTenDot30")(U"戻る").drawAt(Vec2(Scene::Size().x / 5.0 * 1 + 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"リトライ").drawAt(Vec2(Scene::Size().x / 5.0 * 2, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"タイトル").drawAt(Vec2(Scene::Size().x / 5.0 * 3, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30 + 3.25, Scene::CenterF().y + 36 + 33 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30, Scene::CenterF().y + 36 + 33), activeColor);

			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30 + 3, Scene::CenterF().y + 33 + 108 + 3 }, ColorF(0.3));
			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30    , Scene::CenterF().y + 33 + 108 }, ColorF(0.9));
			break;

		case GameScene::PauseState::ConfigMode:
			FontAsset(U"GameUI_BestTenDot30")(U"戻る").drawAt(Vec2(Scene::Size().x / 5.0 * 1 + 30, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"リトライ").drawAt(Vec2(Scene::Size().x / 5.0 * 2 - 40, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"タイトル").drawAt(Vec2(Scene::Size().x / 5.0 * 3, Scene::CenterF().y + 36 + 33), inactiveColor2);

			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30 + 3.25, Scene::CenterF().y + 36 + 33 + 3.25), inactiveColor2);
			FontAsset(U"GameUI_BestTenDot30")(U"設定").drawAt(Vec2(Scene::Size().x / 5.0 * 4 - 30, Scene::CenterF().y + 36 + 33), activeColor);

			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30 + 3, Scene::CenterF().y + 33 + 108 + 3 }, ColorF(0.3));
			FontAsset(U"GameUI_BestTenDot")(U"Shiftで戻る").draw(Arg::bottomRight = Vec2{ Scene::Size().x - 30    , Scene::CenterF().y + 33 + 108 }, ColorF(0.9));

			configDraw();
			break;
		}
		break;
	}
}
