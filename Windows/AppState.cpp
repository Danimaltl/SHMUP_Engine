#include "AppState.h"
#include "GameLevel.h"

void MainMenu::initMenu() {
	printf("Init main menu\n");

	textRenderer.init();

	title.text = "Day of Saturn";
	title.position = glm::vec2(sWidth / 2 - 100, 100);

	start.text = "Press SPACE to start game.";
	start.position = glm::vec2(10, sHeight / 2);

	quit.text = "Press q to quit.";
	quit.position = glm::vec2(10, sHeight / 2 + 50);

}

//Main Menu Functions
AppState* MainMenu::update(float dt) {
	//title.rotate(100 * dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		return new game_component::GameLevel(1);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		window.close();
	}

	//cout << "updating\n";
	return nullptr;
}

void MainMenu::draw() {
	//cout << "drawing\n";
	//window.clear();
	textRenderer.draw(title);
	textRenderer.draw(start);
	textRenderer.draw(quit);
}

MainMenu::MainMenu() {
	initMenu();
}

MainMenu::~MainMenu() {

}

void MainMenu::destroy() {
	textRenderer.destroy();
}
