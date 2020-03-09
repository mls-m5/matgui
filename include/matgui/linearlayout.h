/*
 * linearlayout.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "layout.h"

namespace MatGui {

class LinearLayout : public Layout {
public:
    LinearLayout();
    virtual ~LinearLayout();

    virtual void refresh();
};

} // namespace MatGui
