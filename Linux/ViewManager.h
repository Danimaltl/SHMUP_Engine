#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

//Controls the view of a window
class ViewManager {
public:
	ViewManager(View _view) { view = _view;}
	void setView(View _view) { view = _view; }
	View& getView() { return view; }

	void playAnim(float dt);
	void startAnim(String direction);
	bool animating;
private:
	View view;

	float animTime = 0;
	Vector2f animTarget;
	Vector2f animBase;

	void playAnimLeft();
	void playAnimRight();
	void playAnimDown();
	void playAnimUp();
};
