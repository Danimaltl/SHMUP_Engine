#pragma once
#define GLEW_STATIC

#include <GL/glew.h>
//#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
//#include <SFML/Main.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include <assert.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

extern sf::Window window;
//extern ViewManager viewMan;
extern int sWidth;
extern int sHeight;
//extern sf::Font font;
