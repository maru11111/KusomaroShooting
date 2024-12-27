#pragma once
# include<Siv3D.hpp> // Siv3D v0.6.14

enum class State {
	//Title,
	Game
};

using App = SceneManager<State>;
