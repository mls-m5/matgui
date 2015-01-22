/*
 * knobview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

class KnobView: public ControllerView {
public:
	KnobView();
	virtual ~KnobView();

	void draw();
//	bool handleEvent(const ViewEvent& event);
};

