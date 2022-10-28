/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/knobview.h"
#include "matgui/draw.h"
#include "matgui/matgui-constants.h"

#include <cmath>

namespace matgui {

KnobView::KnobView() {
    indicatorStyle.fill.color(1, 1, 1, .5);

    hoverStyle.fill.color(1, 1, 1, .1);
    style.line.color(1, 1, 1, .3);
    updateStyle();
}

KnobView::~KnobView() = default;

bool KnobView::onPointerDown(PointerId id,
                             MouseButton button,
                             double x,
                             double y) {
    onPointerMove(id, x, y, button);
    return true;
}

bool KnobView::onPointerUp(PointerId /*id*/,
                           MouseButton /*button*/,
                           double /*x*/,
                           double /*y*/) {
    return true;
}

bool KnobView::onPointerMove(PointerId /*id*/,
                             double x,
                             double y,
                             PointerState state) {
    if (state) {
        // Todo: make this nicer, like that the control does not go all the way
        // round or something like that
        double v = atan2(x - width() / 2, -y + height() / 2) / pi2 + .5;
        v *= (1. +
              step() / (max() - min())); // Adding a bit extra to make it
                                         // possible to get the highest value
        amount(v);
        changed.emit(value());
        return true;
    }
    return false;
}

void KnobView::draw() {
    auto middleX = width() / 2.;
    auto middleY = height() / 2.;

    double radius;
    if (width() > height()) {
        radius = height();
    }
    else {
        radius = width();
    }
    radius *= (.8 / 2);
    currentStyle.drawEllipse(
        x() + middleX - radius, y() + middleY - radius, radius * 2, radius * 2);

    const double v = (value() - min()) / (max() - min());

    auto smallR = radius * .1;
    auto a = v * pi2;
    indicatorStyle.drawEllipse(x() + middleX - radius * sin(a) - smallR,
                               y() + middleY + radius * cos(a) - smallR,
                               smallR * 2,
                               smallR * 2);
}

} // namespace matgui
