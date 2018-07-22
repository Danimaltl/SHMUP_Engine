#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "dcMath.h"
#include "Globals.h"
#include "Components.h"

struct VehicleComponent {
	sf::Vector2f m_Position;
	sf::Vector2f m_Velocity;
};

class VehicleSystem {
public:
	void Init(int count);
	void Update(float dt);
	void Draw();

	void AddVehicle(const unsigned int count, sf::Vector2f position);
	void RemoveVehicle(unsigned int count);

	sf::ConvexShape* shapes = nullptr;
	std::vector<VehicleComponent> vehicleComponents;
	std::vector<CollisionComponent*> collisionComponents;
	
	sf::Vector2f m_Acceleration;
	int m_maxShapes;
	int m_numVehicles;
	float m_Separation = 1.2f;
	float m_Alignment = 0.8f;
	float m_Cohesion = 1.1f;

	float m_SepToggle = 1;
	float m_AlToggle = 1;
	float m_CoToggle = 1;

private:
	sf::Vector2f m_Target;

	sf::Font m_Font;
	sf::Text m_Text;

	bool m_MousePressed = false;
	bool m_KeyPressed = false;

	float m_MaxSpeed;
	float m_MaxForce;

	float m_ArriveRadius;
	float m_NeighborRadius;

	void ApplyForce(const sf::Vector2f& force);

	sf::Vector2f ComputeSeparation(VehicleComponent* v);
	sf::Vector2f ComputeAlignment(VehicleComponent* v);
	sf::Vector2f ComputeCohesion(VehicleComponent* v);
	sf::Vector2f WallsForce(VehicleComponent* v);

	//Move towards target
	void Seek(VehicleComponent* v,const sf::Vector2f& target);
	void Arrive(VehicleComponent* v, const sf::Vector2f& target);
};