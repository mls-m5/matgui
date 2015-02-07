/*
 * windowdata.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

namespace MatGui {

struct WindowData {
	SDL_Window *window = 0; //Handle to sdl window
    SDL_GLContext context = 0; //OpenGl context handle
    Uint32 windowId = 0;
};

}

