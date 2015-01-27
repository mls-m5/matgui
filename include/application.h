/*
 * matgui.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once

#include "signal.h"

namespace MatGui {

class Application {
public:

	Application(int argc, char **argv);
	virtual ~Application();
	void mainLoop();
	bool handleEvents();
	class Window *getWindow(unsigned int w);

	void quit();

	bool running = false;

	//Signal called when the main loop take a step
	//the argument is the time since the last time the signal was called
	MatSig::Signal<double> frameUpdate;

protected:

	static void addWindow(class Window*);
	static void removeWindow(class Window*);

	friend class Window;
};

}  // namespace MatGui

