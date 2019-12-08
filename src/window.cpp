/*
 * window.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/window.h"
#include "matgui/draw.h"
#include <iostream>
#include <SDL2/SDL.h>
#include "matgui/common-gl.h"

#include "matgui/application.h"

#include "windowdata.h"
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

void Window::title(string title) {
	SDL_SetWindowTitle(_windowData->window, title.c_str());
}


Window::Window(string title, int width, int height, bool resizable) {
	_windowData.reset(new WindowData);

#ifdef USING_GL2

//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

//    Turn on double buffering with a 24bit Z buffer.
//    You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0); //todo: consider using this when doing pure gui implementations to be able to update just parts of the screen

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create our window centered
    auto scale = Application::Scale();
    _windowData->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width * scale, height * scale, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | (resizable? SDL_WINDOW_RESIZABLE : 0));
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

    location(0,0, width, height);
    initDrawModule(this->width(), this->height()); //Init the drawmodule for the CURRENT CONTEXT

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    Application::addWindow(this);
}

Window::~Window() {
    SDL_GL_DeleteContext(_windowData->context);
    SDL_DestroyWindow(_windowData->window);
    Application::removeWindow(this);
}


void Window::clear() {
	SDL_GL_MakeCurrent(_windowData->window, _windowData->context);
	auto &color = currentStyle.fill;
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::draw() {
	Layout::draw();
}

void Window::swap() {
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

bool Window::setFullscreen(bool state, bool changeVideoMode) {
	if (state) {
		if (changeVideoMode) {
			return SDL_SetWindowFullscreen(_windowData->window, SDL_WINDOW_FULLSCREEN);
		}
		else {
			return SDL_SetWindowFullscreen(_windowData->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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

void Window::cursorVisibility(bool value) {
	SDL_ShowCursor(value);
}

void Window::setCursorPosition(int x, int y) {
	auto scale = Application::Scale();
	SDL_WarpMouseInWindow(_windowData->window, x * scale, y * scale);
}

std::pair<int, int> Window::getCursorPosition() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	auto scale = Application::Scale();
	return std::pair<int, int>(x / scale, y / scale);
}

bool Window::onResize(int width, int height) {
	auto scale = Application::Scale();
	setDimensions(width / scale, height / scale);
	Layout::location(0, 0, width / scale, height / scale);
	return true;
}

} /* namespace MatGui */
