//Dan Cochran 2018
//Uses FreeType https://www.freetype.org/

// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#define GLEW_STATIC

#include <GL/glew.h>

#include <cmath>
#include <iostream>
#include <memory>

#include "Globals.h"
#include "ViewManager.h"
#include "AppState.h"
#include "GameLevel.h"

unsigned int sWidth = 600;
unsigned int sHeight = 800;

//Window to be displayed throughout game
Window window;

//Object for manipulating view
//ViewManager viewMan(window.getDefaultView());



int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	window.create(VideoMode(sWidth, sHeight),"SHMUP Engine", sf::Style::Titlebar | sf::Style::Close, settings);

	// Set OpenGL options
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//viewMan.setView(window.getDefaultView());;

	sf::Clock clock;

	bool running = true;
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
	_CrtDumpMemoryLeaks();
	return 0;
}