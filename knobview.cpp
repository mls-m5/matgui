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

void KnobView::draw() {
	auto middleX = width / 2.;
	auto middleY = height / 2.;

	double radius;
	if (width > height){
		radius = height;
	}
	else{
		radius = width;
	}
	radius *= (.8 / 2);
	drawElipse(vec(xPos + middleX - radius, yPos + middleY - radius), 0, radius * 2, radius * 2, DRAW_STYLE_LINES);

	const double v = (value - min) / (max - min);

	auto smallR = radius *.1;
	auto a = v * pi2;
	drawElipse(vec(xPos + middleX - radius * sin(a) - smallR, yPos + middleY - radius * cos(a) - smallR),
			0, smallR * 2, smallR * 2, DRAW_STYLE_FILLED);
}


}
