#include "GameLevel.h"

namespace game_component {

	/*sf::Shape version*/

	//const int MAX_COLLIDERS = 300;
	//CollisionComponent colliders[MAX_COLLIDERS];
	//CollisionComponent* collidersPtr = colliders;
	//int collidersSize = 0;



	/* *************** */

	void GameLevel::initGame() {
		background.init();

		vehicleSystem.Init(10, &player);

		//if (!font.loadFromFile("arial.ttf")) {
		//	std::cout << "Couldn't load font!" << std::endl;
		//}
		//fpsText.setFont(font);
		//fpsText.setCharacterSize(50);
		//fpsText.setFillColor(sf::Color::Green);
		//fpsText.setPosition(10, 10);

		//title.setFont(font);
		//title.setCharacterSize(50);
		//title.setFillColor(sf::Color::Green);
		//title.setString("Component Version");
		//title.setPosition(sWidth/2 - title.getLocalBounds().width / 2 + 10, 10);

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
	}

	AppState* GameLevel::update(float dt) {
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			return new MainMenu();
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
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
		//window.draw(background, 4, sf::Quads);
		background.draw();

		asteroidSystem.drawShapes();
		laserSystem.drawShapes();
		//window.draw(player.m_shape);
		//printf("Drawing player!\n");
		player.draw();
		//circleRenderer.draw(glm::vec2(200, 200), rotateAngle, 100.0f, glm::vec3(1.0f, 0.0f, 1.0f));

		vehicleSystem.Draw();

		//player.drawTexts();

		sf::Time elapsed = clock.restart();
		//fpsText.setString(std::to_string(static_cast<int>(std::round(1.0f / elapsed.asSeconds()))));
		//window.draw(fpsText);
		//window.draw(title);
	}

	GameLevel::GameLevel(int level) {
		initGame();
	}

	GameLevel::~GameLevel() {
		printf("Destructor is being called.\n");
	}
}