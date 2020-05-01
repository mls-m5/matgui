/*
 * matgui.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#ifndef __ANDROID__

#include "matgui/application.h"
#include "matgui/draw.h"
#include "matgui/window.h"
#include "signal.h"
#include "windowdata.h"

#include <SDL2/SDL_events.h>
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;
using namespace MatGui;

namespace {

// Some global variables
std::list<Window *> windows;
Window *activeWindow = nullptr;
Application *application = nullptr;
float scale = 1.f;
bool continuousUpdates = false;
bool invalidateOnEvent = true;

bool running = false;
uint32_t lastTick = 0; // used to keep track of framerate

} // namespace

namespace MatGui {

Application::Application(int argc, char **argv) {
    application = this;
    MatSig::setMainThread();

    if (running == true) {
        throw std::runtime_error(
            "Application already created in other part of program");
    }

    for (int i = 0; i + 1 < argc; ++i) {
        if (argv[i] == string("--scale")) {
            ++i;
            scale = atof(argv[i]);
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Quit();
        throw std::runtime_error("could not initialize sdl video");
    }
}

void Application::quit() {
    running = false;
}

inline void Application::innerLoop() {
    auto newTime = SDL_GetTicks();
    auto passedTime = (double)(newTime - lastTick) / 1000.;
    lastTick = newTime;

    auto shouldRedraw = continuousUpdates;

    if (!shouldRedraw) {
        for (auto it : windows) {
            if (it->invalid()) {
                shouldRedraw = true;
            }
        }
    }

#ifndef __EMSCRIPTEN__
    if (!shouldRedraw && !continuousUpdates) {
        SDL_Delay(10); // Limit cpu usage on idle
    }
#endif

    for (auto it : windows) {
        if (continuousUpdates || it->invalid()) {
            it->clear();
            it->frameUpdate.directCall(passedTime);
            it->draw();
            it->swap();
            it->invalid(false);
        }
    }

    if (handleEvents()) {
        running = false;
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    MatSig::flushSignals();
}

void Application::mainLoop() {
    running = true;
    lastTick = SDL_GetTicks();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(innerLoop, 60, 1);
#else
    while (running) {
        innerLoop();
    }
#endif
}

Window *Application::getWindow(unsigned int w) {
    for (auto it : windows) {
        if (w == it->_windowData->windowId) {
            return it;
        }
    }
    return nullptr;
}

static void handleWindowEvents(Window *window,
                               const SDL_Event &event,
                               bool &redrawEvent) {
    switch (event.window.event) {
    case SDL_WINDOWEVENT_CLOSE:
        if (not window->onRequestClose()) {
            window->hide();
        }
        break;
    case SDL_WINDOWEVENT_LEAVE:
        window->onPointerLeave();
        break;
    case SDL_WINDOWEVENT_RESIZED:
        window->onResize(event.window.data1, event.window.data2);
        break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        activeWindow = window;
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        if (activeWindow == window) {
            activeWindow = nullptr;
        }
        break;
    default:
        redrawEvent = false;
    }
}

static void handleOtherEvents(Window *window,
                              SDL_Event &event,
                              bool &redrawEvent) {
    switch (event.type) {
    case SDL_MOUSEMOTION: {
        auto &e = event.motion;
        window->onPointerMove(
            0, (double)e.x / scale, (double)e.y / scale, e.state);
        break;
    }

    case SDL_MOUSEBUTTONDOWN: {
        auto &e = event.button;
        auto button =
            MouseButton(1 << (e.button - 1)); // Make one bit for each button
        window->onPointerDown(
            0, button, (double)e.x / scale, (double)e.y / scale);
        break;
    }

    case SDL_MOUSEBUTTONUP: {
        auto &e = event.button;
        auto button =
            MouseButton(1 << (e.button - 1)); // Make one bit for each button
        window->onPointerUp(
            0, button, (double)e.x / scale, (double)e.y / scale);
        break;
    }

    case SDL_MOUSEWHEEL: {
        auto &e = event.wheel;
        window->onScroll(0, (double)e.x / scale, (double)e.y / scale);
        break;
    }

    case SDL_KEYDOWN: {
        auto &key = event.key;
        window->onKeyDown(
            key.keysym.sym, key.keysym.scancode, key.keysym.mod, key.repeat);
        break;
    }

    case SDL_KEYUP: {
        auto &key = event.key;
        window->onKeyUp(
            key.keysym.sym, key.keysym.scancode, key.keysym.mod, key.repeat);
        break;
    }

    case SDL_TEXTINPUT: {
        auto &text = event.text;
        window->onTextInput(text.text);
        break;
    }

    default:
        redrawEvent = false;
        break;
    }
}

bool Application::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return true;
        }

        auto window = getWindow(event.window.windowID);
        if (window) {
            bool redrawEvent =
                invalidateOnEvent; // If the event should trigger a redraw
            if (event.type == SDL_WINDOWEVENT) {
                handleWindowEvents(window, event, redrawEvent);
            }
            else {
                handleOtherEvents(window, event, redrawEvent);
            }
            if (redrawEvent) {
                window->invalidate();
            }
        }
    }
    return false;
}

Application::~Application() {
#ifndef __EMSCRIPTEN__
    QuitDrawModule();

    auto tmpWindowList =
        windows; // windows will start removing themselves from list
    for (auto it : tmpWindowList) {
        delete it;
    }
    windows.clear();

    SDL_Quit();
#endif
}

void Application::addWindow(class Window *window) {
    windows.push_back(window);
}

void Application::removeWindow(class Window *window) {
    windows.remove(window);
    if (windows.empty()) {
        application->quit();
    }
}

float Application::Scale() {
    return scale;
}

void Application::Scale(float s) {
    scale = s;
}

void Application::ContinuousUpdates(bool state) {
    continuousUpdates = state;
}

void Application::InvalidateOnEvent(bool state) {
    invalidateOnEvent = state;
}

} // namespace MatGui

#endif //__ANDROID__ block
