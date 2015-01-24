/*
 * linearlayout.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "linearlayout.h"

namespace MatGui {

LinearLayout::LinearLayout()
{
}

LinearLayout::~LinearLayout() {
}

void LinearLayout::refresh() {
	Layout::refresh();
	if (orientation == LAYOUT_HORIZONTAL){
		double tX = _x + margin;
		for (auto it: children){
			it->x(tX);
			tX += it->width() + margin;
			it->y( _y + margin);
		}
	}
	else{
		double tY = _y + margin;
		for (auto it: children){
			it->y(tY);
			tY += it->height() + margin;
			it->x(_x + margin);
		}
	}

//	Gui::postRedraw();
}

} //namespace MatGui
