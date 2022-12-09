#pragma once

#include "application.h"
#include <memory>
#include <vector>

namespace matgui {

template <typename T>
void matguiMain(int argc, char *argv[]) {
    auto app = matgui::Application{};
    auto main = T{argc, argv};
    app.mainLoop();
}

template <typename T>
void matguiMainWindow(int argc, char *argv[]) {
    auto app = matgui::Application{};
    auto mainWindow = std::make_unique<T>(argc, argv);
    app.mainLoop();
}

} // namespace matgui

// For example on how to use, check demos/main-demo.cpp
// Use this macro if you want your main loop to work in environments where the
// main function need to return before the main loop has stopped, like
// emscripten
// se main-demo.cpp for example
#define MATGUI_MAIN(mainclass)                                                 \
    int main(int argc, char *argv[]) {                                         \
        matguiMain<mainclass>(argc, argv);                                     \
    }

// TODO: Add implementation for emscripten

// The same as main above but the user provide a class that inherits from Window
// This way the user can easier access input events and draw callbacks by
// overriding functions on the window
#define MATGUI_MAINWINDOW(mainwindow)                                          \
    int main(int argc, char *argv[]) {                                         \
        matguiMainWindow<mainwindow>(argc, argv);                              \
    }
