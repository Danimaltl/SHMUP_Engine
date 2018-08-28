#include "ViewManager.h"
#include "Easings.h"
#include <iostream>

using namespace sf;

void ViewManager::startAnim(String direction){
	animTime = 0;
	animating = true;
	animBase  = view.getCenter();
	animTarget = view.getCenter() + Vector2f(0,-5);
}

void ViewManager::playAnim(float dt) {
	if (!animating) return;
	animTime += dt;
	//cout << animTime << endl;;
	if (animTime > .25) {
		animating = false;
		return;
	}

	sf::Vector2f newPos = view.getCenter();

	newPos.y = circEaseInOut(animTime, animTarget.y, animBase.y - animTarget.y, .25);
	newPos = newPos - view.getCenter();
	view.move(newPos);
}

