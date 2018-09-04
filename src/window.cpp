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
#include "common-gl.h"

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


Window::Window(string title, int width, int height, bool resizable) {
	_windowData.reset(new WindowData);

    // Create our window centered at 512x512 resolution
    _windowData->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (resizable? SDL_WINDOW_RESIZABLE : 0));
//    if (!windowData->mainwindow) /* Die if creation failed */
//        sdldie("Unable to create window");
//
	_windowData->windowId = SDL_GetWindowID(_windowData->window);

    checkSDLError(__LINE__);

    // Create our opengl context and attach it to our window
    _windowData->context = SDL_GL_CreateContext(_windowData->window);
    checkSDLError(__LINE__);

	//Windows needs to make the context current
	if (SDL_GL_MakeCurrent(_windowData->window, _windowData->context)) {
		throw 10000000; //Throw random error message if failed
	}

	cout << "Supported version of OpenGl: " << glGetString(GL_VERSION) << endl;

	#ifdef USING_GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cout << "Problems with glewInit" << endl;
	}
	#endif

    setLocation(0,0, width, height);
    initDrawModule(_width, _height); //Init the drawmodule for the CURRENT CONTEXT

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    Application::addWindow(this);
}

Window::~Window() {
    SDL_GL_DeleteContext(_windowData->context);
    SDL_DestroyWindow(_windowData->window);
    Application::removeWindow(this);
}

void Window::draw() {
	SDL_GL_MakeCurrent(_windowData->window, _windowData->context);
//	setDimensions(_width, _height);
	auto &color = currentStyle.fill;
	glClearColor(color.r, color.g, color.b, color.a);
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


bool Window::onResize(int width, int height) {
	Layout::setLocation(0, 0, width, height);
	return true;
}

} /* namespace MatGui */
