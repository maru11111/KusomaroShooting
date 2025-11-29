#pragma once
#include "Common.h"
struct SpawnEnemyData {
public:
	int time;
	Vec2 pos;
	EnemyType type;
	static double spawnTimer;
	static double spawnStartTime;

	bool isSpawnableForEditor()const {
		//出現時間が試運転開始時間より後 かつ 出現時間のとき
		if (spawnStartTime <= time && time <= spawnTimer) return true;
		else return false;
	}

	bool isSpawnable()const {
		if(time <= spawnTimer) return true;
		else return false;
	}
};
