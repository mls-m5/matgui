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
		clicked.connect(this, &Game::viewClicked);
		rectStyle.fill.color(1,1,1);
	}

	~Game() {
		clicked.disconnect(this);
	}

	void draw() override {
		rectStyle.drawRect(
				_x + _width / 2.,
				_y + _height / 2.,
				20,
				20
		);
		drawTextureRect(_x + _textureX, _y + _textureY, _angle / 2 + 3, 60, 30, texture, DrawStyle::CenterOrigo);
	}

	void frameCallback(double t){
		_angle += _rotation * t;
	}

	void buttonClicked() {
		_rotation = -_rotation;
	}

	void viewClicked(View::PointerArgument arg) {
		_textureX = arg.x;
		_textureY = arg.y;
	}


	//Called from layout
	void refresh() override {
		_textureX = _width / 2;
		_textureY = _height / 2;
	}

	double _textureX = 0, _textureY = 0;
	double _angle = 0;
	double _rotation = 1;
	Texture texture;
	Paint rectStyle;
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
