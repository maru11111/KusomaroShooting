#pragma once
#include "Common.h"
#include "Objects.h"
#include "GameScene.h"
#include "SpawnEnemyData.h"

class GameScene : public App::Scene
{
public:
	GameScene(const InitData& init);

	void update()override;

	void stageUpdate();

	void collisionAndRemoveUpdate();

	virtual void spawnEnemy();

	void destroyObjects();

	void commonDraw()const;

	void draw()const override;

	Objects& getObj();

	bool isHitStopping = false;;
	double hitStopTime = 0.15;
	double hitStopTimer = 0;
	double drawTimer = 0;
	double damageUIEffectTimer = 0;
	bool isDamageUIEffectPlaying=false;
	int counterForSlowUpdate=0;

	void loadJson(String path)const;

	void drawHpBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase, double damageEase)const;
	void drawMaroBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, double healEase)const;
	void drawBossBar(double currentNum, double maxNum, TextureAsset backBar, TextureAsset frontBar, int posX, int posY, BaseBoss *boss)const;
	void drawMarshmallowUI()const;

private:
	//GameSceneのステート
	enum class GameState {
		Tutorial,
		Stage,
		BossAppear,
		BossBattle
	};
	GameState gameState = GameState::BossAppear;
	bool isChangeGameState=false;
	GameState nextState;
	double gameStateTimer=0;
	double easeTimer1 = 0;
	double easeTimer2 = 0;
	double easeTimer3 = 0;
	double easeTimer4 = 0;
	double easeTimer5 = 0;
	double easeTimer6 = 0;
	double easeTimer7 = 0;

	bool isHpAnimationStart = false;
	bool isHpAnimationEnd = false;
	mutable double easeBossHpAnimationTimer = 0;
	mutable bool isPlayHpAnimation = false;

	double ease=0;
	double maxTopRectHeight = 50 + TextureAsset(U"UIBack").size().y*6;
	double maxBottomRectHeight = 50;

	//BossAppearのステート
	enum class BossAppearState {
		HideUI,
		DrawRect,
		AppearBoss
	};
	BossAppearState bossAppearState = BossAppearState::HideUI;
	double marshmallowUIOffset = TextureAsset(U"UIBack").size().y * 6;
	double easeBossAppear=0;
	Camera2D camera{ Vec2{Scene::Center() + Vec2{0,-1}}, 1.0 , CameraControl::None_};
	
	RenderTexture renderTexture{ Size(320, 214)*3 };
	RenderTexture fontRenderTexture{ Size(320, 214) * 3 };
	Effect effect;
	bool isEnemyTimeStopped=false;

	//
	Array<String>kusomaroTexts=
	{
		U"俺がしぐれういだ！",
		U"たわわに実ったヒラメ",
		U"ういンカー「曲がるが？」",
		U"ういング「飛ぶが？」",
		U"簡単なこともわからないわ\n和菓子ってパンだっけ？",
		U"黄色いトマトはスイカ",
		U"イラストレーターのしぐれういです！\n今日は1万4千個のマシュマロに回答していきます！\n多分30分くらいで終わるかな？\n\n\n       こ             な             れ             う             い",
		U"きれいに咲いたしらたき",
		U"鋭利なみかん",
		U"トントントントン　カニのうどん",
	};
	Array<String>beamTexts =
	{
		U"R難しいかもしれないけれど\nR今、転職を考えております\nR美術系の仕事を探しているのですが\nRいい感じの\nRうなぎ",
		U"Rうい先生に質問です絵を描き始めるのに\nRいち万円で板タブを譲ってもらったのですが\nRびみょうに古いです\nRいまからでも新型を買った方がいいですか？\nRむだにならない値段も教えてほしいです",
		U"ういRうRいうい麦畑でね\n君とR日なたぼっこ　眠りましょ\n仲良Rい　良い　良いとかよしてよね\n夢はR夢でやっぱお断り",
		U"嫌になっちゃRうぐらい\n伝え切れなRいから引き続き\n凸凹コンRビネーションで\nso YES R/ NOならどーなの？\nNO 成R仏してくださーい！",
		U"Rうに食べたい\nこRいも食べたい\n甘エRビもいいな\nアルコRールと一緒に\n海鮮系のRムニエルで",
		U"Rうちの娘がうい先生のファンで\nRいつもかじりついて配信を見ていますｗ\n\nR美術の授業でもうい先生の絵を描いて\nR一番かわいくできたと持って帰って飾ってます\nR娘は本当はいません"
	};	

protected:
	Objects objects;
	mutable Array<SpawnEnemyData> spawnEnemyData;
};

