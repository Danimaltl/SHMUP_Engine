#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Components.h"

/* Player Ship */
//Size 376
class Ship : public GameObject {
public:
	Ship();
	virtual void update(float dt);
	virtual void draw();
	virtual Vector2f getCenter();
	virtual void checkCollisionWith(GameObject * obj);
	Sprite sprite; //size 272
	Texture texture; //size 40
	Vector2f dir = Vector2f(0, 0); //size 8
	Vector2f vel = Vector2f(0, 0); //size 8
	float speed = 300; //size 4
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
	float speed = 0;
	float lifetime = 0;
};

class LaserSystem {
	//Shared data
	int maxLifetime;
	int numLasers;

	// Component lists
	std::vector<sf::Shape>* shapes;
	std::vector<LaserComponent>* laserComponents;

	void updatePositions(float dt);
	void handleCollisions();
};

/* Asteroid Stuff */

class Asteroid : public GameObject {
public:
	Asteroid(float r);
	virtual void update(float dt);
	virtual void draw();
	virtual Vector2f getCenter();
	virtual void checkCollisionWith(GameObject * obj);
private:
	Texture texture;
	CircleShape shape;
	Vector2f vel = Vector2f(0, 0);
	void initAsteroid(float r);
	int health = 1000;
	float speed = 0;
};

struct AsteroidComponent {
	int health = 1000;
	int maxHealth = 1000;
	float speed = 0;
};

struct AsteroidEntity {
	// Component lists
	std::vector<sf::CircleShape*> shapes;
	std::vector<AsteroidComponent> asteroidComponents;
	std::vector<CollisionComponent*> collisionComponents;

	//Shared data
	int numAsteroids = 0;
};

class AsteroidSystem {
	AsteroidEntity* entity;
public:
	//Behavior
	void initialize(AsteroidEntity* e);
	void updatePositions(float dt);
	void handleCollisions();
};