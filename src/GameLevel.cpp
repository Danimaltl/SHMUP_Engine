#include "GameLevel.h"

namespace game_component {

	void GameLevel::initGame() {

		background.init();

		vehicleSystem.Init(10, &player);

		textRenderer.init();

		fpsText.scale = 1;
		fpsText.color = glm::vec3(0, 1, 0);
		fpsText.position = glm::vec2(30,40);

		player.init();

		asteroidSystem.initialize(10, &player);
		laserSystem.initialize(50, 50, &player);

	}

	void GameLevel::destroy() {
		background.destroy();
		player.destroy();
		vehicleSystem.destroy();
		laserSystem.destroy();
		asteroidSystem.destroy();
		collision::DestroyAllColliders();
		textRenderer.destroy();
	}

	AppState* GameLevel::update(float dt) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			return new MainMenu();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			window.close();
		}

		background.update(dt);
		vehicleSystem.Update(dt);

		if (player.updateFirst(dt)) {
			return new MainMenu();
		}

		//update positions
		player.updatePosition(dt);
		laserSystem.updatePositions(dt);
		asteroidSystem.updatePositions(dt);

		collision::CheckAllCollisions();

		//handle collisions
		player.handleCollision();
		asteroidSystem.handleCollisions();
		vehicleSystem.handleCollisions();
		laserSystem.handleCollisions();

		rotateAngle += dt;

		return nullptr;
	}



	void GameLevel::draw() {
		//window.setView(viewMan.getView());
		//window.clear();
		background.draw();

		asteroidSystem.drawShapes();
		laserSystem.drawShapes();
		player.draw();

		vehicleSystem.Draw();

		sf::Time elapsed = clock.restart();
		fpsText.text = std::to_string(static_cast<int>(std::round(1.0f / elapsed.asSeconds())));
		textRenderer.draw(fpsText);
	}

	GameLevel::GameLevel(int level) {
		initGame();
	}

	GameLevel::~GameLevel() {

	}
}
