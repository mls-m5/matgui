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

	if (_highlight) {
		drawRect(_x, _y, 0, _width, _height, DrawStyle::Filled);
	}

	drawRect(vec(_x + _width * (1 - middleWidth) / 2., _y), 0, _width * middleWidth, _height, DrawStyle::Filled);

	const double v = 1 - (_value - _min) / (_max - _min);

	drawRect(_x, _y + _height * (1 - handleHeight) * v, 0, _width, _height * handleHeight, DrawStyle::Filled);
}

}
