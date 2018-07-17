#pragma once
#include "Globals.h"

struct MoveComponent {
	Vector2f velocity;
};

struct CollisionComponent {
	std::string otherName;
	std::string name;
	sf::Vector2f oldPos;
	sf::Shape* shape;
	bool collided = false;
	bool active = true;
};
