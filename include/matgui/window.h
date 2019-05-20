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

	//Does not work... you need to recreate the open gl context each time
	bool onResize(int width, int height);

	void show();
	void hide();

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

protected:
	std::unique_ptr<struct WindowData> _windowData;

	friend class Application;
};

} /* namespace MatGui */
