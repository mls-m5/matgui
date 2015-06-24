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

	hoverStyle.fill.setColor(1, 1, 1, .1);
	style.line.setColor(1, 1, 1, .3);
	updateStyle();
}

Button::~Button() {
}


void Button::draw() {
	currentStyle.drawBasicView(this);
	if (_fontView) {
		_fontView.draw(_x + _width / 2., _y + _height / 2.);
	}
}

} //namespace Matgui

