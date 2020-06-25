/*
 * minimal_demo.cpp
 *
 *  Created on: 6 feb 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/application.h"
#include "matgui/button.h"
#include "matgui/window.h"

using namespace matgui;

int main(int argc, char **argv) {
    Application app(argc, argv);

    Window window1("hej");
    Window window2("hej2");

    window1.createChild<Button>("hej 1");
    window2.createChild<Button>("hej 2");
    window2.createChild<Button>("hej 3");

    app.mainLoop();
}
