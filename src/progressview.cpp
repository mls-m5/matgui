/*
 * progressview.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "progressview.h"
#include "draw.h"

namespace MatGui {

ProgressView::ProgressView() {
}

ProgressView::~ProgressView() {
}

void ProgressView::draw() {
	drawRect({_x, _y}, 0, _width, _height, DrawStyle::Lines);
	drawRect({_x, _y}, 0, _width * (double)(_value - _min) / (_max - _min), _height, DrawStyle::Filled);
}

} /* namespace MatGui */
