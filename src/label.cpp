/*
 * label.cpp
 *
 *  Created on: 19 dec 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/label.h"

namespace MatGui {

Label::Label(std::string label) {
    _fontView.font(Font(30));
    this->label(label);
}

Label::~Label() {
}

void Label::draw() {
    currentStyle.drawBasicView(this);
    if (_fontView) {
        _fontView.draw(x() + width() / 2., y() + height() / 2.);
    }
}

} // namespace MatGui
