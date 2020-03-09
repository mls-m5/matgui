/*
 * simple-demo.cpp
 *
 *  Created on: 22 jan 2014
 *      Author: Mattias Lasersköld
 */

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

using namespace MatGui;
using namespace std;

// Example callback function
ProgressView *progressView;
void callback(View::PointerArgument arg) {
    cout << "callback... x = " << arg.x << endl;
}

// Another callback example
void knobCallback(double value) {
    progressView->value(value);
}

void keyDownCallback(View::KeyArgument arg) {
    cout << "keypress " << (char)arg.symbol << endl;
}

int main(int argc, char *argv[]) {
    Application app(argc, argv); // Initialize stuff

    Window window("matgui-demo");

    auto button = new Button("knapp 1");
    auto button2 = new Button("stäng");
    auto knob = new KnobView;
    auto progress = new ProgressView;

    progressView = progress;

    window.addChild(button);
    window.addChild(knob);
    window.addChild(button2);
    window.addChild(progress);

    auto leftLabel = new Label("left aligned");
    leftLabel->alignment(FontView::Left);
    window.addChild(leftLabel);

    auto rightLabel = new Label("right aligned");
    rightLabel->alignment(FontView::Right);
    window.addChild(rightLabel);

    auto textEntry = new TextEntry;
    textEntry->text("hej");
    window.addChild(textEntry);
    textEntry->submit.connect(
        [textEntry]() { cout << "got text: " << textEntry->text() << endl; });

    auto layout2 = new LinearLayout;
    layout2->orientation(LAYOUT_HORIZONTAL);
    layout2->addChild(new Label("standard label"));
    layout2->addChild(new ImageView("gfx/test.png"));
    layout2->addChild(new ToggleView());
    layout2->addChild(new SliderView());

    window.addChild(layout2);

    button->clicked.connect(callback); // callback to a function
    button2->clicked.connect(
        Application::quit); // A demo just to show how signals work

    knob->changed.connect(knobCallback);
    // To specify argument is optional
    // as can be seen from these two examples
    knob->changed.connect([](double value) {
        cout << "lambda function called from signal with value " << value
             << endl;
    });
    knob->changed.connect([]() {
        cout << "lambda function called to function without specified arguments"
             << endl;
    });

    window.keyDown.connect(keyDownCallback);

    knob->value(.3);

    app.mainLoop();

    // Application will delete all remaining windows
    // The windows delete all views that is attached to it

    return 0;
}
