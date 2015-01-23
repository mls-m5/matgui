/*
 * Button.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#include "button.h"
#include "draw.h"

namespace MatGui {

Button::Button() {
}

Button::~Button() {
}


void Button::draw() {
	drawSquare(vec(xPos, yPos, 0), 0, width, height, highlight? DRAW_STYLE_FILLED : DRAW_STYLE_LINES);
}

void Button::onPointerEnter(pointerId id, double x, double y,
		pointerState state) {
	highlight = true;
}

void Button::onPointerLeave(pointerId id, double x, double y,
		pointerState state) {
	highlight = false;
}

}
