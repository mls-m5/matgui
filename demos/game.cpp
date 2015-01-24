/*
 * game.cpp
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#include "application.h"
#include "window.h"
#include "button.h"
#include "draw.h"
#include <iostream>

using namespace MatGui;
using namespace std;

class Game : public View {
public:

	Game() {
	}

	void draw() override {
		drawSquare({_x + _width / 2., _y + _height / 2.}, _angle, 20, 20, DRAW_STYLE_FILLED);
	}

	void frameCallback(){
		_angle += _rotation;
	}

	void buttonClicked() {
		_rotation = -_rotation;
	}

	double _angle = 0;
	double _rotation = .1;
};

int main(int argc, char**argv) {
	Application app(argc, argv);

	Window *window = new Window("mattias");
	Button *button = new Button("turn rotation");
	button->weight(.3);
	auto game = new Game;

	window->addChild(button);
	window->addChild(game);

	app.frame.connect(game, &Game::frameCallback);
	button->clicked.connect(game, &Game::buttonClicked);

	app.mainLoop();

}
