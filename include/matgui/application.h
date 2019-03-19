/*
 * matgui.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once

#include "matsig.h"

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

	// Scale is a global value that sets the scale for graphics.
	// This is to enable same window size on hd screens
	// Change by sending for example "--scale 2" as arguments on construction
	static float Scale();

	friend class Window;
};

}  // namespace MatGui

