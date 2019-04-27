//Dan Cochran 2018
//Uses FreeType https://www.freetype.org/
#define GLEW_STATIC

#include <GL/glew.h>

#include <cmath>
#include <iostream>
#include <memory>

#include "Globals.h"
#include "AppState.h"
#include "GameLevel.h"

int sWidth = 600;
int sHeight = 800;

//Window to be displayed throughout game
sf::Window window;

//Object for manipulating view
//ViewManager viewMan(window.getDefaultView());

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	window.create(sf::VideoMode(sWidth, sHeight),"SHMUP Engine", sf::Style::Titlebar | sf::Style::Close, settings);

	// Set OpenGL options
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//viewMan.setView(window.getDefaultView());;

	sf::Clock clock;

	std::unique_ptr<AppState> currentState(new MainMenu());
	while (window.isOpen())
	{
		sf::Event event;
		float dt = clock.restart().asSeconds();
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				currentState->destroy();
				window.close();
				break;
			}
		}

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		AppState* next = currentState->update(dt);
		currentState->draw();

		window.display();

		if (next != nullptr) {
			currentState->destroy();
			currentState.reset(next);
		}

	}

	window.close();
	return 0;
}
