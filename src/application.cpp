/*
 * matgui.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#ifndef __ANDROID__

#include <SDL2/SDL.h>
#include <iostream>
#include "application.h"
#include "commonsdl.h"
#include "signal.h"
#include "window.h"
#include "draw.h"
#include "windowdata.h"

using namespace std;

namespace MatGui {

static std::list<Window *> windows;
static Application *application = nullptr;

Application::Application(int argc, char** argv) {
	application = this;
	MatSig::setMainThread();
    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); //This prevents the rendering function from rendering anything of some reason

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


		for (auto it: windows) {
			it->draw();
			//Swap our back buffer to the front
		}

		if (handleEvents()) {
			break;
		}

		MatSig::flushSignals();
	}
}

Window *Application::getWindow(unsigned int w) {
	for (auto it: windows) {
		if (w == it->_windowData->windowId) {
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

			case SDL_WINDOWEVENT:

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
				} //end switch

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
	if (windows.empty()) {
		application->quit();
	}
}

}  // namespace MatGui

#endif //__ANDROID__ block
