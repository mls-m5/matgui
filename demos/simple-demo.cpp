/*
 * simple-demo.cpp
 *
 *  Created on: 22 jan 2014
 *      Author: Mattias Lasersköld
 */
#include <iostream>

#include "application.h" //for the application to start
#include "window.h"  //for windows view
#include "button.h"  //for button view
#include "imageview.h"
#include "knobview.h"
#include "progressview.h"
#include "label.h"
#include "toggleview.h"
#include "sliderview.h"

using namespace MatGui;
using namespace std;


//Example callback function
ProgressView *progressView;
void callback(View::PointerArgument arg) {
	cout << "callback... x = " << arg.x << endl;
}

//Another callback example
void knobCallback(double value) {
	progressView->value(value);
}

void keyDownCallback(View::KeyArgument arg) {
	cout << "keypress " << (char)arg.symbol << endl;
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
	layout2.addChild(new Label("etikett")); //You can also use pointers
	layout2.addChild(new ImageView("gfx/test.png"));
	layout2.addChild(new ToggleView());
	layout2.addChild(new SliderView());

	window.addChild(&layout2);

	button.clicked.connect(callback); //callback to a function
	button2.clicked.connect(&app, &Application::quit); //A demo just to show how signals work

	knob.changed.connect(knobCallback);
	knob.changed.connect([] (double) {
		cout << "lambda function called from signal" << endl;
	});
	window.keyDown.connect(keyDownCallback);

	knob.value(.3);

	app.mainLoop();

	//Application will delete all remaining windows
	//The windows delete all views that is attached to it

    return 0;
}
