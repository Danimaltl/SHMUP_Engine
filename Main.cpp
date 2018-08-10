#define GLEW_STATIC

#include <GL/glew.h>

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
//ViewManager viewMan(window.getDefaultView());

sf::Font font;

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	window.create(VideoMode(sWidth, sHeight),"SHMUP Engine", sf::Style::Titlebar | sf::Style::Close, settings);

	//viewMan.setView(window.getDefaultView());

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

		//// Clear the screen to black
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// Calculate transformation
		//time += dt;
		//glUniform1f(uniTime, time);

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		//	view = glm::translate(view, glm::vec3(2.0f*dt, 0.0f, 0.0f));
		//}
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		//	view = glm::translate(view, glm::vec3(-2.0f*dt, 0.0f, 0.0f));
		//}

		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, 1.0f*dt));
		//	view = glm::translate(view, glm::vec3(0.0f, 2.0f*dt, 0.0f));
		//}
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		//	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f*dt));
		//	view = glm::translate(view, glm::vec3(0.0f, -2.0f*dt, 0.0f));
		//}

		//glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		//float cubesInColumn = 20.0f;
		//float cubesInRow = 20.0f;
		//float radius = 5.0f;

		//for (int i = 0; i < cubesInRow; i++) {
		//	for (int j = 0; j < cubesInColumn; j++) {
		//		glm::mat4 model = glm::mat4(1.0f);
		//		model = glm::rotate(
		//			model,
		//			time * glm::radians(60.0f),
		//			glm::vec3(0.0f, 0.0f, 1.0f)
		//		);
		//		float xpos = (float)cosf(((float)i / cubesInRow) * 2.0f * (float)M_PI) * (1.5f * radius + cos(time / 2)*(radius / 2));
		//		float ypos = (float)sinf(((float)i / cubesInRow) * 2.0f * (float)M_PI) * (1.5f * radius + cos(time / 2)*(radius / 2));
		//		float zpos = (float)(-cubesInColumn + 2 * j) + cos(time + (i * 5)) * 2.0f;
		//		glm::vec3 position = glm::vec3(xpos, ypos, zpos);
		//		model = glm::translate(model, position);
		//		model = glm::rotate(
		//			model,
		//			time * glm::radians(180.0f),
		//			glm::vec3(cosf(time), sinf(time), 1.0f)
		//		);
		//		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		//		glDrawArrays(GL_TRIANGLES, 0, 36);
		//	}
		//}

		//window.pushGLStates();
		AppState* next = currentState->update(dt);
		currentState->draw();
		//window.popGLStates();
		window.display();

		if (next != nullptr) {
			currentState->destroy();
			currentState.reset(next);
		}
			
	}

	return 0;
}