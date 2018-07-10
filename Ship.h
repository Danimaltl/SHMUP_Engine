#pragma once
#include "Globals.h"
#include "GameObject.h"

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
	float speed = 0;
};

struct AsteroidEntity {
	int m_ID;
	sf::Shape * shape;
	AsteroidComponent * data;
};

class AsteroidSystem {
	//Shared data
	int maxHealth;
	int numAsteroids;

	// Component lists
	std::vector<sf::Shape>* shapes;
	std::vector<AsteroidComponent>* stats;

	//Behavior
	void updatePositions(float dt);
	void handleCollisions();
};