/*
 * progressview.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"
#include "paint.h"

namespace matgui {

class ProgressView : public ControllerView {
public:
    enum class Orientation {
        Horizontal,
        Vertical,
    };
    ProgressView();
    virtual ~ProgressView();

    void draw() override;

    Paint indicatorStyle;

    member_property_declaration_default(Orientation,
                                        orientation,
                                        Orientation::Horizontal);
};

} /* namespace matgui */
