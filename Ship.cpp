#include "Ship.h"
#include "Easings.h"
#include <iostream>

/*

Ship Class

*/

void PlayerShip::init() {
	m_texture.loadFromFile("Seal.png");
	m_shape.setTexture(&m_texture);
	m_shape.setPointCount(4);
	m_shape.setPoint(0, sf::Vector2f(0, 0));
	m_shape.setPoint(1, sf::Vector2f(120, 0));
	m_shape.setPoint(2, sf::Vector2f(120, 40));
	m_shape.setPoint(3, sf::Vector2f(0, 40));

	m_shape.setOrigin(m_shape.getLocalBounds().width/2, m_shape.getLocalBounds().height / 2);
	m_shape.setPosition(sWidth / 2, sHeight / 2);
	m_shape.setRotation(-90);

	CollisionComponent* c = new CollisionComponent;
	c->name = "Player";
	c->oldPos = m_shape.getPosition();
	c->shape = &m_shape;
	collisionComponent = collision::add_object(c);
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

	if (mousePos.x < 0) {
		sf::Mouse::setPosition(sf::Vector2i(0, mousePos.y), window);
	}
	else if (mousePos.x > sWidth) {
		sf::Mouse::setPosition(sf::Vector2i(sWidth, mousePos.y), window);
	}

	if (mousePos.y < 0) {
		sf::Mouse::setPosition(sf::Vector2i(mousePos.x, 0), window);
	}
	else if (mousePos.y > sHeight) {
		sf::Mouse::setPosition(sf::Vector2i(mousePos.x, sHeight), window);
	}



	//Move if mouse is on screen and ball isn't past paddle
	if (mousePosF.x >= 0 && mousePosF.x <= sWidth && mousePosF.y >= 0 && mousePosF.y <= sHeight) {
		float speed = 2000;
		/*Lerp Implementation*/
		float distTravelled = speed * dt; //distance travelled in frame
		float distTarget = abs(pos.x - mousePosF.x); //distance to move towards target
		if (distTarget == 0) return;
		float prog = distTravelled / distTarget; //Fraction of the progress made
		newPos.x = lerp(newPos.x, mousePosF.x, prog);
		newPos.y = lerp(newPos.y, mousePosF.y, prog);
	}
	m_shape.setPosition(newPos);
}

void PlayerShip::handleCollision() {

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
	float laserResetMax = .1f;
	float laserResetCurrent = 0;

	if (shapes != nullptr) {
		delete(shapes);
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
		c->shape = shapeptr;
		c->oldPos = shape.getPosition();
		c->active = false;
		collisionComponents.push_back(collision::add_object(c));

		LaserComponent l;
		l.lifetime = 0;
		l.dir = Vector2f(0, -1);
		laserComponents.push_back(l);
	}
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

void ::AsteroidSystem::initialize(int numAsteroids, int maxShapes) {
	m_numAsteroids = numAsteroids;
	m_maxShapes = maxShapes;

	if (shapes != nullptr) {
		delete(shapes);
		shapes = nullptr;
	}
	shapes = new sf::CircleShape[maxShapes];

	if (!asteroidTexture.loadFromFile("rock.jpg")) {
		printf("Could not load asteroid texture!");
	}

	for (int i = 0; i < numAsteroids; i++) {
		CircleShape shape;
		shape.setRadius(10 + rand() % 40);
		shape.setOrigin(shape.getRadius(), shape.getRadius());
		shape.setPosition(rand() % sWidth, 100);
		shape.setTexture(&asteroidTexture);

		shapes[i] = shape;
		CircleShape* shapeptr = &shapes[i];

		CollisionComponent* c = new CollisionComponent;
		c->name = "Asteroid";
		c->shape = shapeptr;
		c->oldPos = shape.getPosition();
		collisionComponents.push_back(collision::add_object(c));

		AsteroidComponent a;
		a.health = shape.getRadius() * 1.5f;
		a.speed = 50 + (rand() % 100);
		asteroidComponents.push_back(a);
	}
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
			shapes[i].setPosition(rand() % sWidth, -50);
			newRadius = 10 + rand() % 40;
			shapes[i].setRadius(newRadius);
			asteroidComponents[i].health = newRadius * 1.5f;
			asteroidComponents[i].speed = 50 + (rand() % 100);
		}
	}
}

void AsteroidSystem::handleCollisions() {
	for (int i = 0; i < m_numAsteroids; i++) {
		CollisionComponent* col = collisionComponents[i];
		if (col->collided) {
			if (col->otherName == "Laser") {
				printf("Collided with laser. \n");
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