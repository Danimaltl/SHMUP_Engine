#pragma once

#include <vector>

class AppState {
public:
	virtual AppState* update(float dt) = 0;
	virtual void draw() = 0;
};

class MainMenu :public AppState {
private:


public:
	MainMenu();
	virtual AppState* update(float dt);
	virtual void draw();
};