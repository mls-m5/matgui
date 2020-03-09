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

using namespace MatGui;
using namespace std;

int main(int argc, char **argv) {
    Application app(argc, argv);

    Window window("matgui", 200, 200);

    window.addChild(new Label("enter text:"));

    TextEntry *textEntry = new TextEntry();
    textEntry->submit.connect(
        [textEntry]() { cout << textEntry->text() << endl; });

    window.addChild(textEntry);

    app.mainLoop();
}
