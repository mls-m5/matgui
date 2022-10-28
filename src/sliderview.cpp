/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/sliderview.h"
#include "matgui/draw.h"
#include "math.h"

namespace matgui {

static constexpr double middleWidth = .1;
static constexpr double handleHeight = .1;

SliderView::SliderView() {
    indicatorStyle.fill.color(1, 1, 1, .5);
}

SliderView::~SliderView() {
}

bool SliderView::onPointerDown(PointerId id,
                               MouseButton button,
                               double x,
                               double y) {
    onPointerMove(id, x, y, button);
    return true;
}

bool SliderView::onPointerUp(PointerId /*id*/,
                             MouseButton /*button*/,
                             double /*x*/,
                             double /*y*/) {
    return true;
}

bool SliderView::onPointerMove(PointerId /*id*/,
                               double /*x*/,
                               double y,
                               pointerState state) {
    if (state) {
        double v = 1. - (y - height() * handleHeight / 2.) /
                            (height() * (1 - handleHeight));
        amount(v);
        changed.emit(value());
        return true;
    }
    return false;
}

void SliderView::draw() {
    currentStyle.drawBasicView(this);

    indicatorStyle.drawRect(x() + width() * (1 - middleWidth) / 2.,
                            y(),
                            width() * middleWidth,
                            height());

    const double v = 1 - (value() - min()) / (max() - min());

    indicatorStyle.drawRect(x(),
                            y() + height() * (1 - handleHeight) * v,
                            width(),
                            height() * handleHeight);
}

} // namespace matgui
