#include "Ship.h"

/*

Ship Class

*/

void PlayerShip::init(sf::Font& font) {
	m_speed = 1000;
	m_armor = 100;
	m_shields = 100;
	m_regenRate = 10;
	m_score = 0;
	
	iTimerMax = 0.75f;
	iTimerCurr = iTimerMax;

	regenDelayMax = 2.0f;
	regenDelayCurr = regenDelayMax ;

	m_texture.loadFromFile("Seal.png");
	//m_shape.setTexture(&m_texture);
	m_shape.setPointCount(4);
	m_shape.setPoint(0, sf::Vector2f(0, 0));
	m_shape.setPoint(1, sf::Vector2f(50, 25));
	m_shape.setPoint(2, sf::Vector2f(50, 50));
	m_shape.setPoint(3, sf::Vector2f(0,75));

	m_shape.setOrigin(m_shape.getLocalBounds().width/2, m_shape.getLocalBounds().height / 2);
	m_shape.setPosition(sWidth / 2, sHeight / 2);
	m_shape.setRotation(-90);

	shieldsText.setFont(font);
	shieldsText.setCharacterSize(50);
	shieldsText.setFillColor(sf::Color::Blue);
	shieldsText.setPosition(m_shape.getPosition() + sf::Vector2f(30, -20));

	armorText.setFont(font);
	armorText.setCharacterSize(50);
	armorText.setFillColor(sf::Color::Black);
	armorText.setPosition(m_shape.getPosition() + sf::Vector2f(30, 20));

	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::Yellow);
	scoreText.setPosition(sf::Vector2f(20, sHeight - 100));
	scoreText.setString(std::to_string(m_score));

	shieldsText.setString(std::to_string((int)m_shields));
	armorText.setString(std::to_string((int)m_armor));

	CollisionComponent* c = new CollisionComponent;
	c->name = "Player";
	c->oldPos = m_shape.getPosition();
	c->currPos = m_shape.getPosition();
	c->radius = m_shape.getLocalBounds().height / 2;
	collisionComponent = collision::add_object(c);
}

bool PlayerShip::updateFirst(float dt) {
	shieldsText.setPosition(m_shape.getPosition() + sf::Vector2f(30, -20));
	armorText.setPosition(m_shape.getPosition() + sf::Vector2f(30, 20));

	shieldsText.setString(std::to_string((int)m_shields));
	armorText.setString(std::to_string((int)m_armor));
	scoreText.setString(std::to_string(m_score));

	if (m_shields <= 0 && !regenDelayActive) {
		m_shields = 0;
		regenDelayActive = true;
		regenDelayCurr = regenDelayMax;
	}

	if (regenDelayActive) {
		regenDelayCurr -= dt;
		if (regenDelayCurr <= 0) {
			regenDelayActive = false;
		}
	}

	if (m_shields < 100 && !regenDelayActive) {
		m_shields += m_regenRate * dt;
	}

	if (invincible) {
		iTimerCurr -= dt;
		if (iTimerCurr <= 0) {
			invincible = false;
			m_shape.setFillColor(sf::Color::White);
		}
	}

	if (m_armor <= 0) {
		return true;
	}
	return false;
}

void PlayerShip::updatePosition(float dt) {
	//sizeof(sf::Sprite);
	//sizeof(texture);
	//sizeof(radius);
	//sizeof(dir);
	//sizeof(float);
	//sizeof(int);
	//sizeof(size_t);
	//sizeof(Ship);
	//sizeof(std::string);
	//sizeof(std::vector<Ship>);

	sf::Vector2f pos = m_shape.getPosition();
	collisionComponent->oldPos = pos;

	//if (Keyboard::isKeyPressed(Keyboard::Left)) {
	//	m_shape.move(sf::Vector2f(-speed * dt, 0));
	//}
	//else if (Keyboard::isKeyPressed(Keyboard::Right)) {
	//	m_shape.move(sf::Vector2f(speed * dt, 0));
	//}

	//dir.x = cosf(m_shape.getRotation() * (M_PI / 180));
	//dir.y = sinf(m_shape.getRotation() * (M_PI / 180));

	//if (Keyboard::isKeyPressed(Keyboard::Up)) {
	//	m_shape.move(sf::Vector2f(0, -speed * dt));
	//}
	//else if (Keyboard::isKeyPressed(Keyboard::Down)) {
	//	m_shape.move(sf::Vector2f(0, speed * dt));
	//}

	sf::Vector2i mousePos = Mouse::getPosition(window);
	sf::Vector2f mousePosF((float)mousePos.x, (float)mousePos.y);
	sf::Vector2f newPos = pos;

	//if (mousePos.x < 0) {
	//	sf::Mouse::setPosition(sf::Vector2i(0, mousePos.y), window);
	//}
	//else if (mousePos.x > sWidth) {
	//	sf::Mouse::setPosition(sf::Vector2i(sWidth, mousePos.y), window);
	//}

	//if (mousePos.y < 0) {
	//	sf::Mouse::setPosition(sf::Vector2i(mousePos.x, 0), window);
	//}
	//else if (mousePos.y > sHeight) {
	//	sf::Mouse::setPosition(sf::Vector2i(mousePos.x, sHeight), window);
	//}



	//Move if mouse is on screen and ball isn't past paddle
	if (mousePosF.x >= 0 && mousePosF.x <= sWidth && mousePosF.y >= 0 && mousePosF.y <= sHeight) {
		/*Lerp Implementation*/
		float distTravelled = m_speed * dt; //distance travelled in frame
		float distTarget = dcMath::Magnitude(mousePosF - m_shape.getPosition()); //distance to move towards target
		if (distTarget == 0) return;
		float prog = distTravelled / distTarget; //Fraction of the progress made
		newPos.x = lerp(newPos.x, mousePosF.x, prog);
		newPos.y = lerp(newPos.y, mousePosF.y, prog);
	}
	m_shape.setPosition(newPos);
	collisionComponent->currPos = newPos;
}

void PlayerShip::handleCollision() {
	if (collisionComponent->collided && collisionComponent->otherName != "Laser") {
		if (!invincible) {
			float damage = m_shields - 30;
			if (damage < 0) {
				m_shields = 0;
				m_armor += damage;
			}
			else {
				m_shields = damage;
			}
			iTimerCurr = iTimerMax;
			regenDelayCurr = regenDelayMax;
			invincible = true;
			m_shape.setFillColor(sf::Color::Yellow);
		}

	}
}

void PlayerShip::drawTexts() {
	window.draw(shieldsText);
	window.draw(armorText);
	window.draw(scoreText);
}


/*

Laser Class

*/

Laser::Laser() {
	speed = 1000;
	//radius = 5;
	shape.setPosition(99999999, 999999999);
	name = "Laser";
}

void Laser::fire(Vector2f shipDir, Vector2f shipPos) {
	dir = shipDir;
	
	//shape.setRadius(radius);
	shape.setFillColor(Color::Yellow);
	//shape.setOrigin(radius, radius);
	shape.setPosition(shipPos);

	lifetime = 2;
}

void Laser::update(float dt) {
	if (lifetime > 0) {
		shape.move(dir * (dt * speed));
		lifetime -= dt;
	}
	else {
		shape.setPosition(99999999, 999999999);
	}
}

void Laser::draw() {
	window.draw(shape);
}

Vector2f Laser::getCenter() {
	return shape.getPosition();
}

void Laser::checkCollisionWith(GameObject* other) {

}


void LaserSystem::initialize(int numLasers, int maxShapes, PlayerShip* player) {
	m_player = player;
	m_numLasers = numLasers;
	m_maxShapes = maxShapes;
	m_speed = 500;
	m_maxLifetime = 2;
	laserResetMax = .1f;
	laserResetCurrent = 0;

	if (shapes != nullptr) {
		fprintf(stderr, "Shape array isn't null!");
		delete[] shapes;
		shapes = nullptr;
	}
	shapes = new sf::CircleShape[maxShapes];

	for (int i = 0; i < numLasers; i++) {
		CircleShape shape;
		shape.setFillColor(Color::Yellow);
		shape.setRadius(5);
		shape.setOrigin(shape.getRadius(), shape.getRadius());
		shape.setPosition(99999999, 999999999);

		shapes[i] = shape;
		CircleShape* shapeptr = &shapes[i];

		CollisionComponent* c = new CollisionComponent;
		c->name = "Laser";
		c->currPos = shape.getPosition();
		c->oldPos = shape.getPosition();
		c->radius = shape.getRadius();
		c->active = false;
		collisionComponents.push_back(collision::add_object(c));

		LaserComponent l;
		l.lifetime = 0;
		l.dir = Vector2f(0, -1);
		laserComponents.push_back(l);
	}
}

void LaserSystem::destroy() {
	delete[] shapes;
	shapes = nullptr;

	//Let Collision system destroy colliders?
	collisionComponents.clear();
	collisionComponents.resize(0);
	laserComponents.clear();
	laserComponents.resize(0);
}

void LaserSystem::fire(Vector2f shipDir, Vector2f shipPos) {

}

void LaserSystem::updatePositions(float dt) {
	if (Mouse::isButtonPressed(Mouse::Left)) {
		//printf("Left mouse pressed.\n");
		if (firing == false) {
			for (int i = 0; i < m_numLasers; i++) {
				collisionComponents[i]->oldPos = shapes[i].getPosition();
				if (laserComponents[i].lifetime <= 0) {
					laserComponents[i].lifetime = m_maxLifetime;
					shapes[i].setPosition(m_player->m_shape.getPosition());
					//printf("Setting position to: %f, %f\n", m_player->m_shape.getPosition().x, m_player->m_shape.getPosition().y);
					collisionComponents[i]->active = true;
					break;
				}
			}
			laserResetCurrent = laserResetMax;
			firing = true;
		}
		laserResetCurrent -= dt;
		if (laserResetCurrent <= 0) {
			firing = false;
		}
		//cout << laserResetCurrent << endl;
	}
	else if (!Mouse::isButtonPressed(Mouse::Left)) {
		laserResetCurrent = laserResetMax;
		firing = false;
	}

	for (int i = 0; i < m_numLasers; i++) {
		sf::Vector2f pos = shapes[i].getPosition();
		collisionComponents[i]->oldPos = shapes[i].getPosition();
		Vector2f dir = laserComponents.data()[i].dir;
		float lifetime = laserComponents[i].lifetime;

		if (pos.y < -10) {
			printf("Resetting OOB\n");
			lifetime = 0;
			collisionComponents[i]->active = false;
			shapes[i].setPosition(99999999, 999999999);
			continue;
		}

		if (lifetime > 0 /*&& (pos.x < sWidth + 10 && pos.x > -10 && pos.y > -10 && pos.y < sHeight + 10)*/) {
			shapes[i].move(dir * (dt * m_speed));
			laserComponents[i].lifetime -= dt;
		}
		else {
			collisionComponents[i]->active = false;
			shapes[i].setPosition(99999999, 999999999);
		}
		collisionComponents[i]->currPos = shapes[i].getPosition();
	}
}

void LaserSystem::handleCollisions() {
	for (int i = 0; i < m_numLasers; i++) {
		if (collisionComponents[i]->collided) {
			if (collisionComponents[i]->otherName != "Player" && collisionComponents[i]->otherName != "Laser") {
				laserComponents[i].lifetime = 0;
			}
		}
	}
}

void LaserSystem::drawShapes() {
	for (int i = 0; i < m_numLasers; i++) {
		window.draw(shapes[i]);
	}
}


/*

Asteroid Class

*/

Asteroid::Asteroid(float r, sf::Texture* texture) {
	initAsteroid(r, texture);
}

void Asteroid::initAsteroid(float r, sf::Texture* texture) {
	shape.setRadius(r);
	shape.setOrigin(r, r);
	shape.setPosition(rand() % sWidth, 100);
	shape.setTexture(texture);
	lifetime = 1;
	name = "Asteroid";
	speed = 20 + (rand() % 100);
	health = r * r;
}

void Asteroid::update(float dt) {
	if (shape.getPosition().y > sHeight + 50) {
		shape.setPosition(rand() % sWidth, -50);
		shape.setRadius(10 + rand() % 40);
	}

	shape.move(0, speed * dt);
	if (health <= 0) {
		shape.setPosition(rand() % sWidth, -50);
		shape.setRadius(10 + rand() % 40);
		health = shape.getRadius() * 10;
		speed = 50 + (rand() % 100);
	}
}

void Asteroid::draw() {
	window.draw(shape);
}

sf::Vector2f Asteroid::getCenter() {
	return shape.getPosition();
}

void Asteroid::checkCollisionWith(GameObject* other) {
	collided = false;
	if (shape.getGlobalBounds().intersects(other->shape.getGlobalBounds())) {
		//printf("Collided with: %s\n", other->name.c_str());
		//health -= 10;
		//other->lifetime = 0;
		collided = true;
	}
}

void ::AsteroidSystem::initialize(int numAsteroids, int maxShapes, PlayerShip* player) {
	m_player = player;
	m_numAsteroids = numAsteroids;
	m_maxShapes = maxShapes;

	if (shapes != nullptr) {
		fprintf(stderr, "Shape array isn't null!");
		delete[] shapes;
		shapes = nullptr;
	}
	shapes = new sf::CircleShape[maxShapes];

	if (!asteroidTexture.loadFromFile("rock.jpg")) {
		printf("Could not load asteroid texture!");
	}

	if (!asteroidComponents.empty()) {
		printf("AsteroidComponent list isn't empty.\n");
	}

	for (int i = 0; i < numAsteroids; i++) {
		CircleShape shape;
		shape.setRadius(10 + rand() % 40);
		shape.setOrigin(shape.getRadius(), shape.getRadius());
		shape.setPosition(rand() % sWidth, -(50 + rand() % 500));
		shape.setTexture(&asteroidTexture);

		shapes[i] = shape;
		CircleShape* shapeptr = &shapes[i];

		CollisionComponent* c = new CollisionComponent;
		c->name = "Asteroid";
		c->currPos = shape.getPosition();
		c->oldPos = shape.getPosition();
		c->radius = shape.getRadius();
		collisionComponents.push_back(collision::add_object(c));


		AsteroidComponent a;
		a.health = shape.getRadius() * 1.5f;
		a.speed = 50 + (rand() % 100);
		asteroidComponents.push_back(a);
	}
}

void AsteroidSystem::destroy() {
	delete[] shapes;
	shapes = nullptr;

	//Let Collision system destroy colliders?
	collisionComponents.clear();
	collisionComponents.resize(0);
	asteroidComponents.clear();
	asteroidComponents.resize(0);
}

void AsteroidSystem::updatePositions(float dt) {
	//printf("numAsteroids: %d, asteroidComponents.size(): %d, shapes.size(): %d\n", entity.numAsteroids, entity.asteroidComponents.size(), entity.shapes.size());
	//assert(entity->numAsteroids == entity->asteroidComponents.size() && entity->numAsteroids == entity->shapes.size());
	for (int i = 0; i < m_numAsteroids; i++) {
		float speed = asteroidComponents.data()[i].speed;
		int health = asteroidComponents.data()[i].health;
		float newRadius = shapes[i].getRadius();
		//if (i == 0) printf("Position:%f\n", shape->getPosition().y);
		collisionComponents.data()[i]->oldPos = shapes[i].getPosition();
		shapes[i].move(0, speed * dt);

		if (health <= 0 || (shapes[i].getPosition().y > sHeight + 50)) {
			if (health <= 0) {
				m_player->m_score += shapes[i].getRadius();
			}
			shapes[i].setPosition(rand() % sWidth, -50);
			newRadius = 10 + rand() % 40;
			shapes[i].setRadius(newRadius);
			shapes[i].setOrigin(newRadius, newRadius);
			collisionComponents[i]->radius = newRadius;
			asteroidComponents[i].health = newRadius * 1.5f;
			asteroidComponents[i].speed = 50 + (rand() % 100);
		}
		collisionComponents[i]->currPos = shapes[i].getPosition();
	}
}

void AsteroidSystem::handleCollisions() {
	for (int i = 0; i < m_numAsteroids; i++) {
		CollisionComponent* col = collisionComponents[i];
		if (col->collided) {
			if (col->otherName == "Laser") {
				//printf("Collided with laser. \n");
				asteroidComponents[i].health -= 10;
			}
		}
	}
}

void AsteroidSystem::drawShapes() {
	for (int i = 0; i < m_numAsteroids; i++) {
		window.draw(shapes[i]);
	}
}

void VehicleSystem::Init(int count, PlayerShip* player) {
	m_player = player;
	m_numVehicles = count;

	m_maxHealth = 30;

	if (shapes != nullptr) {
		fprintf(stderr, "Shape array isn't null!");
		delete[] shapes;
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
		c->currPos = shapes[i].getPosition();
		c->radius = 11.0f;
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

void VehicleSystem::destroy() {
	delete[] shapes;
	shapes = nullptr;

	//Let Collision system destroy colliders?
	collisionComponents.clear();
	collisionComponents.resize(0);
	vehicleComponents.clear();
	vehicleComponents.resize(0);
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

	//if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
	//	m_MousePressed = false;
	//}

	//if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_MousePressed) {
	//	m_MousePressed = true;
	//	AddVehicle(1, m_Target);
	//}

	//if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !m_MousePressed) {
	//	m_MousePressed = true;
	//	RemoveVehicle(1);
	//}

	//if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
	//	m_KeyPressed = false;
	//}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !m_KeyPressed) {
	//	m_SepToggle = (m_SepToggle == 0) ? 1 : 0;
	//	m_KeyPressed = true;
	//	if (m_SepToggle == 0) {
	//		m_Text.setString("Separation: Off");
	//	}
	//	else {
	//		m_Text.setString("Separation: On");
	//	}

	//	printf("Separation: %f\n", m_SepToggle);
	//}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !m_KeyPressed) {
	//	m_AlToggle = (m_AlToggle == 0) ? 1 : 0;
	//	m_KeyPressed = true;
	//	if (m_AlToggle == 0) {
	//		m_Text.setString("Alignment: Off");
	//	}
	//	else {
	//		m_Text.setString("Alignment: On");
	//	}
	//	printf("Alignment: %f\n", m_AlToggle);
	//}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && !m_KeyPressed) {
	//	m_CoToggle = (m_CoToggle == 0) ? 1 : 0;
	//	m_KeyPressed = true;
	//	if (m_CoToggle == 0) {
	//		m_Text.setString("Cohesion: Off");
	//	}
	//	else {
	//		m_Text.setString("Cohesion: On");
	//	}
	//	printf("Cohesion: %f\n", m_CoToggle);
	//}

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
		collisionComponents[i]->currPos = shapes[i].getPosition();
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
	sf::Vector2f steer(0, 0);

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
