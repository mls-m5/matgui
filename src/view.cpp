/*
 * View.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: mattias
 */

#include "view.h"

namespace MatGui {

View::View():
_weight(1)
{
	setLocation(0, 0, VIEW_WEIGHTED, VIEW_WEIGHTED);
}

View::~View() {
}

void View::draw() {
}

void View::setLocation(double x, double y, double w, double h, double weight) {
	_x = x;
	_y = y;
	_width = w;
	_height = h;
	this->_weight = weight;
	if (_width <= 0) {
		_widthFlags = w;
	} else {
		_widthFlags = 1;
	}
	if (_height <= 0) {
		_heightFlags = h;
	} else {
		_heightFlags = 1;
	}
}

bool View::isPointerInside(double x, double y) {
	if (x >= _x and y >= _y) {
		if (x <= _x + _width and y <= _y + _height) {
			return true;
		}
	}
	return false;
}

bool View::isPointerInsideLocal(double x, double y) {
	if (x >= 0 and y >= 0) {
		if (x <= _width and y <= _height) {
			return true;
		}
	}
	return false;
}

bool View::onPointerDown(pointerId id, double x, double y) {
	pointerDown.emit( { id, x, y, 1 });
	return true;
}

bool View::onPointerUp(pointerId id, double x, double y) {
	pointerUp.emit( { id, x, y, 0 });
	if (isPointerInsideLocal(x, y)) {
		clicked.emit( { id, x, y, 1 });
	}
	return true;
}

bool View::onPointerMove(pointerId id, double x, double y,
		pointerState state) {
	pointerMoved.emit( { id, x, y, state });
	return true;
}

void View::onPointerEnter(pointerId id, double x, double y,
		pointerState state) {
	pointerEnter.emit( { id, x, y, state });
}

void View::onPointerLeave(pointerId id, double x, double y,
		pointerState state) {
	pointerLeave.emit( { id, x, y, state });
}

bool View::onKeyDown(KeySym sym, KeyScanCode scancode, int repeat) {
	//If there is no listener the key go to another view
	if (keyDown) {
		keyDown.emit( { sym, scancode, repeat });
		return true;
	} else {
		return false;
	}
}

bool View::onKeyUp(KeySym sym, KeyScanCode scancode, int repeat) {
	if (keyUp) {
		keyUp.emit( { sym, scancode, repeat });
		return true;
	} else {
		return false;
	}
}

} //namespace MatGui
