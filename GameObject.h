#pragma once
#include "Globals.h"

class GameObject {
public:
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual Vector2f getCenter() = 0;
	virtual void checkCollisionWith(GameObject * obj) = 0;
	float radius = 0;
	float lifetime = 0;
	std::string name = "Unnamed";
};
