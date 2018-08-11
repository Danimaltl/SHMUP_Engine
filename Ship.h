#pragma once
#include "Globals.h"
#include "GameObject.h"
#include "Components.h"
#include "dcMath.h"
#include "dcRenderer.h"
#include "Easings.h"
#include <iostream>

/* Player Ship */
//Size 376
class PlayerShip {
public:
	//sf::ConvexShape m_shape; //size 272
	//sf::Texture m_texture;
	//sf::Text shieldsText;
	//sf::Text armorText;
	//sf::Text scoreText;

	//Behavior
	void init();
	bool updateFirst(float dt);
	void updatePosition(float dt);
	void handleCollision();
	void draw();

	int getScore() { return m_score; }
	void setScore(int newScore) { m_score = newScore; }

private:
	//Rendering
	dcRender::PolyRenderer m_polyRenderer;
	dcRender::Shader m_shader;

	//State Data
	Vector2f dir = Vector2f(0, 0); //size 8
	Vector2f vel = Vector2f(0, 0); //size 8
	glm::vec2 m_position;
	float m_rotation;
	float m_speed = 0; //size 4
	float m_armor = 0;
	float m_shields = 0;
	float m_regenRate = 0;
	int m_score;

	float iTimerCurr = 0;
	float iTimerMax = 0;
	bool invincible = false;

	float regenDelayCurr = 0;
	float regenDelayMax = 0;
	bool regenDelayActive = false;

	//Components
	CollisionComponent* collisionComponent = nullptr;
};

/* Laser Stuff */

//class Laser : public GameObject {
//public:
//	Laser();
//	void fire(Vector2f shipDir, Vector2f shipPos);
//	virtual void update(float dt);
//	virtual void draw();
//	virtual Vector2f getCenter();
//	virtual void checkCollisionWith(GameObject * obj);
//private:
//	CircleShape shape;
//	Vector2f dir = Vector2f(0,0);
//	float speed = 0;
//};

struct LaserComponent {
	Vector2f dir = Vector2f(0, 0);
	float lifetime = 0;
};

class LaserSystem {
	//Shared data
	PlayerShip* m_player = nullptr;
	int m_maxLifetime;
	int m_numLasers;
	int m_maxShapes;
	float m_speed = 0;
	float laserResetMax = 0;
	float laserResetCurrent = 0;
	bool firing = false;

	// Component lists
	//CircleShape* shapes = nullptr;
	std::vector<LaserComponent> laserComponents;
	std::vector<CollisionComponent*> collisionComponents;

public:
	void initialize(int numLasers, int maxShapes, PlayerShip* player);
	void destroy();
	void fire(Vector2f shipDir, Vector2f shipPos);
	void updatePositions(float dt);
	void handleCollisions();
	void drawShapes();
};

/* Asteroid Stuff */

//class Asteroid : public GameObject {
//public:
//	Asteroid(float r, sf::Texture* texture);
//	virtual void update(float dt);
//	virtual void draw();
//	virtual Vector2f getCenter();
//	virtual void checkCollisionWith(GameObject * obj);
//private:
//	Vector2f vel = Vector2f(0, 0);
//	void initAsteroid(float r, sf::Texture* texture);
//	int health = 1000;
//	float speed = 0;
//};

struct AsteroidComponent {
	int health = 1000;
	int maxHealth = 1000;
	float speed = 0;
};

class AsteroidSystem {
	// Component lists
	//CircleShape* shapes = nullptr;
	int m_maxShapes;
	std::vector<AsteroidComponent> asteroidComponents;
	std::vector<CollisionComponent*> collisionComponents;

	//Shared data
	int m_numAsteroids = 0;
	//sf::Texture asteroidTexture;
	PlayerShip* m_player = nullptr;
public:
	//Behavior
	void initialize(int numAsteroids, int maxShapes, PlayerShip* player);
	void destroy();
	void updatePositions(float dt);
	void handleCollisions();
	void drawShapes();
};

struct VehicleComponent {
	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
	int health = 0;
};

class VehicleSystem {
public:
	void destroy();
	void Init(int count, PlayerShip* player);
	void Update(float dt);
	void handleCollisions();
	void Draw();

	void AddVehicle(const unsigned int count, glm::vec2 position);
	void RemoveVehicle(unsigned int count);

	//sf::ConvexShape* shapes = nullptr;
	std::vector<VehicleComponent> vehicleComponents;
	std::vector<CollisionComponent*> collisionComponents;

	glm::vec2 m_Acceleration;
	int m_maxShapes;
	int m_numVehicles;
	float m_Separation = 1.2f;
	float m_Alignment = 0.8f;
	float m_Cohesion = 1.1f;
	float m_Seek = 1.0f;

	float m_SepToggle = 1;
	float m_AlToggle = 1;
	float m_CoToggle = 1;

private:
	glm::vec2 m_Target;

	//sf::Font m_Font;
	//sf::Text m_Text;

	bool m_MousePressed = false;
	bool m_KeyPressed = false;

	float m_MaxSpeed;
	float m_MaxForce;

	float m_maxHealth;

	float m_ArriveRadius;
	float m_NeighborRadius;

	float m_seekRange;

	PlayerShip* m_player = nullptr;

	void ApplyForce(const glm::vec2& force);

	glm::vec2 ComputeSeparation(VehicleComponent* v);
	glm::vec2 ComputeAlignment(VehicleComponent* v);
	glm::vec2 ComputeCohesion(VehicleComponent* v);
	glm::vec2 WallsForce(VehicleComponent* v);

	//Move towards target
	glm::vec2 Seek(VehicleComponent* v, const glm::vec2& target);
	glm::vec2 Arrive(VehicleComponent* v, const glm::vec2& target);
};