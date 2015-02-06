/*
 * window.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <string>
#include "linearlayout.h"

namespace MatGui {

class Window: public LinearLayout {
public:
	Window(std::string title);
	virtual ~Window();

	void draw() override;
	bool onRequestClose();

	void show();
	void hide();

	Signal<void *, bool> closeSignal;
protected:
	class WindowData *_windowData = 0;

	friend class Application;
};

} /* namespace MatGui */
