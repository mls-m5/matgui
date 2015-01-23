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

	Window window("matgui-demo"); //create the window

	Button button("knapp 1");
	Button button2("stäng");
	KnobView knob;
	ProgressView progress;

	progressView = &progress;

	window.addChild(&button);
	window.addChild(&knob);
	window.addChild(&button2);
	window.addChild(&progress);

	LinearLayout layout2;
	layout2.setOrientation(LAYOUT_HORIZONTAL);
	layout2.addChild(new Button("knapp")); //notice memory leak
	layout2.addChild(new ImageView("gfx/test.png"));

	window.addChild(&layout2);

	button.clicked.connect(callback); //callback to a function
	button2.clicked.connect(&app, &Application::quit); //A demo just to show how signals work
	knob.changed.connect(knobCallback);

	app.mainLoop();

    return 0;
}
