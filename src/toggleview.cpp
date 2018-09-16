/*
 * knobview.cpp
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "toggleview.h"
#include "draw.h"
#include "math.h"

namespace MatGui {

ToggleView::ToggleView() {
	indicatorStyle.fill.color(1, 1, 1, .5);

	hoverStyle.fill.color(1, 1, 1, .1);
	style.line.color(1, 1, 1, .3);
	updateStyle();
}

ToggleView::~ToggleView() {
}

bool ToggleView::onPointerDown(pointerId id, MouseButton button, double x, double y) {
	onPointerMove(id, x, y, button);
	_value = not _value;
	changed.emit(_value);
	return true;
}

bool ToggleView::onPointerUp(pointerId id, MouseButton button, double x, double y) {
	onPointerMove(id, x, y, button);
	return true;
}

void ToggleView::draw() {
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

	currentStyle.drawRect(
			_x + middleX - radius,
			_y + middleY - radius,
			radius * 2,
			radius * 2
	);

	if (_value > .5) {
		indicatorStyle.drawRect(
				_x + middleX - radius / 2,
				_y + middleY - radius / 2,
				radius,
				radius
		);
	}
}

}
