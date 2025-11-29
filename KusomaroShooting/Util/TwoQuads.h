#pragma once
# include<Siv3D.hpp> // Siv3D v0.6.14

class TwoQuads {
public:
	Array<Quad>quads;

	TwoQuads(Quad quad1) {
		quads << quad1;
	}

	TwoQuads(Quad quad1, Quad quad2) {
		quads << quad1;
		quads << quad2;
	}

	bool intersects(RectF rect) {
		bool isIntersects=false;
		for (int i = 0; i < quads.size(); i++) {
			if (quads[i].intersects(rect)) {
				isIntersects = true;
			}
		}
		if (isIntersects) return true;
		else return false;
	}
};
