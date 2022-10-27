#pragma once

#include "application.h"
#include <memory>
#include <vector>

namespace matgui {
using Args = std::vector<std::string>;

template <typename T>
void matguiMain(int argc, char *argv[]) {
    auto app = matgui::Application{};
    auto main = std::make_unique<T>(Args{argv, argv + argc});
    app.mainLoop();
}

} // namespace matgui

// For example on how to use, check demos/main-demo.cpp
// Use this macro if you want your main loop to work in environments where the
// main function need to return before the main loop has stopped, like
// emscripten
#define MATGUI_MAIN(mainclass)                                                 \
    int main(int argc, char *argv[]) {                                         \
        matguiMain<mainclass>(argc, argv);                                     \
    }

// TODO: Add implementation for emscripten
