#include "AppState.h"
#include "GameLevel.h"
#include "Globals.h"

Text title;
Text start;
Text quit;

Texture bgTexture;
Sprite background;

void initMenu() {
	bgTexture.loadFromFile("bg.jpg");
	background.setTexture(bgTexture);

	title.setFont(font);
	title.setString("Dan's Asteroids");
	title.setCharacterSize(50);
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);
	title.setPosition(sWidth / 2, 100);
	

	start.setFont(font);
	start.setString("Press SPACE to start game.");
	start.setPosition(10, sHeight / 2);

	quit.setFont(font);
	quit.setString("Press q to quit.");
	quit.setPosition(sWidth - 210, sHeight / 2);
}

//Main Menu Functions
AppState* MainMenu::update(float dt) {
	title.rotate(100 * dt);
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		//return new game::GameLevel(1);
		return new game_component::GameLevel(1);
	}
	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		window.close();
	}

	//cout << "updating\n";
	return nullptr;
}

void MainMenu::draw() {
	//cout << "drawing\n";
	window.clear();
	window.draw(background);
	window.draw(title);
	window.draw(start);
	window.draw(quit);
}

MainMenu::MainMenu() {
	initMenu();
}

void MainMenu::destroy() {

}