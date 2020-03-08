/*
 * windowdata.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h> //For SDL_GLContext

namespace MatGui {

struct Window::WindowData {
    SDL_Window *window = 0;    // Handle to sdl window
    SDL_GLContext context = 0; // OpenGl context handle
    Uint32 windowId = 0;
    //! Invalid: The window needs redrawing (when continuous updates = false)
    bool invalid = true;
    View *_focused = nullptr;
};

} // namespace MatGui
