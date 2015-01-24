/*
 * View.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "common.h"
#include "signal.h"

namespace MatGui {

using MatSig::Signal;
class View {
public:
	enum widthFlags {
		VIEW_MATCH_PARENT = -1,
		VIEW_WRAP_CONTENT = -2,
		VIEW_WEIGHTED = 0,
		VIEW_FIXED = 1
	};

	typedef int pointerId;
	typedef int pointerState;

	View();
	virtual ~View();

	virtual void draw();
	virtual void refresh() {};
	virtual void setLocation(double x, double y, double w, double h,
			double weight = 1);
	virtual bool isPointerInside(double x, double y);
	virtual bool isPointerInsideLocal(double localx, double localy);
	virtual void owned(bool owned) { _owned = owned; }


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
		pointerEnter.emit({id, x, y, state});
	}

	virtual void onPointerLeave(pointerId id, double x, double y, pointerState state) {
		pointerLeave.emit({id, x, y, state});
	}

	struct pointerArgument {
		pointerId id;
		double x, y;
		pointerState state;
	};

	//Signals
	Signal <pointerArgument> clicked;
	Signal <pointerArgument> pointerMoved;
	Signal <pointerArgument> pointerDown;
	Signal <pointerArgument> pointerUp;

	Signal <pointerArgument> pointerEnter;
	Signal <pointerArgument> pointerLeave;

	double _x, _y;
	double _width, _height;
	int _widthFlags, _heightFlags;
	double _weight = 0;
	bool _owned = true; //Id the widget should be deleted by its parent
};

} //namespace MatGui
