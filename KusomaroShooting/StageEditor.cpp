﻿#include "stdafx.h"
#include "StageEditor.h"

StageEditor::StageEditor(const InitData& init)
	: IScene{ init }
	, gameScene(init)
	, objects(gameScene.getObj())
{
	//プレイヤー作成
	objects.player = std::make_unique<Player>(objects, Vec2(100, Scene::Size().y/2.0));
	//初期化
	for (int i = 0; i < maxGridNum; i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				if (k <= 1) {
					grids[i][j][k].rect = RectF{ gridStartOffset.x * 2.0 + j * GridSize, GridSize * 2 + gridStartOffset.y + k * GridSize, GridSize, GridSize };
					grids[i][j][k].centerPos = { (grids[i][j][k].rect.x + grids[i][j][k].rect.w / 2.0), (grids[i][j][k].rect.y + grids[i][j][k].rect.h / 2.0) };
					grids[i][j][k].time = i;
				}
				else {
					grids[i][j][k].rect = RectF{ Scene::Size().x + j * GridSize, gridStartOffset.y + k * GridSize, GridSize, GridSize };
					grids[i][j][k].centerPos = { (grids[i][j][k].rect.x + grids[i][j][k].rect.w / 2.0), (grids[i][j][k].rect.y + grids[i][j][k].rect.h / 2.0) };
					grids[i][j][k].time = i;
				}
			}
		}
	}
}

StageEditor::~StageEditor() {
	//編集後に保存していなければ
	if (isAfterChange) {
		Optional<FilePath> path = Dialog::SaveFile({ FileFilter::JSON() });;
		if (path.has_value()) toJson(*path);
	}
}

void StageEditor::addGrids(int addGridNum) {
	int prevMaxGridNum = maxGridNum;
	maxGridNum += addGridNum;
	maxTime += addGridNum;
	Array<Grid<CellData>> tmp{ (uint64)maxGridNum, Grid<CellData>((int)col, (int)row, CellData(EnemyType::Empty)) };

	//tmpの位置を初期化
	for (int i = 0; i < maxGridNum; i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				if (k <= 1) {
					tmp[i][j][k].rect = RectF{ gridStartOffset.x * 2.0 + j * GridSize, GridSize * 2 + gridStartOffset.y + k * GridSize, GridSize, GridSize };
					tmp[i][j][k].centerPos = { (tmp[i][j][k].rect.x + tmp[i][j][k].rect.w / 2.0), (tmp[i][j][k].rect.y + tmp[i][j][k].rect.h / 2.0) };
					tmp[i][j][k].time = i;
				}
				else {
					tmp[i][j][k].rect = RectF{ Scene::Size().x + j * GridSize, gridStartOffset.y + k * GridSize, GridSize, GridSize };
					tmp[i][j][k].centerPos = { (tmp[i][j][k].rect.x + tmp[i][j][k].rect.w / 2.0), (tmp[i][j][k].rect.y + tmp[i][j][k].rect.h / 2.0) };
					tmp[i][j][k].time = i;
				}
			}
		}
	}

	//元のグリッドの内容をコピー
	for (int i = 0; i < prevMaxGridNum; i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				tmp[i][j][k].type = grids[i][j][k].type;
				tmp[i][j][k].textureName = grids[i][j][k].textureName;
			}
		}
	}

	//グリッドを新しいものに変える
	grids = tmp;
}

void StageEditor::update() {

	ClearPrint();

	//編集モードと試運転モード場合分け
	switch (currentMode)
	{
		case Mode::PlayMode:
			updatePlayMode();
			break;
		case Mode::EditMode:
			updateEditMode();
			break;
	}
}

void StageEditor::updateEditMode() {
	Print << U"\n";

	//Print << U"timer:" << timer;
	//Print << U"static_cast<int>(Parse<double>(Format(timer))):" << static_cast<int>(Parse<double>(Format(timer)));
	//Print << U"static_cast<int>(timer):" << static_cast<int>(timer);

	//編集するステージをロード
	if (KeyL.down()) {
		Optional<FilePath> path = Dialog::OpenFile({ FileFilter::JSON() }, { FileSystem::CurrentDirectory() + U"stage" });
		if (path.has_value()) loadEditJson(*path);
	}

	//現在のステージをjsonに出力
	if (KeyJ.down()) {
		Optional<FilePath> path = Dialog::SaveFile({ FileFilter::JSON() });;
		if (path.has_value()) {
			toJson(*path);
		}
	}

	//敵のデータをリセット
	if (KeyR.pressed() && KeyLShift.pressed()) {
		//グリッドを初期化
		for (int i = 0; i < grids.size(); i++) {
			for (int j = 0; j < row; j++) {
				for (int k = 0; k < col; k++) {
					grids[i][j][k].type = EnemyType::Empty;
					grids[i][j][k].textureName = U"Empty";
				}
			}
		}
	}

	//時間を増やす
	if (KeyT.down()) {
		addGrids(1);
	}

	//敵を選択
	if (Key1.down()) currentSelectedType = EnemyType::Empty;
	if (Key2.down()) currentSelectedType = EnemyType::Bag;
	if (Key3.down()) currentSelectedType = EnemyType::FastBag;
	if (Key4.down()) currentSelectedType = EnemyType::BagWithCan;
	if (Key5.down()) currentSelectedType = EnemyType::Can;
	if (Key6.down()) currentSelectedType = EnemyType::Fish;
	if (Key7.down()) currentSelectedType = EnemyType::Umbrella;
	

	//選択中の敵を表示
	switch (currentSelectedType) {
	case EnemyType::Empty: Print << U"現在の敵:" << U"Empty"; break;
	case EnemyType::Bag: Print << U"現在の敵:" << U"Bag"; break;
	case EnemyType::FastBag: Print << U"現在の敵:" << U"FastBag"; break;
	case EnemyType::BagWithCan: Print << U"現在の敵:" << U"BagWithCan"; break;
	case EnemyType::Can: Print << U"現在の敵:" << U"Can"; break;
	case EnemyType::Fish: Print << U"現在の敵:" << U"Fish"; break;
	case EnemyType::Umbrella: Print << U"現在の敵:" << U"Umbrella"; break;
	}

	// 2D カメラを更新
	camera.update();

	// カメラ座標系の範囲で処理
	const auto t = camera.createTransformer();

	//タイマーが更新されていたら
	if (prevTime != timer)
	{
		//敵を再シミュレーション
		updateEnemy();
	}

	//現在のグリッド更新
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			//フレームの上にマウスがあったら色を変える
			if (grids[getCurrentIntTime()][i][j].rect.mouseOver()) grids[getCurrentIntTime()][i][j].rectColor = mouseOverColor;
			else grids[getCurrentIntTime()][i][j].rectColor = normalColor;

			//フレームをクリックしたら
			if (grids[getCurrentIntTime()][i][j].rect.leftPressed()) {
				//選択中の敵をセット
				switch (currentSelectedType) {
				case EnemyType::Empty:
					grids[getCurrentIntTime()][i][j].type = EnemyType::Empty;
					grids[getCurrentIntTime()][i][j].textureName = U"Empty";
					break;
				case EnemyType::Bag:
					grids[getCurrentIntTime()][i][j].type = EnemyType::Bag;
					grids[getCurrentIntTime()][i][j].textureName = U"GarbageBag";
					break;
				case EnemyType::FastBag:
					grids[getCurrentIntTime()][i][j].type = EnemyType::FastBag;
					grids[getCurrentIntTime()][i][j].textureName = U"GarbageBagFastForEditor";
					break;
				case EnemyType::BagWithCan:
					grids[getCurrentIntTime()][i][j].type = EnemyType::BagWithCan;
					grids[getCurrentIntTime()][i][j].textureName = U"GarbageBagWithCanForEditor";
					break;
				case EnemyType::Can:
					grids[getCurrentIntTime()][i][j].type = EnemyType::Can;
					grids[getCurrentIntTime()][i][j].textureName = U"RedCan";
					break;
				case EnemyType::Fish:
					grids[getCurrentIntTime()][i][j].type = EnemyType::Fish;
					grids[getCurrentIntTime()][i][j].textureName = U"FishForEditor";
					break;
				case EnemyType::Umbrella:
					grids[getCurrentIntTime()][i][j].type = EnemyType::Umbrella;
					grids[getCurrentIntTime()][i][j].textureName = U"Umbrella";
					break;
				}

				//save機能のための編集後フラグ
				if(currentSelectedType != EnemyType::Empty) isAfterChange = true;
			}

			//右クリックでスポイト
			//フレームをクリックしたら
			if (grids[getCurrentIntTime()][i][j].rect.rightPressed()) {
				//選択した敵をスポイト
				switch (grids[getCurrentIntTime()][i][j].type) {
				case EnemyType::Empty: currentSelectedType = EnemyType::Empty;break;
				case EnemyType::Bag: currentSelectedType = EnemyType::Bag;break;
				case EnemyType::FastBag: currentSelectedType = EnemyType::FastBag; break;
				case EnemyType::BagWithCan: currentSelectedType = EnemyType::BagWithCan; break;
				case EnemyType::Can: currentSelectedType = EnemyType::Can; break;
				case EnemyType::Fish: currentSelectedType = EnemyType::Fish; break;
				case EnemyType::Umbrella: currentSelectedType = EnemyType::Umbrella; break;
				}
			}

		}
	}

	//時間が前のフレームから変わったか調べるために保存
	prevTime = timer;
}

void StageEditor::updatePlayMode() {
	gameScene.update();
}


int doubleToInt(double d) {
	return static_cast<int>(Parse<double>(Format(d)));
}

void ::StageEditor::updateEnemy()const {

	//すべての敵を破棄
	objects.enemies.clear();

	//0秒のグリッドからdoubleToTimer(timer)秒のグリッドまで敵を動かす(よくない)
	for (int i = 0; i < timer; i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				//敵を追加
				switch (grids[i][j][k].type) {
				case EnemyType::Empty: /*何もしない*/; break;
				case EnemyType::Bag: objects.enemies << std::make_unique<GarbageBagNormal>(objects, grids[i][j][k].centerPos); break;
				case EnemyType::FastBag: objects.enemies << std::make_unique<GarbageBagFast>(objects, grids[i][j][k].centerPos); break;
				case EnemyType::BagWithCan: objects.enemies << std::make_unique<GarbageBagWithCan>(objects, grids[i][j][k].centerPos); break;
				case EnemyType::Can: objects.enemies << std::make_unique<Can>(objects, grids[i][j][k].centerPos, Vec2{ -1, 0 }); break;
				case EnemyType::Fish: objects.enemies << std::make_unique<Fish>(objects, grids[i][j][k].centerPos); break;
				case EnemyType::Umbrella: objects.enemies << std::make_unique<Umbrella>(objects, grids[i][j][k].centerPos); break;
				}
			}
		}
		//敵を移動
		//1秒移動している場合
		if (getCurrentIntTime() != i) {
			//update()を呼ぶ回数を計算(一秒間に呼ばれる回数だけ呼ぶ)
			int n = 0;
			while (Scene::DeltaTime() * (double)n < 1.0) {
				n++;
			}
			n--;
			//updateを1秒に呼ばれる回数だけ呼ぶ
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < objects.enemies.size(); k++) {
					objects.enemies[k]->update();
				}
			}
			//画面外に行った敵を削除
			objects.enemies.remove_if([](const auto& enemy) {return enemy->isDestroy(); });
		}
		//小数点以下秒の移動の場合
		else {
			int n = 0;
			while (Scene::DeltaTime() * (double)n < timer - getCurrentIntTime()) {
				n++;
			}
			n--;
			//updateを timer-getCurrentIntTime() 秒に呼ばれる回数だけ呼ぶ
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < objects.enemies.size(); k++) {
					objects.enemies[k]->update();
				}
			}
		}
	}
}

//jsonに書き出し
void StageEditor::toJson(String name)const {

	JSON json;
	Array<JSON>enemies;

	for (int i = 0; i < maxGridNum; i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				if (grids[i][j][k].type != EnemyType::Empty) {
					JSON enemy;
					enemy[U"Time"] = grids[i][j][k].time;
					String name;
					switch (grids[i][j][k].type) {
						case EnemyType::Bag: name = U"Bag"; break;
						case EnemyType::FastBag: name = U"FastBag"; break;
						case EnemyType::BagWithCan: name = U"BagWithCan"; break;
						case EnemyType::Can: name = U"Can"; break;
						case EnemyType::Fish: name = U"Fish"; break;
						case EnemyType::Umbrella: name = U"Umbrella"; break;
					}
					enemy[U"Type"] = name;
					enemy[U"Pos"] = grids[i][j][k].centerPos;

					enemies << enemy;
				}
			}
		}
	}
	json[U"Enemies"] = enemies;

	json.save(name);

	isAfterChange = false;
}

//doubleをそのままintにキャストすると誤差が出るのでString→double→intに変換して使う
int StageEditor::getCurrentIntTime()const {
	return static_cast<int>(Parse<double>(Format(timer)));
}

void StageEditor::draw()const {

	//編集モードと試運転モード場合分け
	switch (currentMode)
	{
	case Mode::EditMode:
		drawEditMode();
		break;
	case Mode::PlayMode:
		drawPlayMode();
		break;
	}

	//共通部分描画
	//再生ボタン
	if (currentMode == Mode::EditMode) {
		if (SimpleGUI::Button(U"▶", Vec2{ Scene::Size().x - 71, 50 })) {
			currentMode = Mode::PlayMode;
			toJson(U"stage/editorStage.json");
			gameScene.startPlayMode();
			gameScene.setStartTime(timer);
		}
	}
	else {
		if (SimpleGUI::Button(U"■", Vec2{ Scene::Size().x - 71, 50 })) {
			currentMode = Mode::EditMode;
			updateEnemy();
		}
	}
	
}

void StageEditor::drawEditMode()const {
	{
		// 2D カメラの設定から Transformer2D を作成
		const auto t = camera.createTransformer();

		Scene::SetBackground(Palette::Lightslategray);

		//ゲーム内背景
		Scene::Rect()
			.draw(Arg::top = ColorF{ 0.2, 0.5, 1.0 }, Arg::bottom = ColorF{ 0.5, 0.8, 1.0 });

		//UIの背景
		int n = (int)(Scene::Time() / 0.08) % 65;
		TextureAsset(U"UIBack")
			(n * TextureAsset(U"UIBack").size().x / 65, 0, TextureAsset(U"UIBack").size().x / 65.0, TextureAsset(U"UIBack").size().y)
			.scaled(6)
			.draw(ColorF(1.0, 0.5));

		//グリッド描画
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				//セル描画
				if (grids[getCurrentIntTime()][i][j].textureName != U"Empty") {
					grids[getCurrentIntTime()][i][j].rect(TextureAsset(grids[getCurrentIntTime()][i][j].textureName)).draw(grids[getCurrentIntTime()][i][j].rectColor).drawFrame(3, 0);
				}
				else {
					grids[getCurrentIntTime()][i][j].rect.draw(grids[getCurrentIntTime()][i][j].rectColor).drawFrame(3, 0);
				}
			}
		}

		objects.player->draw();

		//敵描画
		for (auto& enemy : objects.enemies) {
			enemy->draw();
		}
	}

	//時間調整ボタン
	// -0.1ボタン
	if (SimpleGUI::Button(U"<", Vec2{ 73, 10 })) {
		//小数点第二位以下が0なら
		if (timer == (static_cast<int>(Parse<double>(Format((timer * 10))))) / 10.0){
			//そのまま -0.1 する
			timer -= 0.1;
		}
		else {
			//小数点第二位以下を0にする
			timer = (static_cast<int>(Parse<double>(Format((timer * 10))))) / 10.0;
		}
	}
	// -1ボタン
	if (SimpleGUI::Button(U"<<", Vec2{ 5, 10 })) {
		//小数点なし
		if ((Math::Fmod(timer, 1)) == 0) {
			timer--;
		}
		//小数点あり
		else {
			//小数点第一位以下を0に
			timer = getCurrentIntTime();
		}
	}
	// +0.1ボタン
	if (SimpleGUI::Button(U">", Vec2{ Scene::Size().x - 128, 10 })) {
		timer += 0.1;
		//小数点第二位以下を0にする
		timer = (static_cast<int>(Parse<double>(Format((timer * 10))))) / 10.0;
	}
	// +1ボタン
	if (SimpleGUI::Button(U">>", Vec2{ Scene::Size().x - 72, 10 })) {
		//小数点なし
		if ((Math::Fmod(timer, 1)) == 0) {
			timer++;
		}
		//小数点あり
		else {
			//小数点第一位以下を0に
			timer = getCurrentIntTime() + 1;
		}
	}
	//時間正規化
	if (timer < 0) timer = 0;
	if (maxTime < timer) timer = maxTime;
	//if ((Math::Fmod(timer, 1)) != 0) timer = getCurrentIntTime();

	//時間のスライダー
	SimpleGUI::Slider(U"時間:{:.2f}"_fmt(timer), timer, 0.0, maxTime, Vec2{ 130, 10 }, 100, Scene::Size().x - 365);

	//時間正規化
	if (timer < 0) timer = 0;
	if (maxTime < timer) timer = maxTime;
	//if ((Math::Fmod(timer, 1)) != 0) timer = getCurrentIntTime();
}

void StageEditor::loadEditJson(String path) {
	JSON json = JSON::Load(path);
	if (not json) throw Error{ path + U"がありません" };
	//jsonのデータを SpawnEnemyDataに移す
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

	//グリッドを初期化
	for (int i = 0; i < grids.size(); i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				grids[i][j][k].type = EnemyType::Empty;
				grids[i][j][k].textureName = U"Empty";
			}
		}
	}

	//グリッドにデータを格納
	for (int i = 0; i < spawnEnemyData.size(); i++) {
		int enemyRow = 0;
		int enemyCol = 0;
		for (int j = 0; j < row; j++){
			for (int k = 0; k < col; k++) {
				//敵の位置に対応するグリッドのインデックスを求める
				if (grids[0][j][k].centerPos == spawnEnemyData[i].pos) {
					enemyRow = j;
					enemyCol = k;
				}
			}
		}
		grids[spawnEnemyData[i].time][enemyRow][enemyCol].type= spawnEnemyData[i].type;
		switch (spawnEnemyData[i].type) {
		case EnemyType::Empty:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"Empty";
			break;
		case EnemyType::Bag:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"GarbageBag";
			break;
		case EnemyType::FastBag:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"GarbageBagFastForEditor";
			break;
		case EnemyType::BagWithCan:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"GarbageBagWithCanForEditor";
			break;
		case EnemyType::Can:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"RedCan";
			break;
		case EnemyType::Fish:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"FishForEditor";
			break;
		case EnemyType::Umbrella:
			grids[spawnEnemyData[i].time][enemyRow][enemyCol].textureName = U"Umbrella";
			break;
		}
	}
}

void StageEditor::drawPlayMode()const {
	gameScene.draw();
}
