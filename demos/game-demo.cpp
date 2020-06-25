/*
 * game.cpp
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/application.h"
#include "matgui/button.h"
#include "matgui/draw.h"
#include "matgui/texture.h"
#include "matgui/window.h"

#include <iostream>

using namespace matgui;
using namespace std;

class Game : public View {
public:
    Game() {
        texture.load("gfx/test.png");
        clicked.connect(this, &Game::viewClicked);
        rectStyle.fill.color(1, 1, 1);
    }

    ~Game() {
        clicked.disconnect(this);
    }

    void draw() override {
        rectStyle.drawRect(x() + width() / 2., y() + height() / 2., 20, 20);
        drawTextureRect(x() + _textureX,
                        y() + _textureY,
                        _angle / 2 + 3,
                        60,
                        30,
                        texture,
                        DrawStyle::CenterOrigo);
    }

    void frameCallback(double t) {
        _angle += _rotation * t;
    }

    void buttonClicked() {
        _rotation = -_rotation;
    }

    void viewClicked(View::PointerArgument arg) {
        _textureX = arg.x;
        _textureY = arg.y;
    }

    // Called from layout
    void refresh() override {
        _textureX = width() / 2;
        _textureY = height() / 2;
    }

    double _textureX = 0, _textureY = 0;
    double _angle = 0;
    double _rotation = 1;
    Texture texture;
    Paint rectStyle;
};

int main(int argc, char **argv) {
    Application app(argc, argv);

    Application::ContinuousUpdates(true); // Update the screen constantly

    Window window("matgui game-demo");
    auto game = window.createChild<Game>();
    auto button = window.createChild<Button>("turn rotation");

    button->weight(.3);
    button->clicked.connect(game, &Game::buttonClicked);
    window.refresh();

    window.frameUpdate.connect(game, &Game::frameCallback);

    app.mainLoop();
}
