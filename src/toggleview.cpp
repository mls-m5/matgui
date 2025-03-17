/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/toggleview.h"
#include "matgui/draw.h"
#include "math.h"

namespace matgui {

ToggleView::ToggleView() {
    indicatorStyle.fill.color(1, 1, 1, .5);

    hoverStyle.fill.color(1, 1, 1, .1);
    focusStyle.line.color(1, 1, 1, .8);
    style.line.color(1, 1, 1, .3);
    updateStyle();
}

ToggleView::~ToggleView() {
}

bool ToggleView::onPointerDown(PointerId id,
                               MouseButton button,
                               double x,
                               double y) {
    onPointerMove(id, x, y, button);
    value(not value());
    changed.emit(value());
    return true;
}

bool ToggleView::onPointerUp(PointerId id,
                             MouseButton button,
                             double x,
                             double y) {
    onPointerMove(id, x, y, button);
    return true;
}

void ToggleView::draw() {
    auto middleX = width() / 2.;
    auto middleY = height() / 2.;

    auto radius = 0.;
    if (width() > height()) {
        radius = height();
    }
    else {
        radius = width();
    }
    radius *= (.8 / 2);

    currentStyle.drawRect(
        x() + middleX - radius, y() + middleY - radius, radius * 2, radius * 2);

    if (value() > .5) {
        indicatorStyle.drawRect(x() + middleX - radius / 2,
                                y() + middleY - radius / 2,
                                radius,
                                radius);
    }
}

} // namespace matgui
