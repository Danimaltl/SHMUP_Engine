#pragma once
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include <assert.h>

#include "ViewManager.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

extern sf::RenderWindow window;
//extern ViewManager viewMan;
extern unsigned int sWidth;
extern unsigned int sHeight;
extern sf::Font font;

float length(sf::Vector2f v);
sf::Vector2f rotate(sf::Vector2f v, float radians);