#include "GameLevel.h"
#include "Ship.h"
#include "Components.h"
#include <memory>

namespace game {

	std::vector<GameObject *> objects;

	/*Bucket Grid Code from Walt's slides*/
	const float BUCKET_WIDTH = 100;
	const float BUCKET_HEIGHT = 100;
	const int COLUMNS = 10;
	const int ROWS = 10;
	std::vector<GameObject*> grid[COLUMNS][ROWS];

	sf::Vector2i getBucket(Vector2f pos) {
		int col = int(pos.x / BUCKET_WIDTH);
		if (col < 0)
			col = 0;
		else if (col >= COLUMNS)
			col = COLUMNS - 1;

		int row = int(pos.y / BUCKET_HEIGHT);
		if (row < 0)
			row = 0;
		else if (row >= ROWS)
			row = ROWS - 1;

		return sf::Vector2i(col, row);
	}

	void bucket_add(sf::Vector2i b,
		GameObject* obj)
	{
		//printf("Added %s to bucket.\n", obj->name.c_str());
		std::vector<GameObject*> & v
			= grid[b.x][b.y];

		v.push_back(obj);
	}

	void bucket_remove(Vector2i b,
		GameObject* obj)
	{
		std::vector<GameObject*> & v
			= grid[b.x][b.y];

		for (int i = 0; i < v.size(); ++i)
		{
			if (v[i] == obj)
			{
				v.erase(v.begin() + i);
				break;
			}
		}
	}

	void detect_collisions(GameObject* obj,
		Vector2i b)
	{
		int left = std::max(b.x - 1, 0);
		int right = std::min(b.x + 1, COLUMNS - 1);
		int top = std::max(b.y - 1, 0);
		int bot = std::min(b.y + 1, ROWS - 1);

		for (int bx = left; bx <= right; ++bx)
		{
			for (int by = top; by <= bot; ++by)
			{
				std::vector<GameObject*> & v = grid[bx][by];
				for (GameObject* o : v)
				{
					if (o != obj)
						obj->checkCollisionWith(o);
				}
			}
		}
	}

	void physics_add_object(GameObject* obj) {
		objects.push_back(obj);
		sf::Vector2i coord = getBucket(obj->getCenter());
		bucket_add(coord, obj);
	}

	bool firing = true;
	Ship player;
	int score = 0;
	int lives = 0;
	float laserResetMax = .05f;
	float laserResetCurrent = 0;

	Vertex bg[] = {
		Vertex(Vector2f(0,0), sf::Color::Red),
		Vertex(Vector2f(sWidth,0), sf::Color::Blue),
		Vertex(Vector2f(sWidth,sHeight), sf::Color::Green),
		Vertex(Vector2f(0,sHeight), sf::Color::Magenta)
	};

	std::vector<Laser *> lasers;
	std::vector<Asteroid *> asteroids;

	sf::Texture asteroidTexture;

	sf::Text fpsText;
	sf::Font font;

	sf::Clock clock;

	void initGame() {
		if (!font.loadFromFile("arial.ttf")) {
			std::cout << "Couldn't load font!" << std::endl;
		}
		fpsText.setFont(font);
		fpsText.setCharacterSize(50);
		fpsText.setFillColor(sf::Color::Green);
		fpsText.setPosition(10, 10);

		//physics_add_object(&player);
		//lasers.reserve(1000);
		//for (int i = 0; i < 50; i++) {
		//	Laser * l = new Laser();
		//	lasers.push_back(l);
		//	physics_add_object(l);
		//}

		asteroidTexture.loadFromFile("rock.jpg");
		for (int i = 0; i < 2000; i++) {
			Asteroid * a = new Asteroid(10 + rand() % 40, &asteroidTexture);
			asteroids.push_back(a);
			physics_add_object(a);
		}

		////Create all asteroids
		
		//
		//int numRocks = 50;
		//asteroidEntity->numAsteroids = numRocks;
		//for (int i = 0; i < numRocks; i++) {
		//	sf::CircleShape* shape = new sf::CircleShape;
		//	shape->setRadius(10 + rand() % 40);
		//	shape->setOrigin(shape->getRadius(), shape->getRadius());
		//	shape->setPosition(rand() % sWidth, 100);
		//	shape->setTexture(&asteroidTexture);
		//	shapeListForDrawing.push_back(shape);
		//	asteroidEntity->shapes.push_back(shape);

		//	CollisionComponent c;
		//	c.name = "Asteroid";
		//	c.shape = shape;
		//	c.oldPos = shape->getPosition();
		//	asteroidEntity->collisionComponents.push_back(physics_add_object(c));

		//	AsteroidComponent a;
		//	a.health = shape->getRadius() * shape->getRadius();
		//	a.speed = 50 + (rand() % 100);
		//	asteroidEntity->asteroidComponents.push_back(a);
		//}
		//asteroidSystem.initialize(asteroidEntity);

	}

	AppState* GameLevel::update(float dt) {
		//laserResetCurrent -= dt;
		//if (laserResetCurrent <= 0) {
		//	laserResetCurrent = laserResetMax;
		//	firing = false;
		//}
		

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			return new MainMenu();
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			window.close();
		}

		if (Mouse::isButtonPressed(Mouse::Left)) {
			if (firing == false) {
				for (int i = 0; i < lasers.size(); i++) {
					if (lasers[i]->lifetime <= 0) {
						lasers[i]->fire(player.dir, player.getCenter());
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

		for (int i = 0; i < objects.size(); ++i)
		{
			GameObject * obj = objects[i];
			//if (obj->name.compare("Asteroid") == 0) printf("Found Asteroid. Name: %s\n", obj->name.c_str());
			Vector2i curBucket =
				getBucket(obj->getCenter());
			obj->update(dt);
			Vector2i newBucket =
				getBucket(obj->getCenter());
			if (curBucket != newBucket)
			{
				bucket_remove(curBucket, obj);
				bucket_add(newBucket, obj);
			}
			detect_collisions(obj, newBucket);
		}

		//Vector2f targetCenter = player.getCenter();
		//Vector2f curCenter = viewMan.getView().getCenter();
		//Vector2f v = targetCenter - curCenter;
		//curCenter += v * 4.0f * dt;
		//viewMan.getView().setCenter(curCenter);
		//viewMan.getView().setCenter(player.getCenter());
		//cout << viewMan.getView().getCenter().x << endl;

		sf::Time elapsed = clock.restart();
		fpsText.setString(std::to_string(static_cast<int>(std::round(1.0f / elapsed.asSeconds()))));

		return nullptr;
	}

	

	void GameLevel::draw() {
		window.setView(viewMan.getView());
		window.clear();
		window.draw(bg, 4, sf::Quads);
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->draw();
		}

		//for (int i = 0; i < shapeListForDrawing.size(); i++) {
		//	window.draw(*shapeListForDrawing[i]);
		//}
		sf::Time elapsed = clock.restart();
		fpsText.setString(std::to_string(static_cast<int>(std::round(1.0f / elapsed.asSeconds()))));
		window.draw(fpsText);
	}

	GameLevel::GameLevel(int level) {
		initGame();
	}
}