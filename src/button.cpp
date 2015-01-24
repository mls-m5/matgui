/*
 * Button.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#include "button.h"
#include "draw.h"

namespace MatGui {

Button::Button(std::string label):
	_label(label){
	_font = std::shared_ptr<Font>(new Font("font/Ubuntu-R.ttf", 30));
}

Button::~Button() {
}


void Button::draw() {
	drawSquare(vec(_x, _y, 0), 0, _width, _height, highlight? DRAW_STYLE_FILLED : DRAW_STYLE_LINES);
	if (_font and !_label.empty()) {
		_font->draw(_x, _y, _label);
	}
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
