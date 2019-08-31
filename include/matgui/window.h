/*
 * window.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>
#include <memory>
#include "linearlayout.h"

namespace MatGui {

class Window: public LinearLayout {
public:
	Window(std::string title, int width = 512, int height = 512, bool resizable = false);
	virtual ~Window();

	void draw() override;

	// Functions to be called before and after draw() for window
	void clear();
	void swap();

	bool onRequestClose();

	bool onResize(int width, int height);

	void show();
	void hide();

	// Set fullscreen if state is true or disable with state = false
	// if changeVideoMode is set, the display changes resolution to the windows size
	// return false on success and true on error
	bool setFullscreen(bool state = true, bool changeVideoMode = false);

	// Set title of the window
	void title(std::string title);

	// invalidate triggers a redraw of the window
	inline void invalidate() { invalid(true); };

	//check if the window needs redrawing
	bool invalid();
	void invalid(bool state);

	void cursorVisibility(bool value);
	void setCursorPosition(int x, int y);
	std::pair<int, int> getCursorPosition();

	// A direct call signal
	// if a function returns true closing of the window is aborted
	// if true is returned, the window is closed as usual
	Signal<void, bool> closeSignal;

	// Signal called when the main loop take a step
	// the argument is the time since the last time the signal was called
	// Note: the functions is called directly so it is okay to connect
	// render code to this signal
	MatSig::Signal<double> frameUpdate;

protected:
	std::unique_ptr<struct WindowData> _windowData;

	friend class Application;
};

} /* namespace MatGui */
