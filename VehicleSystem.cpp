#include "VehicleSystem.h"

void VehicleSystem::Init(int count, PlayerShip* player) {
	m_player = player;
	m_numVehicles = count;

	m_maxHealth = 30;

	if (shapes != nullptr) {
		delete(shapes);
		shapes = nullptr;
	}
	shapes = new sf::ConvexShape[m_numVehicles];

	for (int i = 0; i < m_numVehicles; i++) {
		sf::ConvexShape shape;

		//Set shape attributes
		shape.setPointCount(3);
		shape.setPoint(0, sf::Vector2f(0, 0));
		shape.setPoint(1, sf::Vector2f(25, 10));
		shape.setPoint(2, sf::Vector2f(0, 20));
		shape.setFillColor(sf::Color(31, 84, 145));
		shape.setOutlineThickness(1);
		shape.setOutlineColor(sf::Color(100, 100, 100));
		shape.setOrigin(sf::Vector2f(10, 10));
		shape.setPosition(rand() % sWidth, -50);

		shapes[i] = shape;

		VehicleComponent v;
		v.m_Position = sf::Vector2f(rand() % sWidth, -50);
		v.health = m_maxHealth;
		vehicleComponents.push_back(v);

		CollisionComponent* c = new CollisionComponent;
		c->name = "Boid";
		c->oldPos = shapes[i].getPosition();
		c->shape = &shapes[i];
		collisionComponents.push_back(collision::add_object(c));
	}
	
	m_ArriveRadius = 100;
	m_NeighborRadius = 75;
	m_seekRange = 800;
	m_MaxSpeed = 400;
	m_MaxForce = 300;

	if (!m_Font.loadFromFile("arial.ttf"))
	{
		fprintf(stderr, "Couldn't load font!");
	}

	m_Text.setFont(m_Font);
	m_Text.setFillColor(sf::Color::Green);
	m_Text.setPosition(sf::Vector2f(sWidth / 2, 20));
	m_Text.setCharacterSize(28);
	m_Text.setString("BNAG");
	m_Text.setOrigin(m_Text.getGlobalBounds().width / 2, m_Text.getGlobalBounds().height / 2);
}

void VehicleSystem::handleCollisions() {
	for (int i = 0; i < m_numVehicles; i++) {
		CollisionComponent* col = collisionComponents[i];
		if (col->collided) {
			if (col->otherName == "Laser") {
				printf("Collided with Laser\n");
				vehicleComponents[i].health -= 10;
			}
		}
	}
}

void VehicleSystem::Update(float dt) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	m_Target.x = (float)mousePos.x;
	m_Target.y = (float)mousePos.y;

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		m_MousePressed = false;
	}
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_MousePressed) {
		m_MousePressed = true;
		AddVehicle(1, m_Target);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !m_MousePressed) {
		m_MousePressed = true;
		RemoveVehicle(1);
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		m_KeyPressed = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !m_KeyPressed) {
		m_SepToggle = (m_SepToggle == 0) ? 1 : 0;
		m_KeyPressed = true;
		if (m_SepToggle == 0) {
			m_Text.setString("Separation: Off");
		}
		else {
			m_Text.setString("Separation: On");
		}
		
		printf("Separation: %f\n", m_SepToggle);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !m_KeyPressed) {
		m_AlToggle = (m_AlToggle == 0) ? 1 : 0;
		m_KeyPressed = true;
		if (m_AlToggle == 0) {
			m_Text.setString("Alignment: Off");
		}
		else {
			m_Text.setString("Alignment: On");
		}
		printf("Alignment: %f\n", m_AlToggle);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !m_KeyPressed) {
		m_CoToggle = (m_CoToggle == 0) ? 1 : 0;
		m_KeyPressed = true;
		if (m_CoToggle == 0) {
			m_Text.setString("Cohesion: Off");
		}
		else {
			m_Text.setString("Cohesion: On");
		}
		printf("Cohesion: %f\n", m_CoToggle);
	}

	for (size_t i = 0; i < m_numVehicles; i++) {
		collisionComponents[i]->oldPos = vehicleComponents[i].m_Position;

		if (vehicleComponents[i].health <= 0) {
			m_player->m_score += 20;
			vehicleComponents[i].m_Position = sf::Vector2f(rand() % sWidth, -50);
			shapes[i].setPosition(rand() % sWidth, -50);
			vehicleComponents[i].health = m_maxHealth;
		}
		//vehicleComponents[i].Seek(m_Target);
		//vehicleComponents[i].Arrive(m_Target);

		ApplyForce(ComputeSeparation(&vehicleComponents[i]) * m_Separation);
		ApplyForce(ComputeAlignment(&vehicleComponents[i]) * m_Alignment);
		ApplyForce(ComputeCohesion(&vehicleComponents[i]) * m_Cohesion);
		ApplyForce(Seek(&vehicleComponents[i], m_player->m_shape.getPosition()) * m_Seek);
		ApplyForce(WallsForce(&vehicleComponents[i]) * 2.0f);

		//printf("Separation:%f \n", ComputeSeparation(&vehicleComponents[i]));
		vehicleComponents[i].m_Velocity += m_Acceleration * dt;
		dcMath::Limit(vehicleComponents[i].m_Velocity, m_MaxSpeed);
		vehicleComponents[i].m_Position += vehicleComponents[i].m_Velocity * dt;
		m_Acceleration = m_Acceleration * 0.0f; //reset so forces don't continue when stopped

		//if (m_Position.x > SCREEN_WIDTH) {
		//	m_Position.x -= SCREEN_WIDTH;
		//}
		//else if (m_Position.x < 0) {
		//	m_Position.x += SCREEN_WIDTH;
		//}

		//if (m_Position.y > SCREEN_HEIGHT) {
		//	m_Position.y -= SCREEN_HEIGHT;
		//}
		//else if (m_Position.y < 0) {
		//	m_Position.y += SCREEN_HEIGHT;
		//}
		//printf("m_Position: %f, %f\n", m_Position.x, m_Position.y);
		//printf("m_Velocity: %f, %f\n", m_Velocity.x, m_Velocity.y);

		//Set new shape position and rotation
		shapes[i].setPosition(vehicleComponents[i].m_Position);
		//printf("Heading: %f\n", dcMath::Heading(m_Velocity));
		shapes[i].setRotation(dcMath::Heading(vehicleComponents[i].m_Velocity));
	}
}

void VehicleSystem::Draw() {
	for (size_t i = 0; i < vehicleComponents.size(); i++) {
		window.draw(shapes[i]);
	}
	m_Text.setOrigin(m_Text.getGlobalBounds().width / 2, m_Text.getGlobalBounds().height / 2);
	//window.draw(m_Text);
}

void VehicleSystem::AddVehicle(const unsigned int count = 1, sf::Vector2f position = sf::Vector2f(sWidth / 2, sHeight / 2)) {
	//vehicleComponents.resize(vehicleComponents.size() + count, Vehicle(position));
}

void VehicleSystem::RemoveVehicle(unsigned int count = 1) {
	//if (count > vehicleComponents.size()) count = 0;
	//vehicleComponents.resize(vehicleComponents.size() - count, Vehicle(sf::Vector2f(sWidth / 2, sHeight / 2)));
}

sf::Vector2f VehicleSystem::Seek(VehicleComponent* v, const sf::Vector2f& target) {
	sf::Vector2f desired = target - v->m_Position;  //Vector to target from my position
	float magnitude = dcMath::Magnitude(desired);
	sf::Vector2f steer(0,0);

	if (magnitude < m_seekRange) {
		desired = dcMath::Normalize(desired);     //Normalize
		desired *= m_MaxSpeed;                       //Magnitude = maxSpeed

		steer = desired - v->m_Velocity;   //Vector to desired velocity from my velocity (force)

		dcMath::Limit(steer, m_MaxForce);         //Limit magnitude of force
	}

	return steer;
}

sf::Vector2f VehicleSystem::Arrive(VehicleComponent* v, const sf::Vector2f& target) {
	sf::Vector2f desired = target - v->m_Position;
	float d = dcMath::Magnitude(desired);
	desired = dcMath::Normalize(desired);
	if (d < m_ArriveRadius) {
		float m = dcMath::Map(d, 0, m_ArriveRadius, 0, m_MaxSpeed);
		//float m = (d/m_ArriveRadius)*m_MaxSpeed;
		desired *= m;
	}
	else {
		desired *= m_MaxSpeed;
	}

	sf::Vector2f steer = desired - v->m_Velocity;

	dcMath::Limit(steer, m_MaxForce);

	return steer;
}

void VehicleSystem::ApplyForce(const sf::Vector2f& force) {
	m_Acceleration += force;
}

sf::Vector2f VehicleSystem::WallsForce(VehicleComponent* v) {
	sf::Vector2f desired;
	sf::Vector2f steer(0, 0);
	if (v->m_Position.x < 100) {
		desired = sf::Vector2f(m_MaxSpeed, v->m_Velocity.y);
		steer += desired - v->m_Velocity;
		dcMath::Limit(steer, m_MaxForce);
	}
	else if (v->m_Position.x >(sWidth - 100)) {
		desired = sf::Vector2f(-m_MaxSpeed, v->m_Velocity.y);
		steer += desired - v->m_Velocity;
		dcMath::Limit(steer, m_MaxForce);
	}
	if (v->m_Position.y < 100) {
		desired = sf::Vector2f(v->m_Velocity.x, m_MaxSpeed);
		steer += desired - v->m_Velocity;
		dcMath::Limit(steer, m_MaxForce);
	}
	else if (v->m_Position.y >(sHeight - 100)) {
		desired = sf::Vector2f(v->m_Velocity.x, -m_MaxSpeed);
		steer += desired - v->m_Velocity;
		dcMath::Limit(steer, m_MaxForce);
	}
	return steer;
}

sf::Vector2f VehicleSystem::ComputeSeparation(VehicleComponent* v) {

	sf::Vector2f desired;
	float count = 0;

	if (vehicleComponents.size() == 0) {
		printf("Size is 0\n");
		return sf::Vector2f(0, 0);
	}

	for (size_t i = 0; i < vehicleComponents.size(); i++) {
		VehicleComponent* current = &vehicleComponents[i];
		if (current == v)
			continue;

		float d = dcMath::Magnitude(v->m_Position - current->m_Position);
		if (d < m_NeighborRadius) {
			desired += dcMath::Normalize(v->m_Position - current->m_Position);
			count++;
		}
	}

	if (count == 0) {
		//printf("Count is 0\n");
		return sf::Vector2f(0, 0);
	}
	desired = desired / count;
	desired = dcMath::Normalize(desired);
	desired *= m_MaxSpeed;
	sf::Vector2f steer = desired - v->m_Velocity;
	dcMath::Limit(steer, m_MaxForce);

	return steer;
}

sf::Vector2f VehicleSystem::ComputeAlignment(VehicleComponent* v) {
	sf::Vector2f desired;
	float count = 0;

	if (vehicleComponents.size() == 0) {
		printf("Size is 0\n");
		return sf::Vector2f(0, 0);
	}

	for (size_t i = 0; i < vehicleComponents.size(); i++) {
		VehicleComponent* current = &vehicleComponents[i];
		if (current == v)
			continue;

		float dot = dcMath::Dot(dcMath::Normalize(v->m_Position - current->m_Position), dcMath::Normalize(v->m_Velocity));

		float angleBetween = dcMath::AngleBetween(v->m_Position - current->m_Position, v->m_Velocity);

		//if (dot < 0.7071) continue;
		if (angleBetween > 45) continue;

		float distance = dcMath::Magnitude(v->m_Position - current->m_Position);
		if (distance < m_NeighborRadius) {
			desired += current->m_Velocity;
			count++;
		}
	}

	if (count == 0) {
		//printf("Count is 0\n");
		return sf::Vector2f(0, 0);
	}
	desired = desired / count;
	desired = dcMath::Normalize(desired);
	desired *= m_MaxSpeed;
	sf::Vector2f steer = desired - v->m_Velocity;
	dcMath::Limit(steer, m_MaxForce);

	return steer;
}

sf::Vector2f VehicleSystem::ComputeCohesion(VehicleComponent* v) {
	sf::Vector2f desired;
	float count = 0;

	if (vehicleComponents.size() == 0) {
		printf("Size is 0\n");
		return sf::Vector2f(0, 0);
	}

	for (size_t i = 0; i < vehicleComponents.size(); i++) {
		VehicleComponent* current = &vehicleComponents[i];
		if (current == v)
			continue;

		float d = dcMath::Magnitude(v->m_Position - current->m_Position);
		if (d < m_NeighborRadius) {
			desired += current->m_Position;
			count++;
		}
	}

	if (count == 0) {
		//printf("Count is 0\n");
		return sf::Vector2f(0, 0);
	}
	desired = desired / count;
	desired = desired - v->m_Position;
	desired = dcMath::Normalize(desired);
	desired *= m_MaxSpeed;
	sf::Vector2f steer = desired - v->m_Velocity;
	dcMath::Limit(steer, m_MaxForce);

	return steer;
}
