/*
 * knobview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

namespace matgui {

class ToggleView : public ControllerView {
public:
    ToggleView();
    virtual ~ToggleView();

    virtual bool onPointerDown(PointerId id,
                               MouseButton button,
                               double x,
                               double y) override;
    virtual bool onPointerUp(PointerId id,
                             MouseButton button,
                             double x,
                             double y) override;

    void draw() override;

    Paint indicatorStyle;
};

} // namespace matgui
