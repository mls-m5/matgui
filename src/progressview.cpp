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
	if (_orientation == Orientation::Horizontal) {
		indicatorStyle.drawRect(
				x(),
				y(),
				width() * (double)(value() - min()) / (max() - min()),
				height()
		);
	}
	else {
		auto value = (double)(this->value() - min()) / (max() - min());
		indicatorStyle.drawRect(
				x(),
				y() + height() * (1 - value),
				width(),
				height() * value
		);
	}
}

} /* namespace MatGui */
