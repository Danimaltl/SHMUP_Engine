#pragma once
#include "Globals.h"

struct CollisionComponent {
	std::string otherName;
	std::string name;
	glm::vec2 oldPos;
	glm::vec2 currPos;
	float radius;
	bool collided = false;
	bool active = true;
	~CollisionComponent() {};
	CollisionComponent() {};
};

namespace collision {
	sf::Vector2i getBucket(glm::vec2 pos);
	void bucket_add(sf::Vector2i b, CollisionComponent* obj);
	void bucket_remove(sf::Vector2i b, CollisionComponent* obj);
	void detect_collisions(CollisionComponent* obj, sf::Vector2i b);
	CollisionComponent* add_object(CollisionComponent* obj);
	void CheckAllCollisions();
	void DestroyAllColliders();
}
