/*
 * matgui.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include <SDL2/SDL.h>
#include <iostream>
#include "application.h"
#include "commonsdl.h"
#include "signal.h"
#include "SDL2/SDL_opengl.h"
#include "window.h"
#include "windowdata.h"
#include "draw.h"

using namespace std;

namespace MatGui {

static std::list<Window *> windows;

Application::Application(int argc, char** argv) {
	MatSig::setMainThread();
    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); //This prevents the rendering function from rendering anything of some reason

//    Turn on double buffering with a 24bit Z buffer.
//    You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0); //todo: consider using this when doing pure gui implementations to be able to update just parts of the screen

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void Application::mainLoop() {
	running = true;
	auto oldTime = SDL_GetTicks();
	while (running) { //todo: fix not constant updates
		auto newTime = SDL_GetTicks();
		auto passedTime = (double) (newTime - oldTime) / 1000.;
		oldTime = newTime;
		if (frameUpdate) {
			//Handle gives the developer the possibility to do things each frame
			frameUpdate.directCall(passedTime);
		}

		glClear ( GL_COLOR_BUFFER_BIT );

		for (auto it: windows) {
			it->draw();
			//Swap our back buffer to the front
			SDL_GL_SwapWindow(it->windowData->window);
		}

		if (handleEvents()) {
			break;
		}

		MatSig::flushSignals();
	}
}

Window *Application::getWindow(unsigned int w) {
	for (auto it: windows) {
		if (w == it->windowData->windowId) {
			return it;
		}
	}
	return nullptr;
}

bool Application::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		auto window = getWindow(event.window.windowID);
		if (event.type == SDL_QUIT) {
			return true;
		}

		if (window) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
			{
				auto &e = event.motion;
				window->onPointerMove(0, (double)e.x, (double)e.y, e.state);
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				auto &e = event.motion;
				window->onPointerDown(0, (double)e.x, (double)e.y);
			}
			break;

			case SDL_MOUSEBUTTONUP:
			{
				auto &e = event.motion;
				window->onPointerUp(0, (double)e.x, (double)e.y);
			}
			break;
			case SDL_KEYDOWN:
			{
				window->onKeyDown(event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod, event.key.repeat);
			}
			break;
			case SDL_KEYUP:
			{
				window->onKeyUp(event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod, event.key.repeat);
			}
			break;
			default:
				break;
			}
		}
	}
	return false;
}

Application::~Application() {
    QuitDrawModule();

    auto tmpWindowList = windows; //windows will start removing themselves from list
    for (auto it: tmpWindowList) {
    	delete it;
    }
    windows.clear();

    SDL_Quit();
}

void Application::quit() {
	running = false;
}

void Application::addWindow(class Window* window) {
	windows.push_back(window);
}

void Application::removeWindow(class Window* window) {
	windows.remove(window);
}

}  // namespace MatGui

