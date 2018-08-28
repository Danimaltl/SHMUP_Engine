#pragma once

#include <vector>
#include "Globals.h"
#include "dcRenderer.h"

class AppState {
public:
	virtual ~AppState() { }
	virtual AppState* update(float dt) = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
};

class MainMenu :public AppState {
private:
	dcRender::TextComponent title;
	dcRender::TextComponent start;
	dcRender::TextComponent quit;

	dcRender::TextRenderer textRenderer;

public:
	MainMenu();
	virtual ~MainMenu();
	void initMenu();
	virtual AppState* update(float dt);
	virtual void draw();
	virtual void destroy();
};