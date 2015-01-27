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

#include "texture.h"
using namespace MatGui;
using namespace std;

class Game : public View {
public:

	Game() {
		texture.load("gfx/test.png");
	}

	void draw() override {
		drawRect({_x + _width / 2., _y + _height / 2.}, _angle, 20, 20, DrawStyle::Filled);
		drawTextureRect({_x + _width / 2, _y + _height / 2.}, _angle / 2 + 3, 60, 30, texture, DrawStyle::CenterOrigo);
	}

	void frameCallback(double t){
		_angle += _rotation * t;
	}

	void buttonClicked() {
		_rotation = -_rotation;
	}

	double _angle = 0;
	double _rotation = 1;
	Texture texture;
};

int main(int argc, char**argv) {
	Application app(argc, argv);

	Window *window = new Window("mattias");
	Button *button = new Button("turn rotation");
	button->weight(.3);
	auto game = new Game;

	window->addChild(button);
	window->addChild(game);

	app.frameUpdate.connect(game, &Game::frameCallback);
	button->clicked.connect(game, &Game::buttonClicked);

	app.mainLoop();

}
