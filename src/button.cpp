/*
 * Button.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/button.h"
#include "matgui/draw.h"

namespace matgui {

Button::Button(std::string label) {
    _fontView.font(Font(30));
    this->label(label);

    hoverStyle.fill.color(1, 1, 1, .1);
    style.line.color(1, 1, 1, .3);
    focusStyle.line.color(1, 1, 1, .8);
    updateStyle();
}

Button::~Button() = default;

void Button::draw() {
    currentStyle.drawBasicView(this);
    if (_fontView) {
        _fontView.draw(x() + width() / 2., y() + height() / 2.);
    }
}

void Button::label(const std::string text) {
    _fontView.text(text);
}

const std::string &Button::label() {
    return _fontView.text();
}

void Button::font(Font font) {
    _fontView.font(font);
}

} // namespace matgui
