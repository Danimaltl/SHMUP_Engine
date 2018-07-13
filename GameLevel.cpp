#include "GameLevel.h"
#include "Ship.h"
#include "Components.h"
#include <memory>

namespace game {

	std::vector<GameObject *> objects;

	/*Bucket Grid Code from Walt's slides*/
	//const float BUCKET_WIDTH = 100;
	//const float BUCKET_HEIGHT = 100;
	//const int COLUMNS = 10;
	//const int ROWS = 10;
	//std::vector<GameObject*> grid[COLUMNS][ROWS];

	//sf::Vector2i getBucket(Vector2f pos) {
	//	int col = int(pos.x / BUCKET_WIDTH);
	//	if (col < 0)
	//		col = 0;
	//	else if (col >= COLUMNS)
	//		col = COLUMNS - 1;

	//	int row = int(pos.y / BUCKET_HEIGHT);
	//	if (row < 0)
	//		row = 0;
	//	else if (row >= ROWS)
	//		row = ROWS - 1;

	//	return sf::Vector2i(col, row);
	//}

	//void bucket_add(sf::Vector2i b,
	//	GameObject* obj)
	//{
	//	printf("Added %s to bucket.\n", obj->name.c_str());
	//	std::vector<GameObject*> & v
	//		= grid[b.x][b.y];

	//	v.push_back(obj);
	//}

	//void bucket_remove(Vector2i b,
	//	GameObject* obj)
	//{
	//	std::vector<GameObject*> & v
	//		= grid[b.x][b.y];

	//	for (int i = 0; i < v.size(); ++i)
	//	{
	//		if (v[i] == obj)
	//		{
	//			v.erase(v.begin() + i);
	//			break;
	//		}
	//	}
	//}

	//void detect_collisions(GameObject* obj,
	//	Vector2i b)
	//{
	//	int left = std::max(b.x - 1, 0);
	//	int right = std::min(b.x + 1, COLUMNS - 1);
	//	int top = std::max(b.y - 1, 0);
	//	int bot = std::min(b.y + 1, ROWS - 1);

	//	for (int bx = left; bx <= right; ++bx)
	//	{
	//		for (int by = top; by <= bot; ++by)
	//		{
	//			std::vector<GameObject*> & v = grid[bx][by];
	//			for (GameObject* o : v)
	//			{
	//				if (o != obj)
	//					obj->checkCollisionWith(o);
	//			}
	//		}
	//	}
	//}

	//void physics_add_object(GameObject* obj) {
	//	objects.push_back(obj);
	//	sf::Vector2i coord = getBucket(obj->getCenter());
	//	bucket_add(coord, obj);
	//}

	/* *************** */

	/*sf::Shape version*/

	//const int MAX_COLLIDERS = 300;
	//CollisionComponent colliders[MAX_COLLIDERS];
	//CollisionComponent* collidersPtr = colliders;
	//int collidersSize = 0;

	std::vector<CollisionComponent> colliders;

	const float BUCKET_WIDTH = 100;
	const float BUCKET_HEIGHT = 100;
	const int COLUMNS = 10;
	const int ROWS = 10;
	std::vector<CollisionComponent*> grid[COLUMNS][ROWS];

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
		CollisionComponent* obj)
	{
		try {
			std::vector<CollisionComponent*> & v
				= grid[b.x][b.y];

			v.push_back(obj);
		}
		catch (std::exception const& e) {
			std::cout << "Message: " << e.what() << "\n";

			/*
			* Note this is platform/compiler specific
			* Your milage may very
			*/
			std::cout << "Type:    " << typeid(e).name() << "\n";
		}
		
	}

	void bucket_remove(Vector2i b,
		CollisionComponent* obj)
	{
		std::vector<CollisionComponent*> & v
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

	bool checkCollision(const sf::FloatRect& box1, const sf::FloatRect& box2) {
		return box1.intersects(box2);
	}

	void detect_collisions(CollisionComponent* obj,
		Vector2i b)
	{
		obj->hitNames.clear();
		int left = std::max(b.x - 1, 0);
		int right = std::min(b.x + 1, COLUMNS - 1);
		int top = std::max(b.y - 1, 0);
		int bot = std::min(b.y + 1, ROWS - 1);

		for (int bx = left; bx <= right; ++bx)
		{
			for (int by = top; by <= bot; ++by)
			{
				std::vector<CollisionComponent*> & v = grid[bx][by];
				for (CollisionComponent* o : v)
				{
					if (o != obj) {
						bool hit = false;//checkCollision(obj->shape->getGlobalBounds(), o->shape->getGlobalBounds());
						if (hit) {
							obj->hitNames.push_back(o->name);
						}
					}
				}
			}
		}
	}

	CollisionComponent* physics_add_object(CollisionComponent obj) {
		//assert(collidersSize < MAX_COLLIDERS);
		/*colliders[collidersSize] = obj;
		CollisionComponent* objPtr = &colliders[collidersSize];*/
		colliders.push_back(obj);
		CollisionComponent* objPtr = &colliders.back();
		sf::Vector2i coord = getBucket(obj.shape->getPosition());
		bucket_add(coord, objPtr);
		return objPtr;
	}

	/* *************** */

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

	/* Parallel Component Arrays */
	std::vector<sf::CircleShape*> shapeListForDrawing;

	sf::Texture asteroidTexture;

	AsteroidSystem asteroidSystem;
	AsteroidEntity* asteroidEntity = new AsteroidEntity;

	void initGame() {
		//physics_add_object(&player);
		//lasers.reserve(1000);
		//for (int i = 0; i < 50; i++) {
		//	Laser * l = new Laser();
		//	lasers.push_back(l);
		//	physics_add_object(l);
		//}

		//for (int i = 0; i < 5; i++) {
		//	Asteroid * a = new Asteroid(10 + rand() % 40);
		//	asteroids.push_back(a);
		//	physics_add_object(a);
		//}

		//Create all asteroids
		asteroidTexture.loadFromFile("rock.jpg");
		
		int numRocks = 50;
		asteroidEntity->numAsteroids = numRocks;
		for (int i = 0; i < numRocks; i++) {
			sf::CircleShape* shape = new sf::CircleShape;
			shape->setRadius(10 + rand() % 40);
			shape->setOrigin(shape->getRadius(), shape->getRadius());
			shape->setPosition(rand() % sWidth, 100);
			shape->setTexture(&asteroidTexture);
			shapeListForDrawing.push_back(shape);
			asteroidEntity->shapes.push_back(shape);

			CollisionComponent c;
			c.name = "Asteroid";
			c.shape = shape;
			c.oldPos = shape->getPosition();
			asteroidEntity->collisionComponents.push_back(physics_add_object(c));

			AsteroidComponent a;
			a.health = shape->getRadius() * shape->getRadius();
			a.speed = 50 + (rand() % 100);
			asteroidEntity->asteroidComponents.push_back(a);
		}
		asteroidSystem.initialize(asteroidEntity);

	}

	AppState* GameLevel::update(float dt) {

		//auto it = objects.begin();
		//while (it != objects.end()) {
		//	auto obj = *it;
		//	if (obj->lifetime <= 0) {
		//		delete obj;
		//		obj = nullptr;
		//		it = objects.erase(it);
		//	}
		//	else ++it;
		//}

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

		//for (int i = 0; i < objects.size(); ++i)
		//{
		//	GameObject * obj = objects[i];
		//	//if (obj->name.compare("Asteroid") == 0) printf("Found Asteroid. Name: %s\n", obj->name.c_str());
		//	Vector2i curBucket =
		//		getBucket(obj->getCenter());
		//	obj->update(dt);
		//	Vector2i newBucket =
		//		getBucket(obj->getCenter());
		//	if (curBucket != newBucket)
		//	{
		//		bucket_remove(curBucket, obj);
		//		bucket_add(newBucket, obj);
		//	}
		//	detect_collisions(obj, newBucket);
		//}
		int foo = asteroidEntity->numAsteroids;
		//update positions
		asteroidSystem.updatePositions(dt);

		//Check all collisions
		for (int i = 0; i < colliders.size(); ++i) {
			CollisionComponent * collision = &colliders[i];
			Vector2i curBucket = getBucket(collision->oldPos);
			Vector2i newBucket = getBucket(collision->shape->getPosition());
			if (curBucket != newBucket) {
				bucket_remove(curBucket, collision);
				bucket_add(newBucket, collision);
			}
			detect_collisions(collision, newBucket);
		}

		//handle collisions
		asteroidSystem.handleCollisions();

		//Vector2f targetCenter = player.getCenter();
		//Vector2f curCenter = viewMan.getView().getCenter();
		//Vector2f v = targetCenter - curCenter;
		//curCenter += v * 4.0f * dt;
		//viewMan.getView().setCenter(curCenter);
		//viewMan.getView().setCenter(player.getCenter());
		//cout << viewMan.getView().getCenter().x << endl;

		return nullptr;
	}

	

	void GameLevel::draw() {
		window.setView(viewMan.getView());
		window.clear();
		window.draw(bg, 4, sf::Quads);
		//for (int i = 0; i < objects.size(); i++) {
		//	objects[i]->draw();
		//}

		for (int i = 0; i < shapeListForDrawing.size(); i++) {
			window.draw(*shapeListForDrawing[i]);
		}
	}

	GameLevel::GameLevel(int level) {
		initGame();
	}
}