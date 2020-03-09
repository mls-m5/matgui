/*
 * knobview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

namespace MatGui {

class SliderView : public ControllerView {
public:
    SliderView();
    virtual ~SliderView();

    virtual bool onPointerDown(pointerId id,
                               MouseButton,
                               double x,
                               double y) override;
    virtual bool onPointerUp(pointerId id,
                             MouseButton,
                             double x,
                             double y) override;
    virtual bool onPointerMove(pointerId id,
                               double x,
                               double y,
                               pointerState state) override;

    void draw() override;

    Paint indicatorStyle;
};

} // namespace MatGui
