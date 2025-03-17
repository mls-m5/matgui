/*
 * matgui.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include <memory>
#include <stdexcept>
#ifndef __ANDROID__

#include "../lib/sdlpp/include/sdlpp/events.hpp"
#include "../lib/sdlpp/include/sdlpp/sdl.hpp"
#include "../lib/sdlpp/include/sdlpp/timer.hpp"
#include "matgui/application.h"
#include "matgui/draw.h"
#include "matgui/window.h"
#include "windowdata.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace matgui {

Application *Application::_instance = nullptr;

struct Application::Impl {
    // Some global variables
    std::list<Window *> windows;
    Window *activeWindow = nullptr;
    float scale = 1.f;
    bool continuousUpdates = false;
    bool invalidateOnEvent = true;
    bool running = false;
    uint32_t lastTick = 0; // used to keep track of framerate

    void handleWindowEvents(Window *window,
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

    void handleOtherEvents(Window *window,
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
            auto button = MouseButton(
                1 << (e.button - 1)); // Make one bit for each button
            window->onPointerDown(
                0, button, (double)e.x / scale, (double)e.y / scale);
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            auto &e = event.button;
            auto button = MouseButton(
                1 << (e.button - 1)); // Make one bit for each button
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
            window->onKeyDown(key.keysym.sym,
                              key.keysym.scancode,
                              key.keysym.mod,
                              key.repeat);
            break;
        }

        case SDL_KEYUP: {
            auto &key = event.key;
            window->onKeyUp(key.keysym.sym,
                            key.keysym.scancode,
                            key.keysym.mod,
                            key.repeat);
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
};

Application::Application(int argc, char **argv)
    : _impl{std::make_unique<Impl>()} {
    _instance = this;
    MatSig::setMainThread();

    if (_impl->running == true) {
        throw std::runtime_error(
            "Application already created in other part of program");
    }

    for (int i = 0; i + 1 < argc; ++i) {
        if (argv[i] == std::string("--scale")) {
            ++i;
            _impl->scale = atof(argv[i]);
        }
    }

    if (sdl::init(SDL_INIT_VIDEO) < 0) {
        sdl::quit();
        throw std::runtime_error("could not initialize sdl video");
    }
}

void Application::quit() {
    impl()->running = false;
}

inline void Application::innerLoop() {
    auto newTime = SDL_GetTicks();
    auto passedTime = (double)(newTime - _impl->lastTick) / 1000.;
    _impl->lastTick = newTime;

    auto shouldRedraw = _impl->continuousUpdates;

    if (!shouldRedraw) {
        for (auto it : _impl->windows) {
            if (it->invalid()) {
                shouldRedraw = true;
            }
        }
    }

#ifndef __EMSCRIPTEN__
    if (!shouldRedraw && !_impl->continuousUpdates) {
        if (!shouldRedraw) {
            SDL_Delay(10); // Limit cpu usage on idle
        }
    }
#endif

    for (auto it : _impl->windows) {
        if (_impl->continuousUpdates || it->invalid()) {
            it->clear();
            it->frameUpdate.directCall(passedTime);
            it->draw();
            it->swap();
            it->invalid(false);
        }
    }

    if (handleEvents()) {
        _impl->running = false;
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    MatSig::flushSignals();
}

Application::Impl *Application::impl() {
    return instance()->_impl.get();
}

void Application::mainLoop() {
    _impl->running = true;
    _impl->lastTick = sdl::getTicks();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(innerLoop, 0, 1);
#else
    while (_impl->running) {
        innerLoop();
    }
#endif
}

Window *Application::getWindow(unsigned int w) {
    for (auto it : _impl->windows) {
        if (w == it->_windowData->windowId) {
            return it;
        }
    }
    return nullptr;
}

bool Application::handleEvents() {
    while (auto optEvent = sdl::pollEvent()) {
        auto event = *optEvent;
        if (event.type == SDL_QUIT) {
            return true;
        }

        auto window = getWindow(event.window.windowID);
        if (window) {
            bool redrawEvent =
                _impl
                    ->invalidateOnEvent; // If the event should trigger a redraw
            if (event.type == SDL_WINDOWEVENT) {
                _impl->handleWindowEvents(window, event, redrawEvent);
            }
            else {
                _impl->handleOtherEvents(window, event, redrawEvent);
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
        _impl->windows; // windows will start removing themselves from list
    for (auto it : tmpWindowList) {
        delete it;
    }
    _impl->windows.clear();

    sdl::quit();
#endif
}

void Application::addWindow(class Window *window) {
    _impl->windows.push_back(window);
}

void Application::removeWindow(class Window *window) {
    _impl->windows.remove(window);
    if (_impl->windows.empty()) {
        _instance->quit();
    }
}

float Application::Scale() {
    return impl()->scale;
}

void Application::Scale(float s) {
    impl()->scale = s;
}

void Application::ContinuousUpdates(bool state) {
    impl()->continuousUpdates = state;
}

void Application::InvalidateOnEvent(bool state) {
    impl()->invalidateOnEvent = state;
}

Application *Application::instance() {
    [[unlikely]]
    if (!_instance) {
        throw std::runtime_error{
            "Trying to access Application instance that is null"};
    }
    return _instance;
}

} // namespace matgui

#endif //__ANDROID__ block
