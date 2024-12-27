#include "stdafx.h"
#include "GameScene.h"

/// @brief 最大のアルファ値を書き込むブレンドステートを返します。
BlendState MaxAlphaBlend()
{
	BlendState blend = BlendState::Default2D;
	blend.opAlpha = BlendOp::Max;
	blend.dstAlpha = Blend::DestAlpha;
	blend.srcAlpha = Blend::SrcAlpha;
	return blend;
}

GameScene::GameScene(const InitData& init)
	: IScene{ init }
{
	objects.player = std::make_unique<Player>(objects);
}

void GameScene::drawBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(2).draw(backBarPosX, barPosY);
	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawStartPosX = frontBar.size().x - frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(barDrawStartPosX, 0, frontBar.size().x - barDrawStartPosX, frontBar.size().y).scaled(2).draw(backBarPosX + 4, barPosY);
}

void GameScene::drawBossBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(2).draw(backBarPosX, barPosY);
	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawEndPosX = frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(0, 0, barDrawEndPosX, frontBar.size().y).scaled(2).draw(backBarPosX + 4 + (frontBar.size().x - barDrawEndPosX) * 2, barPosY);
	Print << remainingParcentage;
}

void GameScene::drawMarshmallowUI() const {

	Rect{ 0,0,Scene::Size().x,40 }.draw(ColorF(0.9, 0.3, 0.3));

	TextureAsset(U"UIBack").scaled(2).draw();

	//HPバー
	drawBar(objects.player->getHp(), objects.player->getMaxHp(), TextureAsset(U"PlayerBarBack"), TextureAsset(U"PlayerHpFront"), 50, 6);

	//マシュマロバー
	drawBar(objects.player->getNumMarshmallows(), objects.player->getMaxNumMarshmallows(), TextureAsset(U"PlayerBarBack"), TextureAsset(U"MarshmallowBarFront"), 50, 26);

	//ボスのHPバー
	drawBossBar(1, 4, TextureAsset(U"BossBarBack"), TextureAsset(U"BossBarFront"), 320 - TextureAsset(U"BossBarBack").size().x * 2 - 16, 26);

	//次のマシュマロを表示
	TextureAsset(U"MarshmallowBox").scaled(2).drawAt(320 / 2 - 5, 24);
	switch (objects.player->getNextMaro()) {
	case MaroType::Normal: TextureAsset(U"Marshmallow").scaled(2).rotated(-90_deg).drawAt(320 / 2 - 5, 24); break;
	case MaroType::Up: TextureAsset(U"KusomaroUp").scaled(2).rotated(-90_deg).drawAt(320 / 2 - 5, 24); break;
	case MaroType::Down: TextureAsset(U"KusomaroDown").scaled(2).rotated(-90_deg).drawAt(320 / 2 - 5, 24); break;
	case MaroType::Sine: TextureAsset(U"KusomaroSine").scaled(2).rotated(-90_deg).drawAt(320 / 2 - 5, 24); break;
	case MaroType::Beem: TextureAsset(U"KusomaroBeem").scaled(2).rotated(-90_deg).drawAt(320 / 2 - 5, 24); break;
	}


	{
		const ScopedRenderTarget2D renderTarget{ fontRenderTexture };

		//背景を透明に
		fontRenderTexture.clear(ColorF{ 1.0, 0.0 });

		//アルファ値を戻す
		const ScopedRenderStates2D renderState{ MaxAlphaBlend() };

		FontAsset(U"GameUI_Pixel")(U"HP").draw(120, 24);

		if (objects.player->getNumMarshmallows() < 10) FontAsset(U"GameUI_Pixel")(U"Marshmallow\n    ", objects.player->getNumMarshmallows(), U"/30").drawAt(74, 93);
		else FontAsset(U"GameUI_Pixel")(U"Marshmallow\n   ", objects.player->getNumMarshmallows(), U"/30").drawAt(74, 93);

		FontAsset(U"GameUI_Pixel")(U"Next").draw(423, 11);

		FontAsset(U"GameUI_BestTenDot30")(U"Boss:微炭酸ペットボトラーズ").draw(540, 25);
		FontAsset(U"GameUI_Pixel")(U"HP").draw(918, 84);
	}

}

void GameScene::update() {

	//Debug
	if (KeyE.down())objects.enemies << std::make_unique<GarbageBag>(objects, Vec2{ 800, 540 / 2 });
	if (KeyR.down())objects.player->damage(1);

	//更新
	objects.player->update();
	for (auto& maro : objects.marshmallows) {
		maro->update();
	}
	for (auto& enemy : objects.enemies) {
		enemy->update();
	}

	//当たり判定
	//プレイヤーと敵本体
	for (auto& enemy : objects.enemies) {
		if (objects.player->collision().intersects(enemy->collision()))Print << U"プレイヤーにダメージ！";
	}

	//マシュマロと敵
	for (auto& enemy : objects.enemies) {
		for (auto& maro : objects.marshmallows) {
			if (enemy->collision().intersects(maro->collision()))Print << U"マシュマロが敵にダメージ！";
		}
	}

	//不要なオブジェクトを破壊
	objects.marshmallows.remove_if([](const auto& maro) { return maro->isDestroy(); });
	//objects.enemies.remove_if([](const auto& enemy) {return enemy->isDestroy(); });
}

void GameScene::draw() const {
	{
		const ScopedRenderTarget2D renderTarget{ renderTexture };
		//背景
		Scene::Rect()
			.draw(Arg::top = ColorF{ 0.2, 0.5, 1.0 }, Arg::bottom = ColorF{ 0.5, 0.8, 1.0 });

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
	}

	// シーン転送時の拡大縮小方法を最近傍法にする
	Scene::SetTextureFilter(TextureFilter::Nearest);
	//文字以外のレンダーテクスチャをdraw
	renderTexture.scaled(3).draw();

	// シーン転送時の拡大縮小方法をバイリニア法にする
	Scene::SetTextureFilter(TextureFilter::Nearest);
	//文字のレンダーテクスチャをdraw
	fontRenderTexture.draw();
}
