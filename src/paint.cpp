/*
 * paint.cpp
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#include "paint.h"
#include "view.h"

namespace MatGui {

Paint::Paint() {
	fill._style = DrawStyle::None;
}

Paint::~Paint() {
}

void Paint::drawRect(double x, double y, double width, double height) {
	MatGui::drawRect(x, y, width, height, this);
}

void Paint::push(Paint& s) {
	if (not s.enabled) {
		return;
	}
	fill += s.fill;
	line += s.line;
}

void Paint::drawLine(double x, double y, double x2, double y2) {
	if (line.style()) {
		MatGui::drawLine(x, y, x2, y2, this);
	}
}

void Paint::drawElipse(double x, double y, double width, double height) {
	MatGui::drawElipse(x, y, width, height, this);
}

void Paint::drawBasicView(View* view) {
	drawRect(view->x(), view->y(), view->width(), view->height());
}

} /* namespace MatGui */
