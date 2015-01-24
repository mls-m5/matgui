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
#include "imageview.h"
#include "knobview.h"
#include "progressview.h"

using namespace MatGui;
using namespace std;

ProgressView *progressView;
void callback(View::pointerArgument arg) {
	cout << "callback... x = " << arg.x << endl;
}


void knobCallback(double value) {
//	cout << "changed... " << value << endl;
	progressView->value(value);
}

int main(int argc, char *argv[])
{
	Application app(argc, argv); //Initialize stuff

	auto window = new Window("matgui-demo"); //create the window

	auto button = new Button("knapp 1");
	auto button2 = new Button("stäng");
	auto knob = new KnobView;
	auto progress = new ProgressView;

	progressView = progress;

	window->addChild(button);
	window->addChild(knob);
	window->addChild(button2);
	window->addChild(progress);

	auto layout2 = new LinearLayout;
	layout2->setOrientation(LAYOUT_HORIZONTAL);
	layout2->addChild(new Button("knapp"));
	layout2->addChild(new ImageView("gfx/test.png"));

	window->addChild(layout2);

	button->clicked.connect(callback); //callback to a function
	button2->clicked.connect(&app, &Application::quit); //A demo just to show how signals work
	knob->changed.connect(knobCallback);

	knob->value(.3);

	app.mainLoop();

	//Application will delete all remaining windows
	//The windows delete all views that is attached to it

    return 0;
}
