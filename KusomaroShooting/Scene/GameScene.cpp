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

struct DamageEffect : IEffect
{
	Vec2 pos;
	int n=0;
	double timer=0;

	// このコンストラクタ引数が、Effect::add<RingEffect>() の引数になる
	explicit DamageEffect(const Vec2& pos_)
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

		Print << pos;

		//trueの間継続
		return (n < 5);
	}
};

GameScene::GameScene(const InitData& init)
	: IScene{ init }
{
	objects.player = std::make_unique<Player>(objects);
}

void GameScene::drawBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(6).draw(backBarPosX, barPosY);
	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawStartPosX = frontBar.size().x - frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(barDrawStartPosX, 0, frontBar.size().x - barDrawStartPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12, barPosY);
}

void GameScene::drawBossBar(int currentNum, int maxNum, TextureAsset backBar, TextureAsset frontBar, int backBarPosX, int barPosY)const {
	//HPバーの後ろ部分を描画
	backBar.scaled(6).draw(backBarPosX, barPosY);
	//残りHP割合を計算
	const double remainingParcentage = (double)currentNum / (double)maxNum;
	//残りHP割合からHPバーのカットpxを計算
	const double barDrawEndPosX = frontBar.size().x * remainingParcentage;
	//HPバーの前部分を描画
	frontBar(0, 0, barDrawEndPosX, frontBar.size().y).scaled(6).draw(backBarPosX + 12 + (frontBar.size().x - barDrawEndPosX) * 6, barPosY);
	//Print << remainingParcentage;
}

void GameScene::drawMarshmallowUI() const {

	Rect{ 0,0,Scene::Size().x,40 }.draw(ColorF(0.9, 0.3, 0.3));

	TextureAsset(U"UIBack").scaled(6).draw();

	//HPバー
	drawBar(objects.player->getHp(), objects.player->getMaxHp(), TextureAsset(U"PlayerBarBack"), TextureAsset(U"PlayerHpFront"), 50*3, 6*3);

	//マシュマロバー
	drawBar(objects.player->getNumMarshmallows(), objects.player->getMaxNumMarshmallows(), TextureAsset(U"PlayerBarBack"), TextureAsset(U"MarshmallowBarFront"), 50*3, 26*3);

	//ボスのHPバー
	drawBossBar(1, 4, TextureAsset(U"BossBarBack"), TextureAsset(U"BossBarFront"), (320 - TextureAsset(U"BossBarBack").size().x * 2 - 16)*3, 26*3);

	//次のマシュマロを表示
	TextureAsset(U"MarshmallowBox").scaled(6).drawAt((320 / 2 - 5)*3, 24*3);
	switch (objects.player->getNextMaro()) {
	case MaroType::Normal: TextureAsset(U"Marshmallow").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3, 24*3); break;
	case MaroType::Up: TextureAsset(U"KusomaroUp").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3, 24 * 3); break;
	case MaroType::Down: TextureAsset(U"KusomaroDown").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3, 24*3); break;
	case MaroType::Sine: TextureAsset(U"KusomaroSine").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3, 24*3); break;
	case MaroType::Beam: TextureAsset(U"KusomaroBeam").scaled(6).rotated(-90_deg).drawAt((320 / 2 - 5)*3, 24*3); break;
	}


	{
		const ScopedRenderTarget2D renderTarget{ fontRenderTexture };

		//背景を透明に
		fontRenderTexture.clear(ColorF{ 1.0, 0.0 });

		//アルファ値を戻す
		const ScopedRenderStates2D renderState{ MaxAlphaBlend() };

		FontAsset(U"GameUI_Pixel")(U"HP").draw(165, 24);

		//if (objects.player->getNumMarshmallows() < 10) FontAsset(U"GameUI_Pixel")(U"Marshmallow\n    ", objects.player->getNumMarshmallows(), U"/30").drawAt(74, 93);
		/*else*/ FontAsset(U"GameUI_Pixel")(U"MP:", objects.player->getNumMarshmallows(), U"/30").draw(165, 85);

		FontAsset(U"GameUI_Pixel")(U"Next").draw(423, 11);

		FontAsset(U"GameUI_BestTenDot30")(U"Boss:***********").draw(540, 25);
		//FontAsset(U"GameUI_Pixel")(U"HP").draw(872, 84);
	}

}

void GameScene::update() {


	//Debug
	if (KeyE.down())objects.enemies << std::make_unique<GarbageBagNormal>(objects, Vec2{900, 300 });
	if (KeyR.down())objects.player->damage(1);
	if (KeyT.down())isTimeStopped = !isTimeStopped;

	//
	if (objects.player->getIsBeamAttacking()) {
		isTimeStopped = true;
	}
	else {
		isTimeStopped = false;
	}

	if (isTimeStopped) {
		//敵以外を更新
		objects.player->update();
		for (auto& maro : objects.marshmallows) {
			maro->update();
		}
	}
	else {
		//更新
		objects.player->update();
		for (auto& maro : objects.marshmallows) {
			maro->update();
		}
		for (auto& enemy : objects.enemies) {
			enemy->update();
		}
	}

	//当たり判定
	//プレイヤーと敵本体
	//プレイヤー直接攻撃と敵本体
	if (objects.player->getIsAttackColOn()) {
		for (auto& enemy : objects.enemies) {
			if (objects.player->attackCollision().intersects(enemy->collision())) {
				enemy->damage(objects.player->getDamageAmount());
			}
		}
	}
	//不要なオブジェクトを破壊
	destroyObjects();
	//プレイヤー本体と敵本体
	for (auto& enemy : objects.enemies) {
		if (objects.player->bodyCollision().intersects(enemy->collision())) {
			objects.player->damage(enemy->getDamageAmount());
		}
	}

	//マシュマロと敵
	for (auto& enemy : objects.enemies) {
		for (auto& maro : objects.marshmallows) {
			if (enemy->collision().intersects(maro->collision())) {
				//敵にヒットした判定をtrueに
				maro->setIsHit(true);
				//敵にダメージ
				enemy->damage(maro->getDamageAmount());
				//エフェクトを追加
				effect.add<DamageEffect>(enemy->getPos());
			}
		}
	}

	//画面外に行ったクソマロはもう一度boxに
	for (auto& maro : objects.marshmallows) {
		if (maro->isOffScreen() && (maro->getType()!=MaroType::Normal || maro->getType() != MaroType::Beam)) objects.player->setKusomaro(maro->getType());
	}

	//不要なオブジェクトを破壊
	destroyObjects();
}

void GameScene::destroyObjects() {
	objects.marshmallows.remove_if([](const auto& maro) { return maro->isDestroy(); });
	objects.enemies.remove_if([](const auto& enemy) {return enemy->isDestroy(); });
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

		//エフェクトを再生
		effect.update();
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
