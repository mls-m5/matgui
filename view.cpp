/*
 * View.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: mattias
 */

#include "view.h"

namespace MatGui {

View::View():
weight(1)
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
	this->weight = weight;
	if (_width <= 0) {
		widthFlags = w;
	} else {
		widthFlags = 1;
	}
	if (_height <= 0) {
		heightFlags = h;
	} else {
		heightFlags = 1;
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

} //namespace MatGui
