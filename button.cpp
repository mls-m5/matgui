/*
 * Button.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#include "button.h"
#include "draw.h"

#include <iostream>
using namespace std;
Button::Button() {
}

Button::~Button() {
}

void Button::onPointerMove(pointerId id, double x, double y,
		pointerState state) {
	View::onPointerMove(id, x, y, state);
	highlight = isPointerInside(x, y);
}

void Button::draw() {
	drawSquare(vec(xPos, yPos, 0), 0, width, height, highlight? DRAW_STYLE_FILLED : DRAW_STYLE_LINES);
}
