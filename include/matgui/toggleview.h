/*
 * knobview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

namespace MatGui {

class ToggleView: public ControllerView {
public:
	ToggleView();
	virtual ~ToggleView();

	virtual bool onPointerDown(pointerId id, double x, double y) override;
	virtual bool onPointerUp(pointerId id, double x, double y) override;

	void draw() override;
};


}
