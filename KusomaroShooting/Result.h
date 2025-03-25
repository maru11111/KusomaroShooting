#pragma once
#include "Common.h"

class Result : public App::Scene{
public:

	enum class MovieState {
		Movie,
		Still,
		ToResult,
		Result
	};

	enum class ResultState {
		RegisterOrNot,
		Register,
		ViewRanking,
		Normal
	};

	enum class RegisterOrNotButton {
		Yes,
		No
	};

	MovieState state=MovieState::Movie;
	ResultState resultState = ResultState::Normal;
	RegisterOrNotButton stillButton=RegisterOrNotButton::Yes;

	double resultTimer = 0;
	const ColorF UIBacColor = ColorF(0.1, 0.6);
	double opacity = 0.0;
	
	mutable double skipAnimTimer = 0;

	Result(const InitData& init);

	~Result();

	void update()override;

	void draw()const override;
};

