Matgui
=======================================

Matgui is a simple gui that aims to
* Be platform independent (linux, windows, mac, android, iphone/ipad, web)
* Have a minimal set of dependencies
* Easy to import in project
* Implement signals and slots to do callback
* Thread safe
* Super easy to build and used (compared to gtk, qt, ect)
* Easy to add opengl-graphics to program

Matgui is for the moment based on SDL2 and opengl 3 or 2 (depending on platform)

Design
-------
Views have a property named parent. If this is set when the parent is deleted, the view will delete itself from its parent

Layout automatically sets the parent-property when a child view is added


example usage:
--------

### Minimal example
``` c++

#include "matgui/application.h"
#include "matgui/label.h"
#include "matgui/textentry.h"
#include "matgui/window.h"

#include <iostream>

using namespace MatGui;

int main(int argc, char **argv) {
    Application app(argc, argv);

    Window window("matgui", 200, 200);
    window.createChild<Label>("enter text:");
    auto *textEntry = window.createChild<TextEntry>();

    textEntry->submit.connect(
        [textEntry]() { std::cout << textEntry->text() << std::endl; });

    app.mainLoop();
}


```


### A example with callbacks
``` c++

#include "matgui/application.h"
#include "matgui/button.h"
#include "matgui/imageview.h"
#include "matgui/knobview.h"
#include "matgui/label.h"
#include "matgui/progressview.h"
#include "matgui/sliderview.h"
#include "matgui/textentry.h"
#include "matgui/toggleview.h"
#include "matgui/window.h"

#include <iostream>
#include <memory>

using namespace matgui;

// Example callback function
ProgressView *progressView = nullptr;
void callback(View::PointerArgument arg) {
    std::cout << "callback... x = " << arg.x << std::endl;
}

// Another callback example
void knobCallback(double value) {
    progressView->value(value);
}

void keyDownCallback(View::KeyArgument arg) {
    std::cout << "keypress " << (char)arg.symbol << std::endl;
}

int main(int argc, char *argv[]) {
    Application app(argc, argv); // Initialize stuff

    Window window("matgui-demo");

    auto *button = window.addChild(std::make_unique<Button>("button 1"));
    auto *button2 = window.addChild(std::make_unique<Button>("close"));
    auto *knob = window.createChild<KnobView>(); // Create the child directly

    auto progress = std::make_unique<ProgressView>();

    ::progressView = progress.get();

    window.addChild(std::move(progress));

    auto leftLabel = std::make_unique<Label>("left aligned");
    leftLabel->alignment(FontView::Left);
    window.addChild(std::move(leftLabel));

    auto rightLabel = std::make_unique<Label>("right aligned");
    rightLabel->alignment(FontView::Right);
    window.addChild(std::move(rightLabel));

    auto *textEntry = window.createChild<TextEntry>();
    textEntry->text("hej");

    textEntry->submit.connect([textEntry]() {
        std::cout << "got text: " << textEntry->text() << std::endl;
    });

    auto layout2 = std::make_unique<LinearLayout>();
    layout2->orientation(LAYOUT_HORIZONTAL);
    layout2->createChild<Label>("standard label");
    layout2->createChild<ImageView>("gfx/test.png");
    layout2->createChild<ToggleView>();
    layout2->createChild<SliderView>();

    window.addChild(std::move(layout2));

    button->clicked.connect(callback); // callback to a function
    button2->clicked.connect(
        Application::quit); // A demo just to show how signals work

    knob->changed.connect(knobCallback);
    // To specify argument is optional
    // as can be seen from these two examples
    knob->changed.connect([](double value) {
        std::cout << "lambda function called from signal with value " << value
                  << std::endl;
    });
    knob->changed.connect([]() {
        std::cout
            << "lambda function called to function without specified arguments"
            << std::endl;
    });

    window.keyDown.connect(keyDownCallback);

    knob->value(.3);

    app.mainLoop();

    // Application will delete all remaining windows
    // The windows delete all views that is attached to it

    return 0;
}
```

Buildflags
--------

### Example build flags

``` bash

CXXFLAGS = -std=c++11 -Iinclude/ -Imatgui/include/matgui
LIBS = -lGL -lSDL2 -lSDL2_ttf -lSDL2_image

CXXFLAGS += -DUSE_TTF_FONT #This flag will use libsdl2-ttf fonts instead of the built in bitmap fonts.
```

### Build with [matmake](https://github.com/mls-m5/matmake)
``` bash
config += c++14

includes +=
   include
   matgui/include

main.src +=
   src/*.cpp           # project source files
   matgui/matgui.cpp   # matgui source files

main.libs +=
   -lSDL2
   -lGL
   -lSDL2_ttf
   -lSDL2_image

```

### Build demos with cmake

```

```

For compilation with emscripten, to compile to javascript these flags are needed.

``` bash
-s USE_SDL=2 -s FULL_ES2=1 -s USE_WEBGL2=1

-s USE_SDL_IMAGE=2 # Needed if using sdl2-image

```

It is currently in early development phase, please contact me if you want more information if how to use it
