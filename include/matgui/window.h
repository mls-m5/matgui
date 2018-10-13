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
	bool onRequestClose();

	//Does not work... you need to recreate the open gl context each time
	bool onResize(int width, int height);

	void show();
	void hide();

	void cursorVisibility(bool value);
	void setCursorPosition(int x, int y);
	std::pair<int, int> getCursorPosition();

	Signal<void *, bool> closeSignal;

protected:
	std::unique_ptr<class WindowData> _windowData;

	friend class Application;
};

} /* namespace MatGui */
