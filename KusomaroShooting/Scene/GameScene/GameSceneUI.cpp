#include "stdafx.h"
#include "GameSceneUI.h"
#include "GameScene.h"

GameSceneUI::GameSceneUI(GameScene *gameScene_)
	: gameScene(gameScene_)
{}

void GameSceneUI::draw()const {
	Print << U"UI_PRIIIIIIIIIIIIIINT";//DEBUG
}
