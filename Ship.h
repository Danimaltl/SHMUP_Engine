#pragma once
#include "Globals.h"
#include "GameObject.h"

class Ship : public GameObject {
public:
	Ship();
	virtual void update(float dt);
	virtual void draw();
	virtual Vector2f getCenter();
	virtual void checkCollisionWith(GameObject * obj);

	Vector2f dir = Vector2f(0, 0);
	Vector2f vel = Vector2f(0, 0);
private:
	Texture texture;
	Sprite sprite;

	float speed = 300;
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