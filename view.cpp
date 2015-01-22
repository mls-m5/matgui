/*
 * View.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: mattias
 */

#include "view.h"

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
	if (x >= xPos && y >= yPos) {
		if (x <= xPos + width && y <= yPos + height) {
			return true;
		}
	}
	return false;
}
