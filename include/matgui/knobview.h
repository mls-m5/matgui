/*
 * knobview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

namespace matgui {

class KnobView : public ControllerView {
public:
    KnobView();
    virtual ~KnobView();

    virtual bool onPointerDown(PointerId id,
                               MouseButton button,
                               double x,
                               double y) override;
    virtual bool onPointerUp(PointerId id,
                             MouseButton button,
                             double x,
                             double y) override;
    virtual bool onPointerMove(PointerId id,
                               double x,
                               double y,
                               PointerState state) override;

    void draw() override;

    Paint indicatorStyle;
};

} // namespace matgui
