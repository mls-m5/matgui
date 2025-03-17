/*
 * window.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/window.h"
#include "../lib/sdlpp/include/sdlpp/mouse.hpp"
#include "../lib/sdlpp/include/sdlpp/opengl.hpp"
#include "matgui/application.h"
#include "matgui/draw.h"
#include "windowdata.h"
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>

namespace {

void checkSDLError(int line = -1) {

#ifndef NDEBUG
    const char *error = SDL_GetError();
    if (*error != '\0') {
        std::cout << "SDL Error: " << error << "\n";
        if (line != -1)
            std::cerr << " + line: " << line << "\n";
        SDL_ClearError();
    }
#endif
}

} // namespace

namespace matgui {

void Window::title(std::string title) {
    SDL_SetWindowTitle(_windowData->window, title.c_str());
}

void Window::bordered(bool state) {
    SDL_SetWindowBordered(_windowData->window, static_cast<SDL_bool>(state));
}

Window::Window(std::string title, int width, int height, bool resizable) {
    _windowData = std::make_unique<WindowData>();

#ifdef USING_GL2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    // Request opengl 3.2 context.
    sdl::gl::setAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    sdl::gl::setAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    sdl::gl::setAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                          SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    sdl::gl::setAttribute(SDL_GL_DOUBLEBUFFER, 1);
    sdl::gl::setAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create our window centered
    auto scale = Application::Scale();
    _windowData->window =
        sdl::Window(title.c_str(),
                    SDL_WINDOWPOS_CENTERED,
                    SDL_WINDOWPOS_CENTERED,
                    width * scale,
                    height * scale,
                    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN |
                        (resizable ? SDL_WINDOW_RESIZABLE : 0));
    if (!_windowData->window) {
        throw std::runtime_error("failed to create sdl window");
    }
    _windowData->windowId = _windowData->window.id();

    checkSDLError(__LINE__);

    _windowData->context = SDL_GL_CreateContext(_windowData->window);
    checkSDLError(__LINE__);

    // Windows needs to make the context current

    if (SDL_GL_MakeCurrent(_windowData->window.get(), _windowData->context)) {
        throw std::runtime_error("could not make SDL GL context active");
    }

#ifdef USING_GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cout << "Problems with glewInit" << endl;
    }
#endif

    location(0, 0, width, height);
    initDrawModule(
        this->width(),
        this->height()); // Init the drawmodule for the CURRENT CONTEXT

    // This makes our buffer swap synchronized with the monitor's vertical
    // refresh
    sdl::gl::setSwapInterval(1);

    Application::instance()->addWindow(this);
}

Window::~Window() {
    Application::instance()->removeWindow(this);
}

void Window::clear() {
    SDL_GL_MakeCurrent(_windowData->window, _windowData->context);
    auto &color = currentStyle.fill;
    matgui::clear(color.r, color.g, color.b, color.a);
}

void Window::draw() {
    Layout::draw();
}

void Window::swap() {
    _windowData->window.swap();
}

bool Window::onRequestClose() {
    return closeSignal.directCall();
}

void Window::show() {
    _windowData->window.show();
    Application::instance()->addWindow(this);
}

void Window::hide() {
    _windowData->window.hide();
    Application::instance()->removeWindow(this);
}

bool Window::fullscreen(bool state, bool changeVideoMode) {
    if (state) {
        if (changeVideoMode) {
            return _windowData->window.fullscreen(SDL_WINDOW_FULLSCREEN);
        }
        else {
            return _windowData->window.fullscreen(
                SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
    }
    else {
        return SDL_SetWindowFullscreen(_windowData->window, 0);
    }
}

bool Window::invalid() {
    return _windowData->invalid;
}

void Window::invalid(bool state) {
    _windowData->invalid = state;
}

void Window::focus(View *view) {
    if (_windowData->focused) {
        if (_windowData->focused == view) {
            return; // Already focused
        }
        else {
            _windowData->focused->onUnfocus();
        }
    }
    _windowData->focused = view;
    if (view) {
        view->onFocus();
    }
}

View *Window::focused() {
    return _windowData->focused;
}

void Window::unfocus(const View *view) {
    if (_windowData->focused == view) {
        focus(nullptr);
    }
}

void Window::cursorVisibility(bool value) {
    sdl::showCursor(value);
}

void Window::cursorPosition(int x, int y) {
    auto scale = Application::Scale();
    _windowData->window.warpMouse(x * scale, y * scale);
}

std::pair<int, int> Window::cursorPosition() {
    auto state = sdl::getMouseState();
    auto scale = Application::Scale();
    return std::pair<int, int>(state.x / scale, state.y / scale);
}

bool Window::onResize(int width, int height) {
    auto scale = Application::Scale();
    setDimensions(width / scale, height / scale);
    Layout::location(0, 0, width / scale, height / scale);
    resized.emit({width, height});
    return true;
}

bool Window::onKeyDown(KeySym sym,
                       KeyScanCode scancode,
                       KeyModifiers modifiers,
                       int repeat) {
    if (focused()) {
        if (focused()->onKeyDown(sym, scancode, modifiers, repeat)) {
            return true;
        }
    }
    return View::onKeyDown(sym, scancode, modifiers, repeat);
}

bool Window::onKeyUp(KeySym sym,
                     KeyScanCode scancode,
                     KeyModifiers modifiers,
                     int repeat) {
    if (focused()) {
        if (focused()->onKeyUp(sym, scancode, modifiers, repeat)) {
            return true;
        }
    }
    return View::onKeyUp(sym, scancode, modifiers, repeat);
}

bool Window::onTextInput(const char *text) {
    if (focused()) {
        if (focused()->onTextInput(text)) {
            return true;
        }
    }
    return View::onTextInput(text);
}

} // namespace matgui
