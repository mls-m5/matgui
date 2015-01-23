/*
 * window.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "window.h"
#include "draw.h"
#include "windowdata.h"
#include <iostream>
#include <SDL2/SDL.h>

#include "application.h"
using std::cout; using std::endl;
using std::string;

namespace MatGui {



void checkSDLError(int line = -1)
{
#ifndef NDEBUG
	const char *error = SDL_GetError();
	if (*error != '\0')
	{
		cout << "SDL Error: " << error << endl;
		if (line != -1)
			cout << " + line: " << line << endl;
		SDL_ClearError();
	}
#endif
}


Window::Window(string title) {
	windowData = new WindowData;

    // Create our window centered at 512x512 resolution
    windowData->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
//    if (!windowData->mainwindow) /* Die if creation failed */
//        sdldie("Unable to create window");
//
    checkSDLError(__LINE__);

    windowData->windowId = SDL_GetWindowID(windowData->window);

    // Create our opengl context and attach it to our window
    windowData->context = SDL_GL_CreateContext(windowData->window);
    checkSDLError(__LINE__);

    initDrawModule(512, 512);
    setLocation(0,0, 512, 512);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    Application::addWindow(this);
}

Window::~Window() {
    SDL_GL_DeleteContext(windowData->context);
    SDL_DestroyWindow(windowData->window);
	delete windowData;
}

} /* namespace MatGui */
