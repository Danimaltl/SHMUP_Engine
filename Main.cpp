#define GLEW_STATIC

#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <cmath>
#include <iostream>
#include <memory>

#include "Globals.h"
#include "ViewManager.h"
#include "AppState.h"

unsigned int sWidth = 600;
unsigned int sHeight = 800;

//Window to be displayed throughout game
RenderWindow window;

//Object for manipulating view
ViewManager viewMan(window.getDefaultView());

sf::Font font;

int main()
{
	sf::ContextSettings settings;
	window.create(VideoMode(sWidth, sHeight),"Dan's Asteroids!", sf::Style::Titlebar | sf::Style::Close);
	viewMan.setView(window.getDefaultView());

	font.loadFromFile("arial.ttf");

	sf::Clock clock;

	std::unique_ptr<AppState> currentState(new MainMenu());
	while (window.isOpen())
	{
		sf::Event event;
		float dt = clock.restart().asSeconds();
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		AppState* next = currentState->update(dt);
		currentState->draw();
		window.display();

		if (next != nullptr)
			currentState.reset(next);
	}

	return 0;
}