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
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	this->weight = weight;
	if (width <= 0) {
		widthFlags = w;
	} else {
		widthFlags = 1;
	}
	if (height <= 0) {
		heightFlags = h;
	} else {
		heightFlags = 1;
	}
}

bool View::isPointerInside(double x, double y) {
	if (x >= xPos and y >= yPos) {
		if (x <= xPos + width and y <= yPos + height) {
			return true;
		}
	}
	return false;
}

bool View::isPointerInsideLocal(double x, double y) {
	if (x >= 0 and y >= 0) {
		if (x <= width and y <= height) {
			return true;
		}
	}
	return false;
}

} //namespace MatGui
