/*
 * Button.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#include "button.h"
#include "draw.h"

namespace MatGui {

Button::Button(std::string label) {
	_fontView.font(Font(30));
	this->label(label);
}

Button::~Button() {
}


void Button::draw() {
	drawRect(_x, _y, 0, _width, _height, _highlight? DrawStyle::Filled : DrawStyle::Lines);
	if (_fontView) {
		_fontView.draw(_x + _width / 2., _y + _height / 2.);
	}
}

} //namespace Matgui

