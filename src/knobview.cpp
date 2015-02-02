/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "knobview.h"
#include "draw.h"
#include "math.h"

namespace MatGui {

KnobView::KnobView() {
}

KnobView::~KnobView() {
}

bool KnobView::onPointerDown(pointerId id, double x, double y) {
	onPointerMove(id, x, y, 1);
	return true;
}

bool KnobView::onPointerUp(pointerId id, double x, double y) {
	return true;
}

bool KnobView::onPointerMove(pointerId id, double x, double y,
		pointerState state) {
	if (state) {
		double v = atan2(x - _width / 2, -y + _height / 2) / pi2 + .5;
		amount(v);
		changed.emit(_value);
		return true;
	}
	return false;
}

void KnobView::draw() {
	auto middleX = _width / 2.;
	auto middleY = _height / 2.;

	double radius;
	if (_width > _height){
		radius = _height;
	}
	else{
		radius = _width;
	}
	radius *= (.8 / 2);
	drawElipse(vec(_x + middleX - radius, _y + middleY - radius), 0, radius * 2, radius * 2, _highlight? DrawStyle::Filled : DrawStyle::Lines);

	const double v = (_value - _min) / (_max - _min);

	auto smallR = radius *.1;
	auto a = v * pi2;
	drawElipse(vec(_x + middleX - radius * sin(a) - smallR, _y + middleY + radius * cos(a) - smallR),
			0, smallR * 2, smallR * 2, DrawStyle::Filled);
}

}
