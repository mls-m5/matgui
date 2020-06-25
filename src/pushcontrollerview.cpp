/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/pushcontrollerview.h"
#include "matgui/draw.h"
#include "math.h"

namespace matgui {

PushControlerView::PushControlerView() {
}

PushControlerView::~PushControlerView() {
}

bool PushControlerView::onPointerDown(pointerId id,
                                      MouseButton /*button*/,
                                      double x,
                                      double y) {
    onPointerMove(id, x, y, 1);
    value(1);
    changed.emit(value());
    return true;
}

bool PushControlerView::onPointerUp(pointerId id,
                                    MouseButton /*button*/,
                                    double x,
                                    double y) {
    onPointerMove(id, x, y, 1);
    value(0);
    changed.emit(value());
    return true;
}

void PushControlerView::draw() {
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

    currentStyle.drawBasicView(this);

    drawRect(x() + middleX - radius,
             y() + middleY - radius,
             radius * 2,
             radius * 2,
             &currentStyle);

    if (value() > .5) {
        indicatorStyle.fill.color(1, 1, 1);
        indicatorStyle.drawRect(x() + middleX - radius / 2,
                                y() + middleY - radius / 2,
                                radius,
                                radius);
    }
}

} // namespace matgui
