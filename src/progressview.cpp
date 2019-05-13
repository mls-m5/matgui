/*
 * progressview.cpp
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/progressview.h"
#include "matgui/draw.h"

namespace MatGui {

ProgressView::ProgressView() {
	indicatorStyle.fill.color(1, 1, 1, .5);

	style.line.color(1, 1, 1, .3);
	updateStyle();

}

ProgressView::~ProgressView() {
}

void ProgressView::draw() {
	currentStyle.drawBasicView(this);
	indicatorStyle.drawRect(
			_x,
			_y,
			_width * (double)(_value - _min) / (_max - _min),
			_height
	);
}

} /* namespace MatGui */
