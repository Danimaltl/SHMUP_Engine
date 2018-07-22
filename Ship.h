#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Components.h"
#include "GameLevel.h"
#include "dcMath.h"

/* Player Ship */
//Size 376
class PlayerShip {
public:
	sf::ConvexShape m_shape; //size 272
	sf::Texture m_texture;
	sf::Text shieldsText;
	sf::Text armorText;

	//State Data
	Vector2f dir = Vector2f(0, 0); //size 8
	Vector2f vel = Vector2f(0, 0); //size 8
	float m_speed = 0; //size 4
	float m_armor = 0;
	float m_shields = 0;
	float m_regenRate = 0;
	
	float iTimerCurr = 0;
	float iTimerMax = 0;
	bool invincible = false;

	//Components
	CollisionComponent* collisionComponent;

	//Behavior
	void init(sf::Font& font);
	void updateFirst(float dt);
	void updatePosition(float dt);
	void handleCollision();
	void drawTexts();

private:

};

/* Laser Stuff */

class Laser : public GameObject {
public:
	Laser();
	void fire(Vector2f shipDir, Vector2f shipPos);
	virtual void update(float dt);
	virtual void draw();
	virtual Vector2f getCenter();
	virtual void checkCollisionWith(GameObject * obj);
private:
	CircleShape shape;
	Vector2f dir = Vector2f(0,0);
	float speed = 0;
};

struct LaserComponent {
	Vector2f dir = Vector2f(0, 0);
	float lifetime = 0;
};

class LaserSystem {
	//Shared data
	PlayerShip* m_player;
	int m_maxLifetime;
	int m_numLasers;
	int m_maxShapes;
	float m_speed = 0;
	float laserResetMax = 0;
	float laserResetCurrent = 0;
	bool firing = false;

	// Component lists
	CircleShape* shapes = nullptr;
	std::vector<LaserComponent> laserComponents;
	std::vector<CollisionComponent*> collisionComponents;

public:
	void initialize(int numLasers, int maxShapes, PlayerShip* player);
	void fire(Vector2f shipDir, Vector2f shipPos);
	void updatePositions(float dt);
	void handleCollisions();
	void drawShapes();
};

/* Asteroid Stuff */

class Asteroid : public GameObject {
public:
	Asteroid(float r, sf::Texture* texture);
	virtual void update(float dt);
	virtual void draw();
	virtual Vector2f getCenter();
	virtual void checkCollisionWith(GameObject * obj);
private:
	Vector2f vel = Vector2f(0, 0);
	void initAsteroid(float r, sf::Texture* texture);
	int health = 1000;
	float speed = 0;
};

struct AsteroidComponent {
	int health = 1000;
	int maxHealth = 1000;
	float speed = 0;
};

class AsteroidSystem {
	// Component lists
	CircleShape* shapes = nullptr;
	int m_maxShapes;
	std::vector<AsteroidComponent> asteroidComponents;
	std::vector<CollisionComponent*> collisionComponents;

	//Shared data
	int m_numAsteroids = 0;
	sf::Texture asteroidTexture;
public:
	//Behavior
	void initialize(int numAsteroids, int maxShapes);
	void updatePositions(float dt);
	void handleCollisions();
	void drawShapes();
};