#pragma once
#include "Globals.h"

struct MoveComponent {
	Vector2f velocity;
};

struct CollisionComponent {
	std::vector<std::string> hitNames;
	std::string name;
	sf::Vector2f oldPos;
	sf::Shape* shape;
};
