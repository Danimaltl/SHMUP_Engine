#pragma once
#include "Globals.h"
#include "AppState.h"
#include "GameObject.h"

namespace game {

	class GameLevel :public AppState {
	public:
		GameLevel(int level);

		int highscore = 0;

		virtual AppState* update(float dt);
		virtual void draw();
	};

}