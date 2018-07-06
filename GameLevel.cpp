#include "GameLevel.h"
#include "Ship.h"
#include <memory>

namespace game {

	vector<GameObject *> objects;

	/*Bucket Grid Code from Walt's slides*/
	const float BUCKET_WIDTH = 100;
	const float BUCKET_HEIGHT = 100;
	const int COLUMNS = 10;
	const int ROWS = 10;
	vector<GameObject*> grid[COLUMNS][ROWS];

	Vector2i getBucket(Vector2f pos) {
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

		return Vector2i(col, row);
	}

	void bucket_add(Vector2i b,
		GameObject* obj)
	{
		vector<GameObject*> & v
			= grid[b.x][b.y];

		v.push_back(obj);
	}

	void bucket_remove(Vector2i b,
		GameObject* obj)
	{
		vector<GameObject*> & v
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
		int left = max(b.x - 1, 0);
		int right = min(b.x + 1, COLUMNS - 1);
		int top = max(b.y - 1, 0);
		int bot = min(b.y + 1, ROWS - 1);

		for (int bx = left; bx <= right; ++bx)
		{
			for (int by = top; by <= bot; ++by)
			{
				vector<GameObject*> & v = grid[bx][by];
				for (GameObject* o : v)
				{
					if (o != obj)
						obj->checkCollisionWith(o);
				}
			}
		}
	}

	/* *************** */

	bool firing = true;
	Ship player;
	int score = 0;
	int lives = 0;
	float laserResetMax = .05f;
	float laserResetCurrent = 0;

	Vertex bg[] = {
		Vertex(Vector2f(-(int)sWidth * 1.5,-(int)sHeight * 1.5), sf::Color::Red),
		Vertex(Vector2f(sWidth * 1.5,-(int)sHeight * 1.5), sf::Color::Blue),
		Vertex(Vector2f(sWidth * 1.5,sHeight * 1.5), sf::Color::Green),
		Vertex(Vector2f(-(int)sWidth * 1.5,sHeight * 1.5), sf::Color::Magenta)
	};

	vector<Laser *> lasers;
	vector<Asteroid *> asteroids;

	void initGame() {
		objects.push_back(&player);
		lasers.reserve(1000);
		for (int i = 0; i < 50; i++) {
			Laser * l = new Laser();
			lasers.push_back(l);
			objects.push_back(l);
		}



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
			cout << laserResetCurrent << endl;
		}
		else if (!Mouse::isButtonPressed(Mouse::Left)) {
			laserResetCurrent = laserResetMax;
			firing = false;
		}

		for (int i = 0; i < objects.size(); ++i)
		{
			GameObject * obj = objects[i];
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

		return nullptr;
	}

	

	void GameLevel::draw() {
		window.setView(viewMan.getView());
		window.clear();
		window.draw(bg, 4, Quads);
		for (int i = 0; i < objects.size(); i++) {
			objects[i]->draw();
		}
	}

	GameLevel::GameLevel(int level) {
		initGame();
	}
}