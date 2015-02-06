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
	_windowData = new WindowData;

    // Create our window centered at 512x512 resolution
    _windowData->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        512, 512, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
//    if (!windowData->mainwindow) /* Die if creation failed */
//        sdldie("Unable to create window");
//
    checkSDLError(__LINE__);

    _windowData->windowId = SDL_GetWindowID(_windowData->window);

    // Create our opengl context and attach it to our window
    _windowData->context = SDL_GL_CreateContext(_windowData->window);
    checkSDLError(__LINE__);

    setLocation(0,0, 512, 512);
    initDrawModule(_width, _height); //Init the drawmodule for the CURRENT CONTEXT

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    Application::addWindow(this);
}

Window::~Window() {
    SDL_GL_DeleteContext(_windowData->context);
    SDL_DestroyWindow(_windowData->window);
	delete _windowData;
    Application::removeWindow(this);
}

void Window::draw() {
	SDL_GL_MakeCurrent(_windowData->window, _windowData->context);
	setDimensions(_width, _height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Layout::draw();
	SDL_GL_SwapWindow(_windowData->window);
}

bool Window::onRequestClose() {
	return closeSignal.directCall();
}


void Window::show() {
	SDL_ShowWindow(_windowData->window);
	Application::addWindow(this);
}

void Window::hide() {
	SDL_HideWindow(_windowData->window);
    Application::removeWindow(this);
}

} /* namespace MatGui */

