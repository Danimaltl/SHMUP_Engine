#pragma once

#include <vector>

class AppState {
public:
	virtual ~AppState() { printf("Destructor is being called.\n"); }
	virtual AppState* update(float dt) = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
};

class MainMenu :public AppState {
private:


public:
	MainMenu();
	virtual ~MainMenu();
	virtual AppState* update(float dt);
	virtual void draw();
	virtual void destroy();
};