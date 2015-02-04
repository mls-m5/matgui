/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "pushcontrollerview.h"
#include "draw.h"
#include "math.h"

namespace MatGui {

PushControlerView::PushControlerView() {
}

PushControlerView::~PushControlerView() {
}

bool PushControlerView::onPointerDown(pointerId id, double x, double y) {
	onPointerMove(id, x, y, 1);
	_value = 1;
	changed.emit(_value);
	return true;
}

bool PushControlerView::onPointerUp(pointerId id, double x, double y) {
	onPointerMove(id, x, y, 1);
	_value = 0;
	changed.emit(_value);
	return true;
}

void PushControlerView::draw() {
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
	if (_highlight) {
		drawRect(_x + middleX - radius, _y + middleY - radius, 0, radius * 2, radius * 2, DrawStyle::Filled);
	}
	drawRect(_x + middleX - radius, _y + middleY - radius, 0, radius * 2, radius * 2, DrawStyle::Lines);

	if (_value > .5) {
		drawRect(_x + middleX - radius / 2, _y + middleY - radius / 2, 0, radius, radius, DrawStyle::Filled);
	}
}

}
