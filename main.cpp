/*
 * View.cpp
 *
 *  Created on: 22 jan 2014
 *      Author: Mattias Lasersköld
 */
#include <iostream>

#include "application.h"
#include "window.h"
#include "button.h"
#include "draw.h"
#include "knobview.h"
#include "linearlayout.h"

using namespace MatGui;
using namespace std;

void callback(View::pointerArgument arg) {
	cout << "callback... x = " << arg.x << endl;
}

int main(int argc, char *argv[])
{
	Application app(argc, argv); //Initialize stuff

	Window window("matgui-demo"); //create the window

	Button button;
	Button button2;
	KnobView knob;

	window.addChild(&button);
	window.addChild(&knob);
	window.addChild(&button2);

	button.clicked.connect(callback); //callback to a function
	button2.clicked.connect(&app, &Application::quit); //A demo just to show how signals work

	app.mainLoop();

    return 0;
}
