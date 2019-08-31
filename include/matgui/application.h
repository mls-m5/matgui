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
	// Constructor sets up environment
	Application(int argc, char **argv);

	// Destructor tears down environment
	// Exception is for emscripten when the destructor does nothing
	virtual ~Application();
	static void mainLoop();
	static bool handleEvents();

	static class Window *getWindow(unsigned int w);

	// Stops the main loop
	static void quit();

	// note: frameUpdate signal is moved to window

	// Scale is a global value that sets the scale for graphics.
	// This is to enable same window size on hd screens
	// Change by sending for example "--scale 2" as arguments on construction
	static float Scale();

	// Used to set scale
	// If used after crating any window the result is undefined
	static void Scale(float scale);

	// Continuous updates is when the screen updates as fast as possible
	// (in sync with the screen updates
	// When disable window::invalidate triggers a redraw
	// Also any input event triggers redraw of the window by default
	static void ContinuousUpdates(bool state);

	// Sets if windows should be invalidated on input events
	// if false window::invalidate needs to be called to redraw
	static void InvalidateOnEvent(bool state);

	// Functions to convert between key and description string
	static std::string GetKeyNameFromScancode(int scancode);
	static std::string GetKeyNameFromKey(int scancode);
	static int GetKeyFromName(const std::string &name);
	static int GetScancodeFromName(const std::string &name);
	static int GetScancodeFromKey(int key);
	static int GetKeyFromScancode(int scancode);

protected:

	//Functions used internally to handle windows
	static void addWindow(class Window*);
	static void removeWindow(class Window*);
	static inline void innerLoop();

	friend class Window;
};

}  // namespace MatGui

