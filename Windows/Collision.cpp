#include "Components.h"
#include "dcMath.h"

namespace collision {
	std::vector<CollisionComponent*> colliders;

	const float BUCKET_WIDTH = sWidth / 4;
	const float BUCKET_HEIGHT = sHeight / 6;
	const int COLUMNS = 10;
	const int ROWS = 10;
	std::vector<CollisionComponent*> grid[COLUMNS][ROWS];

	sf::Vector2i getBucket(glm::vec2 pos) {
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

	void bucket_add(sf::Vector2i b, CollisionComponent* obj)
	{
		std::vector<CollisionComponent*> & v
			= grid[b.x][b.y];

		v.push_back(obj);

	}

	void bucket_remove(Vector2i b, CollisionComponent* obj)
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

	bool checkCollisionRadius(float r1, float r2, glm::vec2 p1, glm::vec2 p2) {
		return dcMath::Magnitude(p1 - p2) < (r1 + r2);
	}

	void detect_collisions(CollisionComponent* obj, Vector2i b)
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
						bool hit = checkCollisionRadius(obj->radius, o->radius, obj->currPos, o->currPos);
						if (hit) {
							obj->collided = true;
							obj->otherName = o->name;
						}
					}
				}
			}
		}
	}

	CollisionComponent* add_object(CollisionComponent* obj) {
		//assert(collidersSize < MAX_COLLIDERS);
		/*colliders[collidersSize] = obj;
		CollisionComponent* objPtr = &colliders[collidersSize];*/
		colliders.push_back(obj);
		sf::Vector2i coord = getBucket(obj->oldPos);
		bucket_add(coord, obj);
		return obj;
	}

	void CheckAllCollisions() {
		//Check all collisions
		for (int i = 0; i < colliders.size(); ++i) {
			CollisionComponent * collision = colliders[i];
			if (collision->active == false) continue;
			Vector2i curBucket = getBucket(collision->oldPos);
			Vector2i newBucket = getBucket(collision->currPos);
			if (curBucket != newBucket) {
				bucket_remove(curBucket, collision);
				bucket_add(newBucket, collision);
			}
			detect_collisions(collision, newBucket);
		}
	}

	void DestroyAllColliders() {
		printf("Destroying all colliders.\n");
		for (int i = 0; i < colliders.size(); i++) {
			delete colliders[i];
			colliders[i] = nullptr;
		}
		colliders.clear();
		
		if (!colliders.empty()) {
			printf("Colliders isn't empty!\n");
		}

		for (int i = 0; i < COLUMNS; i++) {
			for (int j = 0; j < ROWS; j++) {
				grid[i][j].clear();
			}
		}

		for (int i = 0; i < COLUMNS; i++) {
			for (int j = 0; j < ROWS; j++) {
				if (!grid[i][j].empty()) {
					printf("grid[%d][%d] isn't empty!\n", i, j);
				}
			}
		}

 	}

}