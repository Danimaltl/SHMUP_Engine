 #pragma once
#include "Globals.h"
#include "AppState.h"
#include "GameObject.h"
#include "Components.h"

namespace game {

	class GameLevel :public AppState {
	public:
		GameLevel(int level);

		int highscore = 0;

		virtual AppState* update(float dt);
		virtual void draw();
	};
}

namespace game_component {
	class GameLevel :public AppState {
	public:
		GameLevel(int level);

		int highscore = 0;

		virtual AppState* update(float dt);
		virtual void draw();
	};
}

namespace collision {
	sf::Vector2i getBucket(Vector2f pos);
	void bucket_add(sf::Vector2i b, CollisionComponent* obj);
	void bucket_remove(Vector2i b, CollisionComponent* obj);
	bool checkCollision(const sf::FloatRect& box1, const sf::FloatRect& box2);
	void detect_collisions(CollisionComponent* obj, Vector2i b);
	CollisionComponent* add_object(CollisionComponent* obj);
	void CheckAllCollisions();
}



