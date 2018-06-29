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

#include "ViewManager.h"

#define PI 3.14159265358979323846f

using namespace sf;
using namespace std;

extern RenderWindow window;
extern ViewManager viewMan;
extern unsigned int sWidth;
extern unsigned int sHeight;
extern Font font;

float length(Vector2f v);
Vector2f rotate(Vector2f v, float radians);