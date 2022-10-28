/*
 * knobview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

namespace matgui {

class SliderView : public ControllerView {
public:
    SliderView();
    virtual ~SliderView();

    virtual bool onPointerDown(PointerId id,
                               MouseButton,
                               double x,
                               double y) override;
    virtual bool onPointerUp(PointerId id,
                             MouseButton,
                             double x,
                             double y) override;
    virtual bool onPointerMove(PointerId id,
                               double x,
                               double y,
                               pointerState state) override;

    void draw() override;

    Paint indicatorStyle;
};

} // namespace matgui
