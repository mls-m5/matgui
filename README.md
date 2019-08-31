#matgui

Matgui is a simple gui that aims to
* Be platform independent (linux, windows, mac, android, iphone/ipad)
* Have a minimal set of dependencies
* Easy to import in project
* Implement signals and slots to do callback
* Thread safe
* Super easy to build and used (compared to gtk, qt, ect)

Matgui is for the moment based on SDL2

Design
-------
Views have a property named parent. If this is set when the parent is deleted, the view will delete itself from its parent

Layout automatically sets the parent-property when a child view is added


example usage:
--------


```
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

	auto window = new Window("matgui-demo"); //create the window

	auto button = new Button("button");
	auto button2 = new Button;
	auto knob = new KnobView;

	window.addChild(button);
	window.addChild(knob);
	window.addChild(button2);
	
	window.style.fill.setColor(1, 1, 1); //Change the window style
	window.updateStyle();

	button->clicked.connect(callback); //callback to a function
	button2->clicked.connect(&app, &Application::quit); //A demo just to show how signals work

	app.mainLoop();
	
	//All views are automatically deleted when the containers are deleted

    return 0;
}
```

Buildflags
--------

Here is some example flags

```

CXXFLAGS = -std=c++11 -Iinclude/ -Imatgui/include/matgui
LIBS = -lGL -lSDL2 -lSDL2_ttf -lSDL2_image

CXXFLAGS += -DUSE_TTF_FONT //This flag will use libsdl2-ttf fonts instead of the built in bitmap fonts.
```


For compilation with emscripten, to compile to javascript these flags are needed.

```
-s USE_SDL=2 -s FULL_ES2=1 -s USE_WEBGL2=1

-s USE_SDL_IMAGE=2 // needed if using sdl2-image

```

It is currently in early development phase, please contact me if you want more information if how to use it
