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
