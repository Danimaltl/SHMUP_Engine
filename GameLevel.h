 #pragma once
#include "Globals.h"
#include "AppState.h"
#include "GameObject.h"
#include "Components.h"
#include "Ship.h"
#include <memory>

namespace game {

	class GameLevel :public AppState {
	public:
		GameLevel(int level);

		int highscore = 0;

		virtual AppState* update(float dt);
		virtual void draw();
		virtual void destroy();
	};
}

namespace game_component {
	class GameLevel :public AppState {
	public:
		GameLevel(int level);
		virtual ~GameLevel();

		bool firing = true;
		PlayerShip player;
		int score = 0;
		int lives = 0;

		Vertex background[4];

		AsteroidSystem asteroidSystem;
		LaserSystem laserSystem;
		VehicleSystem vehicleSystem;

		sf::Text fpsText;
		sf::Text title;
		sf::Font font;

		sf::Clock clock;

		void initGame();

		int highscore = 0;

		virtual AppState* update(float dt);
		virtual void draw();
		virtual void destroy();
	};
}



