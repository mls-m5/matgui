#matgui

Matgui is a simple gui that aims to
* Be platform independent (linux, windows, mac, android, iphone/ipad)
* Have a minimal set of dependencies
* Easy to import in project
* Implement signals and slots to do callback
* Thread safe
* Super easy to build and used (compared to gtk, qt, ect)



example usage:


```
#include <iostream>

#include "application.h"
#include "window.h"
#include "button.h"
#include <GL/gl.h>
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
```



It is currently in early development phase, please contact me if you want more information if how to use it
