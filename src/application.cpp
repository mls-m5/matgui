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
static Window* activeWindow = nullptr;
static Application *application = nullptr;
static float scale = 1.f;
static bool continuousUpdates = true;
static bool invalidateOnEvent = true;

Application::Application(int argc, char** argv) {
	application = this;
	MatSig::setMainThread();

	for (int i = 0; i + 1 < argc; ++i) {
		if (argv[i] == string("--scale")) {
			++i;
			scale = atof(argv[i]);
		}
	}

    if (SDL_Init(SDL_INIT_VIDEO) < 0) /* Initialize SDL's Video subsystem */
        sdldie("Unable to initialize SDL"); /* Or die on error */
}

void Application::mainLoop() {
	running = true;
	auto oldTime = SDL_GetTicks();
	while (running) {
		auto newTime = SDL_GetTicks();
		auto passedTime = (double) (newTime - oldTime) / 1000.;
		oldTime = newTime;

		auto shouldRedraw = continuousUpdates;

		if (!shouldRedraw) {
			for (auto it: windows){
				if (it->invalid()) {
					shouldRedraw = true;
				}
			}
		}

		if (shouldRedraw && frameUpdate) {
			//Handle gives the developer the possibility to do things each frame
			frameUpdate.directCall(passedTime);
		}

		if (!shouldRedraw) {
			SDL_Delay(10); //Limit cpu usage on idle
		}

		for (auto it: windows) {
			if (it->invalid()) {
				it->draw();
				it->invalid(false);
			}
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
		if (event.type == SDL_QUIT) {
			return true;
		}

		auto window = getWindow(event.window.windowID);
		if (window) {
			auto redrawEvent = invalidateOnEvent; //If the event should trigger a redraw
			if (event.type == SDL_WINDOWEVENT) {

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
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					activeWindow = window;
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					if (activeWindow == window) {
						activeWindow = nullptr;
					}
					break;
				default:
					redrawEvent = false;
				}
			}
			else {

				switch (event.type) {
				case SDL_MOUSEMOTION:
				{
					auto &e = event.motion;
					window->onPointerMove(0, (double)e.x / scale, (double)e.y / scale, e.state);
				}
				break;
				case SDL_MOUSEBUTTONDOWN:
				{
					auto &e = event.button;
					auto button = MouseButton(1 << (e.button - 1)); //Make one bit for each button
					window->onPointerDown(0, button, (double)e.x / scale, (double)e.y / scale);
				}
				break;

				case SDL_MOUSEBUTTONUP:
				{
					auto &e = event.button;
					auto button = MouseButton(1 << (e.button - 1)); //Make one bit for each button
					window->onPointerUp(0, button, (double)e.x / scale, (double)e.y / scale);
				}
				break;
				case SDL_KEYDOWN:
				{
					window->onKeyDown(event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod, event.key.repeat);
				}
				break;
				case SDL_MOUSEWHEEL:
				{
					auto &e = event.wheel;
					window->onScroll(0, (double) e.x / scale, (double) e.y / scale);
				}
				break;
				case SDL_KEYUP:
				{
					window->onKeyUp(event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod, event.key.repeat);
				}
				break;

				default:
					redrawEvent = false;
					break;
				}
			}
			window->invalidate();
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

void Application::addWindow(class Window* window) {
	windows.push_back(window);
}

void Application::removeWindow(class Window* window) {
	windows.remove(window);
	if (windows.empty()) {
		application->quit();
	}
}

float Application::Scale() {
	return scale;
}

void Application::ContinuousUpdates(bool state) {
	continuousUpdates = state;
}

void Application::InvalidateOnEvent(bool state) {
	invalidateOnEvent = state;
}

}  // namespace MatGui

#endif //__ANDROID__ block
