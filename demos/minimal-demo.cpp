/*
 * minimal_demo.cpp
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/application.h"
#include "matgui/label.h"
#include "matgui/textentry.h"
#include "matgui/window.h"
#include <iostream>

using namespace matgui;

int main(int argc, char **argv) {
    Application app(argc, argv);

    Window window("matgui", 200, 200);
    window.createChild<Label>("enter text:");
    auto *textEntry = window.createChild<TextEntry>();

    textEntry->submit.connect(
        [textEntry]() { std::cout << textEntry->text() << std::endl; });

    app.mainLoop();
}
