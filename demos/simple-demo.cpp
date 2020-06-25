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

using namespace matgui;
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

    auto *button = window.addChild(make_unique<Button>("button 1"));
    auto *button2 = window.addChild(make_unique<Button>("close"));
    auto *knob = window.createChild<KnobView>(); // Create the child directly

    auto *progress = new ProgressView; // You can create objects as raw pointers
                                       // but i don't using 'new' in new code
    window.addChild(progress);         // Notice2 this is not recommended

    auto leftLabel = make_unique<Label>("left aligned");
    leftLabel->alignment(FontView::Left);
    window.addChild(move(leftLabel));

    auto rightLabel = make_unique<Label>("right aligned");
    rightLabel->alignment(FontView::Right);
    window.addChild(move(rightLabel));

    auto *textEntry = window.createChild<TextEntry>();
    textEntry->text("hej");

    textEntry->submit.connect(
        [textEntry]() { cout << "got text: " << textEntry->text() << endl; });

    auto layout2 = make_unique<LinearLayout>();
    layout2->orientation(LAYOUT_HORIZONTAL);
    layout2->createChild<Label>("standard label");
    layout2->createChild<ImageView>("gfx/test.png");
    layout2->createChild<ToggleView>();
    layout2->createChild<SliderView>();

    window.addChild(move(layout2));

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
