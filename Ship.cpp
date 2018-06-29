#include "Ship.h"
#include "Easings.h"

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
}

void Ship::update(float dt) {
	Vector2f pos = sprite.getPosition();

	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		sprite.move(sf::Vector2f(-speed * dt, 0));
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right)) {
		sprite.move(sf::Vector2f(speed * dt, 0));
	}

	dir.x = cosf(sprite.getRotation() * (PI / 180));
	dir.y = sinf(sprite.getRotation() * (PI / 180));

	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		sprite.move(sf::Vector2f(0, -speed * dt));
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down)) {
		sprite.move(sf::Vector2f(0, speed * dt));
	}

	Vector2i mousePos = Mouse::getPosition(window);
	Vector2f mousePosF((float)mousePos.x, (float)mousePos.y);
	Vector2f newPos = pos;

	//Move if mouse is on screen and ball isn't past paddle
	if (mousePosF.x > 0 && mousePosF.x < sWidth && mousePosF.y > 0 && mousePosF.y < sHeight) {
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

Vector2f Ship::getCenter() {
	return sprite.getPosition();
}

void Ship::checkCollisionWith(GameObject * other) {
	float d = length(getCenter() - other->getCenter());
	float sum = radius + other->radius;
	if (d < sum) {
		//cout << "Ship collided!\n";
	}
}


/*

Laser Class

*/

Laser::Laser() {
	speed = 500;
	radius = 5;
	shape.setPosition(99999999, 999999999);
}

void Laser::fire(Vector2f shipDir, Vector2f shipPos) {
	dir = shipDir;
	
	shape.setRadius(radius);
	shape.setFillColor(Color::Yellow);
	shape.setOrigin(radius, radius);
	shape.setPosition(shipPos);

	lifetime = 3;
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

Asteroid::Asteroid(float r) {
	initAsteroid(r);
}

void Asteroid::initAsteroid(float r) {
	texture.loadFromFile("rock.jpg");
	radius = r;
	shape.setRadius(r);
	shape.setPosition(-99999999, -999999999);
	shape.setTexture(&texture);
	lifetime = 1;
}

void Asteroid::update(float dt) {
	if (lifetime > 0) {
		shape.move(vel);
	}
	else {
		shape.setPosition(-99999999, -999999999);
	}
}

void Asteroid::draw() {
	window.draw(shape);
}

Vector2f Asteroid::getCenter() {
	return shape.getPosition();
}

void Asteroid::checkCollisionWith(GameObject* other) {

}