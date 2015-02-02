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
	_font = std::shared_ptr<Font>(new Font(30));
}

Button::~Button() {
}


void Button::draw() {
	drawRect(_x, _y, 0, _width, _height, _highlight? DrawStyle::Filled : DrawStyle::Lines);
	if (_font and !_label.empty()) {
		_font->draw(_x + _width / 2., _y + _height / 2., _label, true);
	}
}

}
