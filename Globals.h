#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <iostream>
#include <cmath>
#include <string>

#include "ViewManager.h"

#define PI 3.14159265358979323846f

extern sf::RenderWindow window;
extern ViewManager viewMan;
extern unsigned int sWidth;
extern unsigned int sHeight;
extern sf::Font font;

float length(sf::Vector2f v);
sf::Vector2f rotate(sf::Vector2f v, float radians);