/*
 * windowdata.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "../lib/sdlpp/include/sdlpp/window.hpp"
#include "matgui/window.h"
#include <SDL2/SDL_video.h> //For SDL_GLContext

namespace matgui {

struct Window::WindowData {
    sdl::Window window;
    SDL_GLContext context = 0; // OpenGl context handle
    Uint32 windowId = 0;
    //! Invalid: The window needs redrawing (when continuous updates = false)
    bool invalid = true;
    View *focused = nullptr;
};

} // namespace matgui
