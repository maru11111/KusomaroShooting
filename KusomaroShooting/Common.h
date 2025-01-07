#pragma once
# include<Siv3D.hpp> // Siv3D v0.6.14

enum class State {
	//Title,
	Game,
	StageEditor
};

enum class EnemyType {
	Empty,
	Bag,
	FastBag,
	BagWithCan,
	Can,
	Fish,
	Umbrella
};

using App = SceneManager<State>;
