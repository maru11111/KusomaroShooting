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

		Print << pos;

		//trueの間継続
		return (n < 5);
	}
};

GameScene::GameScene(const InitData& init)
	: IScene{ init }
{
	objects.player = std::make_unique<Player>(objects);

	loadJson(U"stage/test.json");
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

	//UIの背景
	int n = (int)(Scene::Time() / 0.08) % 65;
	TextureAsset(U"UIBack")(n*TextureAsset(U"UIBack").size().x/65, 0, TextureAsset(U"UIBack").size().x/65.0, TextureAsset(U"UIBack").size().y).scaled(6).draw();

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


	//敵を追加
	spawnEnemy();

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
		/*enemy内でenemiesに要素を入れる場合があるので普通のforにする。
		  範囲forの場合だと走査中に要素が入れられたときにnullptrでエラー*/
		for (int i = 0; i < objects.enemies.size();i++) {
			objects.enemies[i]->update();
		}
	}

	//当たり判定
	//プレイヤーと敵本体
	//プレイヤー直接攻撃と敵本体
	if (objects.player->getIsAttackColOn()) {
		for (auto& enemy : objects.enemies) {
			if (enemy->collision().collision(objects.player->attackCollision())) {
				Print << U"damage!!!";
				enemy->damage(objects.player->getDamageAmount());
			}
		}
	}
	//プレイヤーの攻撃で倒された敵を破壊
	destroyObjects();
	//プレイヤー本体と敵本体
	for (auto& enemy : objects.enemies) {
		if (enemy->collision().collision(objects.player->bodyCollision())) {
			objects.player->damage(enemy->getDamageAmount());
		}
	}

	//マシュマロと敵
	for (auto& enemy : objects.enemies) {
		for (auto& maro : objects.marshmallows) {
			if (enemy->collision().collision(maro->collision())) {
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

void GameScene::spawnEnemy() {
	SpawnEnemyData::spawnTimer = Scene::DeltaTime();



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

void GameScene::draw() const {
	{
		const ScopedRenderTarget2D renderTarget{ renderTexture };

		//背景
		Scene::Rect()
			.draw(Arg::top = ColorF{ 0.2, 0.5, 1.0 }, Arg::bottom = ColorF{ 0.5, 0.8, 1.0 });
		const double farBackMountainPosX = -(int)(  Scene::Time() / 0.0390) % 320*3;
		const double backMountainPosX = -(int)(     Scene::Time() / 0.0370) % 320 * 3;
		const double middleMountainPosX= -(int)(Scene::Time() / 0.0350) % 320 * 3;
		const double frontMountainPosX = -(int)(Scene::Time() / 0.0330) % 320 * 3;
		const double frontCityPosX = -(int)( Scene::Time() / 0.0115)%320*3;
		const double middleCityPosX = -(int)(Scene::Time() / 0.0090) % 320 * 3;
		const double backCityPosX =  -(int)( Scene::Time() / 0.0065)%320*3;

		TextureAsset(U"BackGroundMountain").scaled(3).draw(farBackMountainPosX, 0);
		TextureAsset(U"BackGroundMountain").scaled(3).draw(Scene::Size().x + farBackMountainPosX, 0);
		TextureAsset(U"BackGroundMountainBack").scaled(3).draw(backMountainPosX, 0);
		TextureAsset(U"BackGroundMountainBack").scaled(3).draw(Scene::Size().x + backMountainPosX, 0);
		TextureAsset(U"BackGroundMountainMiddle").scaled(3).draw(middleMountainPosX, 0);
		TextureAsset(U"BackGroundMountainMiddle").scaled(3).draw(Scene::Size().x + middleMountainPosX, 0);
		TextureAsset(U"BackGroundMountainFront").scaled(3).draw(frontMountainPosX, 0);
		TextureAsset(U"BackGroundMountainFront").scaled(3).draw(Scene::Size().x + frontMountainPosX, 0);
		TextureAsset(U"BackGroundCityBack").scaled(3).draw(frontCityPosX, 0);
		TextureAsset(U"BackGroundCityBack").scaled(3).draw(Scene::Size().x+frontCityPosX, 0);
		TextureAsset(U"BackGroundCityMiddle").scaled(3).draw(middleCityPosX, 0);
		TextureAsset(U"BackGroundCityMiddle").scaled(3).draw(Scene::Size().x + middleCityPosX, 0);
		TextureAsset(U"BackGroundCity").scaled(3).draw(backCityPosX, 0);
		TextureAsset(U"BackGroundCity").scaled(3).draw(Scene::Size().x+backCityPosX, 0);

		//背景とオブジェクトの間
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
		//プレイヤー攻撃エフェクト
		objects.player->drawEffect();
		//gameSceneのエフェクト
		effect.update();


		//Debug
		//TextureAsset(U"Fish").draw(300,200);
		//スプライトシートを再生
		/*int n = (int)(Scene::Time() / 0.035) % 21;
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
