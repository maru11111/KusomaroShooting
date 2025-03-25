#include "stdafx.h"
#include "Result.h"

Result::Result(const InitData& init)
	: IScene{ init }
{
	//if (getData().isNoContinueClear) {
		resultState = ResultState::RegisterOrNot;
	//}
}

Result::~Result() {
	LeaderBoard::Instance()->reset();
}

void Result::update() {
	switch (state) {
	case MovieState::Movie:
		if (resultTimer==0) AudioManager::Instance()->play(U"ClearMovieAudio", 2s);
		resultTimer += Scene::DeltaTime();
		if (14.0 <= resultTimer /*not AudioAsset(U"ClearMovieAudio").isPlaying()*/) {
			state = MovieState::Still;
			resultTimer = 0;
		}
		//スキップ入力受付
		if (KeyEnter.pressed() || KeyZ.down() || KeyN.down() || MouseL.pressed()) {
			skipAnimTimer += Scene::DeltaTime();
		}
		else {
			skipAnimTimer = 0;
		}
		//スキップ
		if (1.0 < skipAnimTimer) {
			state = MovieState::Still;
			resultTimer = 2.0;
			if (AudioAsset(U"ClearMovieAudio").isPlaying())
				AudioManager::Instance()->stop(U"ClearMovieAudio", 0s);
			opacity = 1.0;
		}
		break;

	case MovieState::Still: 
		if (resultTimer == 0) AudioManager::Instance()->play(U"ResultWindBlow", 2s);
		resultTimer += Scene::DeltaTime();
		opacity = std::lerp(0.0, 1.0, EaseInExpo(Min(resultTimer/2.0, 1.0)));
		if (KeyEnter.down()) {
			AudioManager::Instance()->playOneShot(U"Select");
			state = MovieState::ToResult;
			resultTimer = 0;
			opacity = 0.0;
		}
		break;

	case MovieState::ToResult:
		resultTimer += Scene::DeltaTime();
		opacity = std::lerp(0.0, 1.0, EaseOutExpo(Min(resultTimer, 1.0)));
		if (KeyEnter.down()) {
			AudioManager::Instance()->playOneShot(U"Select");
			state = MovieState::Result;
			resultTimer = 0;
			opacity = 0.0;
		}
		break;

	case MovieState::Result:
		resultTimer += Scene::DeltaTime();
		switch (resultState) {
		case ResultState::RegisterOrNot:
			opacity = std::lerp(0.0, 1.0, EaseOutExpo(Min(resultTimer, 1.0)) );

			switch (stillButton) {
			case RegisterOrNotButton::Yes:
				if (KeyD.down() || KeyRight.down()) {
					AudioManager::Instance()->playOneShot(U"ChangeButton");
					stillButton = RegisterOrNotButton::No;
				}
				if (KeySpace.down() || KeyEnter.down()) {
					AudioManager::Instance()->playOneShot(U"Select");
					resultState = ResultState::Register;
					LeaderBoard::Instance()->setState(LeaderBoard::State::BeforeInput);
					LeaderBoard::Instance()->init(getData().lastClearScore);
					opacity = 0.0;
					resultTimer = 0;
				}
				break;

			case RegisterOrNotButton::No:
				if (KeyA.down() || KeyLeft.down()) {
					AudioManager::Instance()->playOneShot(U"ChangeButton");
					stillButton = RegisterOrNotButton::Yes;
				}
				if (KeySpace.down() || KeyEnter.down()) {
					AudioManager::Instance()->playOneShot(U"Cancel");
					resultState = ResultState::Normal;
					opacity = 0.0;
					resultTimer = 0;
				}
				break;
			}
			break;

		case ResultState::Register:
			LeaderBoard::Instance()->update();
			if (LeaderBoard::Instance()->isEndRegister()) {
				resultState = ResultState::ViewRanking;
			}
			break;

		case ResultState::ViewRanking:
			LeaderBoard::Instance()->update();
			if (Rect(Scene::Size().x - 20 - TextureAsset(U"Batu").size().x * 9, 0 + 20, TextureAsset(U"Batu").size().x * 9).leftClicked()) {
				resultState = ResultState::Normal;
			}
			break;

		case ResultState::Normal:
			if (Rect(Scene::Size().x - 20 - TextureAsset(U"Trophy").size().x * 9, 0 + 20, TextureAsset(U"Trophy").size().x * 9).leftClicked()) {
				resultState = ResultState::ViewRanking;
			}
			if (KeyEnter.down()) {
				//Print << U"未実装";
				AudioManager::Instance()->playOneShot(U"Select");
				AudioAsset(U"ResultWindBlow").stop();
				changeScene(State::Title, 1.0s);
			}
			break;
		}

		break;
	}
}

void Result::draw()const {
	switch (state) {
	case MovieState::Movie:
		Rect(0, 0, Scene::Size()).draw(ColorF(0.0));
		//スキップ決定アニメーション
		Circle{ Scene::Size().x-30, 0+30, 10}.drawArc(0_deg, 360_deg * (skipAnimTimer / 1.0), 0, 5, ColorF(0.9));
		if (skipAnimTimer != 0)FontAsset(U"GameUI_Kei")(U"長押しでスキップ").draw(Arg::topRight = Vec2{Scene::Size().x-30-20-5, +20});
		break;

	case MovieState::Still:
		Rect(0, 0, Scene::Size()).draw(ColorF(0.0));
		TextureAsset(U"ClearStill").scaled(0.5).draw(0, 0, ColorF(1.0, opacity));
		if(opacity==1)FontAsset(U"GameUI_BestTenDot45")(U"Enter to Next").drawAt(Scene::CenterF().movedBy(0, 250), ColorF(0.85, 1 - Periodic::Sawtooth0_1(1.5s)));
		break;

	case MovieState::ToResult:
		TextureAsset(U"ClearStill").scaled(0.5).draw(0, 0);
		RectF(0, 0, Scene::Size().x, Scene::Size().y).draw(UIBacColor);
		FontAsset(U"GameUI_BestTenDot90")(U"Game Clear").drawAt(Scene::CenterF().movedBy(0, -100), ColorF(1.0, opacity));
		FontAsset(U"GameUI_BestTenDot60")(U"Thank you for playing!").drawAt(Scene::CenterF().movedBy(0, 100), ColorF(1.0, opacity));
		FontAsset(U"GameUI_BestTenDot45")(U"Enter to Next").drawAt(Scene::CenterF().movedBy(0, 250), ColorF(0.85, 1 - Periodic::Sawtooth0_1(1.5s)));
		break;

	case MovieState::Result:
		switch (resultState) {
		case ResultState::RegisterOrNot:
			TextureAsset(U"ClearStill").scaled(0.5).draw(0, 0);
			RectF(0, 0, Scene::Size().x, Scene::Size().y).draw(UIBacColor);
			FontAsset(U"GameUI_BestTenDot45")(U"スコアをランキングに登録しますか？").drawAt(Scene::CenterF().movedBy(0, 0 - 50), ColorF(1.0, opacity));
			FontAsset(U"GameUI_BestTenDot")(U"※登録した情報は全世界のユーザーに公開されます。").drawAt(Scene::CenterF().movedBy(0, 70 - 50), ColorF(0.90, opacity));

			switch (stillButton) {
			case RegisterOrNotButton::Yes:
				Triangle(Scene::CenterF().movedBy(-Scene::Size().x / 4.0 - 40, Scene::Size().y / 4.0 + 2.5), 35).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
				FontAsset(U"GameUI_BestTenDot45")(U"はい").drawAt(Scene::CenterF().movedBy(-Scene::Size().x / 4.0 + 40, Scene::Size().y / 4.0), ColorF(activeColor, opacity));
				FontAsset(U"GameUI_BestTenDot45")(U"いいえ").drawAt(Scene::CenterF().movedBy(+Scene::Size().x / 4.0 - 40, Scene::Size().y / 4.0), ColorF(inactiveColor, opacity));
				break;

			case RegisterOrNotButton::No:
				Triangle(Scene::CenterF().movedBy(+Scene::Size().x / 4.0 - 140, Scene::Size().y / 4.0 + 2.5), 35).rotated(90_deg).draw(ColorF(activeColor, 1 - Periodic::Sawtooth0_1(1.25s)));
				FontAsset(U"GameUI_BestTenDot45")(U"はい").drawAt(Scene::CenterF().movedBy(-Scene::Size().x / 4.0 + 40, Scene::Size().y / 4.0), ColorF(inactiveColor, opacity));
				FontAsset(U"GameUI_BestTenDot45")(U"いいえ").drawAt(Scene::CenterF().movedBy(+Scene::Size().x / 4.0 - 40, Scene::Size().y / 4.0), ColorF(activeColor, opacity));
				break;
			}
			break;

		case ResultState::Register:
			TextureAsset(U"ClearStill").scaled(0.5).draw(0, 0);
			RectF(0, 0, Scene::Size().x, Scene::Size().y).draw(UIBacColor);
			LeaderBoard::Instance()->draw(Scene::CenterF());
			break;

		case ResultState::ViewRanking:
			TextureAsset(U"ClearStill").scaled(0.5).draw(0, 0);
			RectF(0, 0, Scene::Size().x, Scene::Size().y).draw(UIBacColor);
			LeaderBoard::Instance()->draw(Scene::CenterF());
			if (Rect(Scene::Size().x - 20 - TextureAsset(U"Batu").size().x * 9, 0 + 20, TextureAsset(U"Batu").size().x * 9).mouseOver()) TextureAsset(U"Batu").scaled(9).draw(Scene::Size().x - 20 - TextureAsset(U"Batu").size().x * 9, 0 + 20, ColorF(1.0, 1.0));
			else TextureAsset(U"Batu").scaled(9).draw(Scene::Size().x - 20 - TextureAsset(U"Batu").size().x * 9, 0 + 20, ColorF(1.0, 0.6));
			break;

		case ResultState::Normal:
			TextureAsset(U"ClearStill").scaled(0.5).draw(0, 0);
			if (Rect(Scene::Size().x - 20 - TextureAsset(U"Trophy").size().x * 9, 0 + 20, TextureAsset(U"Trophy").size().x * 9).mouseOver()) TextureAsset(U"Trophy").scaled(9).draw(Scene::Size().x - 20 - TextureAsset(U"Trophy").size().x * 9, 0 + 20, ColorF(1.0, 1.0));
			else TextureAsset(U"Trophy").scaled(9).draw(Scene::Size().x - 20 - TextureAsset(U"Trophy").size().x * 9, 0 + 20, ColorF(1.0, 0.6));

			FontAsset(U"GameUI_BestTenDot45")(U"Enter to Title").drawAt(Scene::CenterF().movedBy(0, 250), ColorF(0.85, 1 - Periodic::Sawtooth0_1(1.5s)));
			break;
		}

		break;
	}
}
