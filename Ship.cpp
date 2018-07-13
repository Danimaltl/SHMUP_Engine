#include "Ship.h"
#include "Easings.h"
#include <iostream>

/*

Ship Class

*/

Ship::Ship() {
	radius = 20;

	texture.loadFromFile("seal.png");
	sprite.setTexture(texture);

	sprite.setScale(.2,.2);
	sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height / 2);
	sprite.setPosition(sWidth / 2, sHeight / 2);
	sprite.setRotation(-90);

	name = "Player";
}

void Ship::update(float dt) {
	//sizeof(sf::Sprite);
	//sizeof(texture);
	//sizeof(radius);
	//sizeof(dir);
	sizeof(float);
	sizeof(int);
	//sizeof(Ship);
	//sizeof(std::string);
	//sizeof(std::vector<Ship>);

	sf::Vector2f pos = sprite.getPosition();

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		sprite.move(sf::Vector2f(-speed * dt, 0));
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right)) {
		sprite.move(sf::Vector2f(speed * dt, 0));
	}

	dir.x = cosf(sprite.getRotation() * (M_PI / 180));
	dir.y = sinf(sprite.getRotation() * (M_PI / 180));

	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		sprite.move(sf::Vector2f(0, -speed * dt));
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down)) {
		sprite.move(sf::Vector2f(0, speed * dt));
	}

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
	sprite.setPosition(newPos);
}

void Ship::draw() {
	window.draw(sprite);
}

sf::Vector2f Ship::getCenter() {
	return sprite.getPosition();
}

void Ship::checkCollisionWith(GameObject * other) {
	float d = length(getCenter() - other->getCenter());
	float sum = radius + other->radius;
	if (d < sum) {
		printf("Collided with: %s\n", other->name.c_str());
	}
}


/*

Laser Class

*/

Laser::Laser() {
	speed = 1000;
	radius = 5;
	shape.setPosition(99999999, 999999999);
	name = "Laser";
}

void Laser::fire(Vector2f shipDir, Vector2f shipPos) {
	dir = shipDir;
	
	shape.setRadius(radius);
	shape.setFillColor(Color::Yellow);
	shape.setOrigin(radius, radius);
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



/*

Asteroid Class

*/

Asteroid::Asteroid(float r, sf::Texture* texture) {
	initAsteroid(r, texture);
}

void Asteroid::initAsteroid(float r, sf::Texture* texture) {
	radius = r;
	shape.setRadius(r);
	shape.setOrigin(radius, radius);
	shape.setPosition(rand() % sWidth, 100);
	shape.setTexture(texture);
	lifetime = 1;
	name = "Asteroid";
	speed = 20 + (rand() % 100);
	health = radius * radius;
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
		health = radius * 10;
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
	float d = length(getCenter() - other->getCenter());
	float sum = radius + other->radius;
	if (d < sum) {
		//printf("Collided with: %s\n", other->name.c_str());
		//health -= 10;
		//other->lifetime = 0;
	}
}

void ::AsteroidSystem::initialize(AsteroidEntity* e) {
	entity = e;
}

void AsteroidSystem::updatePositions(float dt) {
	//printf("numAsteroids: %d, asteroidComponents.size(): %d, shapes.size(): %d\n", entity.numAsteroids, entity.asteroidComponents.size(), entity.shapes.size());
	assert(entity->numAsteroids == entity->asteroidComponents.size() && entity->numAsteroids == entity->shapes.size());
	for (int i = 0; i < entity->numAsteroids; i++) {
		
		float speed = entity->asteroidComponents[i].speed;
		int health = entity->asteroidComponents[i].health;
		CircleShape* shape = entity->shapes[i];
		//if (i == 0) printf("Position:%f\n", shape->getPosition().y);
		entity->collisionComponents[i]->oldPos = shape->getPosition();
		shape->move(0, speed * dt);

		if (shape->getPosition().y > sHeight + 50) {
			shape->setPosition(rand() % sWidth, -50);
			shape->setRadius(10 + rand() % 40);
		}

		if (health <= 0) {
			shape->setPosition(rand() % sWidth, -50);
			shape->setRadius(10 + rand() % 40);
			entity->asteroidComponents[i].health = shape->getRadius() * shape->getRadius();
			entity->asteroidComponents[i].speed = 50 + (rand() % 100);
		}
	}
}

void AsteroidSystem::handleCollisions() {
	assert(entity->numAsteroids == entity->asteroidComponents.size() && entity->numAsteroids == entity->shapes.size());
	for (int i = 0; i < entity->numAsteroids; i++) {
		//entity->asteroidComponents[i].health -= 10;
	}
}