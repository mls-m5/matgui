/*
 * View.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "common.h"
#include "signal.h"

typedef int pointerId;
typedef int pointerState;

enum widthFlags {
	VIEW_MATCH_PARENT = -1,
	VIEW_WRAP_CONTENT = -2,
	VIEW_WEIGHTED = 0,
	VIEW_FIXED = 1
};

class View {
public:
	View();
	virtual ~View();

	virtual void draw();
	virtual void refresh() {};
	virtual void setLocation(double x, double y, double w, double h,
			double weight = 1);
	virtual bool isPointerInside(double x, double y);
	virtual bool isPointerInsideLocal(double localx, double localy);


	//Callback functions return true to capture the mouse focus
	virtual bool onPointerDown(pointerId id, double x, double y) {
		pointerDown.emit({id, x, y, 1});
		return true;
	}
	virtual bool onPointerUp(pointerId id, double x, double y) {
		pointerUp.emit({id, x, y, 0});
		if (isPointerInsideLocal(x, y)) {
			clicked.emit({id, x, y, 1});
		}
		return true;
	}
	virtual bool onPointerMove(pointerId id, double x, double y, pointerState state) {
		pointerMoved.emit({id, x, y, state});
		return true;
	}

	virtual void onPointerEnter(pointerId id, double x, double y, pointerState state) {

	}

	virtual void onPointerLeave(pointerId id, double x, double y, pointerState state) {

	}

	struct pointerArgument {
		pointerId id;
		double x, y;
		pointerState state;
	};

	//Signals
	Signal <void, pointerArgument> clicked;
	Signal <void, pointerArgument> pointerMoved;
	Signal <void, pointerArgument> pointerDown;
	Signal <void, pointerArgument> pointerUp;

	double xPos, yPos;
	double width, height;
	int widthFlags, heightFlags;
	double weight = 0;
};
