#pragma once
#include "Common.h"
#include "Objects.h"
#include "GameSceneForEditor.h"
#include "SpawnEnemyData.h"

enum class Mode {
	EditMode,
	PlayMode
};

struct CellData {
public:
	CellData(EnemyType type_)
		: type{type_}
		, textureName{U"Empty"}
	{}
	EnemyType type;
	mutable String textureName;
	RectF rect;
	Vec2 centerPos;
	double time;
	ColorF rectColor;
};

class StageEditor : public App::Scene
{
public:
	StageEditor(const InitData& init);
	~StageEditor();
	void addGrids(int addGridNum);
	void update()override;
	void updateEditMode();
	void updatePlayMode();
	void updateEnemy()const;
	void toJson(String name, bool isForPlay)const;
	int getCurrentIntTime()const;
	void draw()const override;
	void drawEditMode()const;
	void drawPlayMode()const;
private:
	Objects& objects;

	mutable double timer = 0.0;
	double prevTime = 0;
	double maxTime = 100;
	int scale = 3;
	const Size SceneSize = { 320 * scale, 214 * scale };
	const double GridSize = (SceneSize.y - TextureAsset(U"UIBack").size().y * 3) / 8.0;
	//int row = (int)(SceneSize.x / GridSize)-5+1;
	//int col = (int)(SceneSize.y / GridSize)+4;
	int row = 22;
	int col = 12;
	Vec2 gridStartOffset = { (SceneSize.x - GridSize * row) / 2.0, - GridSize * 2 +(SceneSize.y - GridSize * col) / 2.0 };

	uint32 maxGridNum=maxTime+1;

	Array<Grid<CellData>>grids{ maxGridNum, Grid<CellData>((int)col, (int)row, CellData(EnemyType::Empty))};
	
	Camera2D camera{ Vec2{ 700, 200 }, 0.5 };

	ColorF normalColor=ColorF(1.0, 0.25);
	ColorF mouseOverColor=ColorF(1.0, 1.0, 1.0, 0.5);

	EnemyType currentSelectedType=EnemyType::Empty;

	mutable bool isPlaying=false;

	bool isNextTime=false;

	mutable Mode currentMode=Mode::EditMode;

	GameSceneForEditor gameScene;

	void loadEditJson(String path);

	mutable Array<SpawnEnemyData> spawnEnemyData;

	mutable bool isAfterChange=false;
};
