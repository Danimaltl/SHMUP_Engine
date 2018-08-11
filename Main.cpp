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

	glEnable(GL_DEPTH_TEST);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	//viewMan.setView(window.getDefaultView());

	sf::Clock clock;

	dcRender::Shader* circleShader = new dcRender::Shader;
	circleShader->loadFromFile("2dshape.vert", "2dshape.frag");

	dcRender::CircleRenderer circleRenderer;
	circleRenderer.init(60, circleShader);
	float rotateAngle = 0;

	glm::mat4 projection = glm::ortho(0.0f, 600.0f, 800.0f, 0.0f, -1.0f, 1.0f);
	//glm::vec4 test = projection * glm::vec4(200.0f, 200.0f, 0.0f, 1.0f);
	//printf("x: %f, y: %f\n", test.x, test.y);
	circleShader->use();
	circleShader->SetMatrix4("projection", projection);

	bool running = true;
	std::unique_ptr<AppState> currentState(new game_component::GameLevel(1));
	while (running)
	{
		sf::Event event;
		float dt = clock.restart().asSeconds();
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				running = false;
				break;
			}
		}

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rotateAngle += dt;
		circleRenderer.draw(glm::vec2(200,200), rotateAngle, 100.0f, glm::vec3(1.0f, 0.0f, 1.0f));

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