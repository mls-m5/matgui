#include "matgui/application.h"
#include "matgui/button.h"
#include "matgui/label.h"
#include "matgui/scrollview.h"
#include "matgui/window.h"

using namespace matgui;

int main(int argc, char *argv[]) {
    auto app = Application{argc, argv};

    auto window = Window{"scrolled", 200, 200, true};

    auto scroll = window.createChild<ScrollView>();
    //    auto scroll = window.createChild<ScrollView>();

    scroll->createChild<Button>("hello");
    scroll->createChild<Button>("hello");
    scroll->createChild<Button>("hello");
    scroll->refresh();

    auto button = window.createChild<Button>("unfinished scroll demo");

    button->clicked.connect([&] { scroll->scrollY(scroll->scrollY() + 10); });

    app.mainLoop();

    return 0;
}
