#pragma once
#include "GameScene.h"
#include "Common.h"

class GameSceneForEditor :public GameScene {
public:
	GameSceneForEditor(const InitData& init)
		: GameScene(init)
	{
		spawnEnemyData.clear();
		currentStage = Stage::Editor;
	}

	void startPlayMode()const {
		loadJson(U"stage/editorStage.json");
	}

	void setStartTime(double time)const {
		SpawnEnemyData::spawnStartTime = time;
		SpawnEnemyData::spawnTimer = SpawnEnemyData::spawnStartTime;
	}

	void spawnEnemy()override {
		SpawnEnemyData::spawnTimer += Scene::DeltaTime();

		for (int i = 0; i < spawnEnemyData.size(); i++) {
			if (spawnEnemyData[i].isSpawnableForEditor()) {
				switch (spawnEnemyData[i].type) {
					case EnemyType::Empty: /*何もしない*/; break;
					case EnemyType::Bag: objects.enemies << std::make_unique<GarbageBagNormal>(objects, spawnEnemyData[i].pos); break;
					case EnemyType::AccelBag: objects.enemies << std::make_unique<GarbageBagAccel>(objects, spawnEnemyData[i].pos); break;
					case EnemyType::SineBag: objects.enemies << std::make_unique<GarbageBagSine>(objects, spawnEnemyData[i].pos); break;
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
		spawnEnemyData.remove_if([](const SpawnEnemyData& spawnEnemyData) {return spawnEnemyData.isSpawnableForEditor(); });
	}

};
