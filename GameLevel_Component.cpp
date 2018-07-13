#include "GameLevel.h"
#include "Ship.h"
#include "Components.h"
#include <memory>

namespace game_component {

	/*sf::Shape version*/

	//const int MAX_COLLIDERS = 300;
	//CollisionComponent colliders[MAX_COLLIDERS];
	//CollisionComponent* collidersPtr = colliders;
	//int collidersSize = 0;

	std::vector<CollisionComponent*> colliders;

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
		obj->collided = false;
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
						bool hit = checkCollision(obj->shape->getGlobalBounds(), o->shape->getGlobalBounds());
						if (hit) {
							obj->collided = true;
							obj->otherName = o->name;
						}
					}
				}
			}
		}
	}

	CollisionComponent* physics_add_object(CollisionComponent* obj) {
		//assert(collidersSize < MAX_COLLIDERS);
		/*colliders[collidersSize] = obj;
		CollisionComponent* objPtr = &colliders[collidersSize];*/
		colliders.push_back(obj);
		sf::Vector2i coord = getBucket(obj->shape->getPosition());
		bucket_add(coord, obj);
		return obj;
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

	/* Parallel Component Arrays */
	const int shapeListSize = 5000;
	CircleShape* shapeListForDrawing = new CircleShape[5000];
	int shapeListCount = 0;

	sf::Texture asteroidTexture;

	AsteroidSystem asteroidSystem;
	AsteroidEntity* asteroidEntity = new AsteroidEntity;

	sf::Text fpsText;
	sf::Text title;
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

		title.setFont(font);
		title.setCharacterSize(50);
		title.setFillColor(sf::Color::Green);
		title.setString("Component Version");
		title.setPosition(sWidth/2 - title.getLocalBounds().width / 2 + 10, 10);

		//Create all asteroids
		asteroidTexture.loadFromFile("rock.jpg");

		int numRocks = 2000;
		asteroidEntity->numAsteroids = numRocks;
		for (int i = 0; i < numRocks; i++) {
			CircleShape shape;
			shape.setRadius(10 + rand() % 40);
			shape.setOrigin(shape.getRadius(), shape.getRadius());
			shape.setPosition(rand() % sWidth, 100);
			shape.setTexture(&asteroidTexture);

			shapeListForDrawing[shapeListCount] = shape;
			CircleShape* shapeptr = &shapeListForDrawing[shapeListCount];
			shapeListCount++;
			

			asteroidEntity->shapes.push_back(shapeptr);

			CollisionComponent* c = new CollisionComponent;
			c->name = "Asteroid";
			c->shape = shapeptr;
			c->oldPos = shape.getPosition();
			asteroidEntity->collisionComponents.push_back(physics_add_object(c));

			AsteroidComponent a;
			a.health = shape.getRadius() * shape.getRadius();
			a.speed = 50 + (rand() % 100);
			asteroidEntity->asteroidComponents.push_back(a);
		}
		asteroidSystem.initialize(asteroidEntity);

	}

	AppState* GameLevel::update(float dt) {

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			return new MainMenu();
		}
		if (Keyboard::isKeyPressed(Keyboard::Q)) {
			window.close();
		}

		//update positions
		asteroidSystem.updatePositions(dt);

		////Check all collisions
		//for (int i = 0; i < colliders.size(); ++i) {
		//	CollisionComponent * collision = colliders[i];
		//	Vector2i curBucket = getBucket(collision->oldPos);
		//	Vector2i newBucket = getBucket(collision->shape->getPosition());
		//	if (curBucket != newBucket) {
		//		bucket_remove(curBucket, collision);
		//		bucket_add(newBucket, collision);
		//	}
		//	detect_collisions(collision, newBucket);
		//}

		////handle collisions
		//asteroidSystem.handleCollisions();

		return nullptr;
	}



	void GameLevel::draw() {
		window.setView(viewMan.getView());
		window.clear();
		window.draw(bg, 4, sf::Quads);

		for (int i = 0; i < shapeListCount; i++) {
			window.draw(shapeListForDrawing[i]);
		}

		sf::Time elapsed = clock.restart();
		fpsText.setString(std::to_string(static_cast<int>(std::round(1.0f / elapsed.asSeconds()))));
		window.draw(fpsText);
		window.draw(title);
	}

	GameLevel::GameLevel(int level) {
		initGame();
	}
}