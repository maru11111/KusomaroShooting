#include "Common.h"
#include "Load.h"
#include "Title.h"
#include "GameScene.h"
#include "StageEditor.h"
#include "Result.h"

void Main()
{
	// 
	const int scaling = 3;
	//Scene::Resize(Size(320, 214) * scaling);

	// 最近傍補間
	const ScopedRenderStates2D renderState{ SamplerState::ClampNearest };

	// ウィンドウリサイズ
	Window::Resize(Size(320, 214)*3);

	//オーディオマネージャー
	AudioManager::Init();

	//タイトル変更
	Window::SetTitle(U"クソマロシューティング？");

	//ロード画面で使うフォントだけロード
	FontAsset::Register(U"GameUI_Kei", 20, Resource(U"font/keifont.ttf"));
	FontAsset::Load(U"GameUI_Kei");

	//escで終了しないように		
	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	//クレジット
	LicenseManager::AddLicense({
		.title = U"しぐれういのクソマロシューティング",
		.copyright = U"(C) 2025 ma-re"});

	LicenseManager::AddLicense({
	.title = U"プログラム、イラスト全般、一部BGM",
	.copyright = U"ma-re" });

	LicenseManager::AddLicense({
	.title = U"クリア画面のイラスト",
	.copyright = U"妹" });

	LicenseManager::AddLicense({
	.title = U"音楽、効果音",
	.copyright = U"イワシロ音楽素材(https://iwashiro-sounds.work/)\nザ・マッチメイカァズ(https://osabisi.sakura.ne.jp/m2/)\nOtoLogic(https://otologic.jp/)\nOn-Jin ～音人～(https://on-jin.com/)" });

	App manager;
	manager.add<Load>(State::Load);
	manager.add<Title>(State::Title);
	manager.add<GameScene>(State::Game);
	manager.add<StageEditor>(State::StageEditor);
	manager.add<Result>(State::Result);

	//-から開始
	manager.init(State::Load, 1s);

	//manager.init(State::Title, 0s);
	//manager.init(State::Game, 0s);
	//manager.init(State::StageEditor, 0s);
	//manager.init(State::Result, 0s);

	while (System::Update())
	{
		ClearPrint();
		{
			if (not manager.update())
			{
				break;
			}
		}
	}
}

//
// - Debug ビルド: プログラムの最適化を減らす代わりに、エラーやクラッシュ時に詳細な情報を得られます。
//
// - Release ビルド: 最大限の最適化でビルドします。
//
// - [デバッグ] メニュー → [デバッグの開始] でプログラムを実行すると、[出力] ウィンドウに詳細なログが表示され、エラーの原因を探せます。
//
// - Visual Studio を更新した直後は、プログラムのリビルド（[ビルド]メニュー → [ソリューションのリビルド]）が必要な場合があります。
//
