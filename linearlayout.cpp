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
		double tX = xPos + margin;
		for (auto it: children){
			it->xPos = tX;
			tX += it->width + margin;
			it->yPos = yPos + margin;
		}
	}
	else{
		double tY = yPos + margin;
		for (auto it: children){
			it->yPos = tY;
			tY += it->height + margin;
			it->xPos = xPos + margin;
		}
	}

//	Gui::postRedraw();
}

} //namespace MatGui
