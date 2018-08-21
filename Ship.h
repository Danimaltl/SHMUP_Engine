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
	void destroy();

	glm::vec2 getPosition() { return m_position; }
	int getScore() { return m_score; }
	void setScore(int newScore) { m_score = newScore; }

private:
	//Rendering
	dcRender::PolyRenderer m_polyRenderer;
	dcRender::Shader m_shader;
	glm::vec3 m_color = glm::vec3(1, 1, 1);

	//State Data
	glm::vec2 dir = glm::vec2(0, 0); //size 8
	glm::vec2 vel = glm::vec2(0, 0); //size 8
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
struct LaserComponent {
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 dir = glm::vec2(0, 0);
	float lifetime = 0;
	float radius;
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

	//Rendering
	dcRender::CircleRenderer m_circleRenderer;
	dcRender::Shader m_shader;
	glm::vec3 m_color = glm::vec3(1, 1, 1);

	std::vector<LaserComponent> laserComponents;
	std::vector<CollisionComponent*> collisionComponents;

public:
	void initialize(int numLasers, int maxShapes, PlayerShip* player);
	void destroy();
	void fire(glm::vec2 shipDir, glm::vec2 shipPos);
	void updatePositions(float dt);
	void handleCollisions();
	void drawShapes();
};

/* Asteroid Stuff */
struct AsteroidComponent {
	glm::vec2 position = glm::vec2(0, 0);
	int health = 1000;
	int maxHealth = 1000;
	float speed = 0;
	float radius;
};

class AsteroidSystem {
	// Component lists
	std::vector<AsteroidComponent> asteroidComponents;
	std::vector<CollisionComponent*> collisionComponents;

	//Rendering
	dcRender::CircleRenderer m_circleRenderer;
	dcRender::Shader m_shader;
	glm::vec3 m_color = glm::vec3(1, 1, 1);

	//Shared data
	int m_numAsteroids = 0;
	//sf::Texture asteroidTexture;
	PlayerShip* m_player = nullptr;
public:
	//Behavior
	void initialize(int numAsteroids, PlayerShip* player);
	void destroy();
	void updatePositions(float dt);
	void handleCollisions();
	void drawShapes();
};

struct VehicleComponent {
	glm::vec2 m_Position = glm::vec2(0,0);
	glm::vec2 m_Velocity = glm::vec2(0, 0);
	float rotation = 0;
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
	//Rendering
	dcRender::PolyRenderer m_polyRenderer;
	dcRender::Shader m_shader;
	glm::vec3 m_color = glm::vec3(0, 0, 1);

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