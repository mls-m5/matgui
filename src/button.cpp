/*
 * Button.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#include "matgui/button.h"
#include "matgui/draw.h"

namespace MatGui {

Button::Button(std::string label) {
	_fontView.font(Font(30));
	this->label(label);

	hoverStyle.fill.color(1, 1, 1, .1);
	style.line.color(1, 1, 1, .3);
	updateStyle();
}

Button::~Button() {
}


void Button::draw() {
	currentStyle.drawBasicView(this);
	if (_fontView) {
		_fontView.draw(x() + width() / 2., y() + height() / 2.);
	}
}

} //namespace Matgui

