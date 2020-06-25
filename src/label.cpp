/*
 * label.cpp
 *
 *  Created on: 19 dec 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/label.h"

namespace matgui {

Label::Label(std::string label) {
    _fontView.font(Font(30));
    this->label(label);
}

Label::~Label() {
}

void Label::draw() {
    currentStyle.drawBasicView(this);
    if (_fontView) {
        auto halfWidth = width() / 2.;
        _fontView.draw(x() + halfWidth * (1. + -_fontView.alignment()),
                       y() + height() / 2.);
    }
}

} // namespace matgui
