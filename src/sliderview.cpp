/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "sliderview.h"
#include "draw.h"
#include "math.h"

namespace MatGui {


static constexpr double middleWidth = .1;
static constexpr double handleHeight = .1;

SliderView::SliderView() {
	indicatorStyle.fill.setColor(1,1,1, .5);
}

SliderView::~SliderView() {
}

bool SliderView::onPointerDown(pointerId id, double x, double y) {
	onPointerMove(id, x, y, 1);
	return true;
}

bool SliderView::onPointerUp(pointerId id, double x, double y) {
	return true;
}

bool SliderView::onPointerMove(pointerId id, double x, double y,
		pointerState state) {
	if (state) {
		double v = 1 - (y - _height * handleHeight / 2.) / (_height * (1 - handleHeight));;
		amount(v);
		changed.emit(_value);
		return true;
	}
	return false;
}

void SliderView::draw() {
	currentStyle.drawBasicView(this);

	indicatorStyle.drawRect(
			_x + _width * (1 - middleWidth) / 2.,
			_y,
			_width * middleWidth,
			_height);

	const double v = 1 - (_value - _min) / (_max - _min);

	indicatorStyle.drawRect(
			_x,
			_y + _height * (1 - handleHeight) * v,
			_width,
			_height * handleHeight
	);
}

}
