/*
 * progressview.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "controllerview.h"

namespace MatGui {

class ProgressView : public ControllerView {
public:
	ProgressView();
	virtual ~ProgressView();

	void draw() override;

	Paint indicatorStyle;
};

} /* namespace MatGui */
