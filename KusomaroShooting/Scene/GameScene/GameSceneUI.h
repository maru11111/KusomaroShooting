#pragma once

class GameScene;

class GameSceneUI{
public:
	GameSceneUI(GameScene *gameScene_);

	~GameSceneUI()=default;

	void draw()const;

private:
	GameScene *gameScene;
};

