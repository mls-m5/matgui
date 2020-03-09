/*
 * linearlayout.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/linearlayout.h"

namespace MatGui {

LinearLayout::LinearLayout() {
}

LinearLayout::~LinearLayout() {
}

void LinearLayout::refresh() {
    Layout::refresh();
    if (_orientation == LAYOUT_HORIZONTAL) {
        double tX = x() + _padding;
        for (auto &it : children) {
            it->x(tX);
            tX += it->width() + _padding;
            it->y(y() + _padding);
            it->refresh();
        }
    }
    else {
        double tY = y() + _padding;
        for (auto &it : children) {
            it->y(tY);
            tY += it->height() + _padding;
            it->x(x() + _padding);
            it->refresh();
        }
    }

    //	Gui::postRedraw();
}

} // namespace MatGui
