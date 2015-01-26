/*
 * View.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "common.h"
#include "signal.h"

//standard getters and setters
#define member_set(type, m) void m (type value) { _ ## m = value; }
#define member_get(type, m) type m () { return _ ## m; }
#define member_property(type, m) member_get(type, m) member_set(type, m)

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


	//Callback functions return true to capture the mouse focus
	virtual bool onPointerDown(pointerId id, double x, double y);
	virtual bool onPointerUp(pointerId id, double x, double y);
	virtual bool onPointerMove(pointerId id, double x, double y,
			pointerState state);
	virtual void onPointerEnter(pointerId id, double x, double y,
			pointerState state);
	virtual void onPointerLeave(pointerId id, double x, double y,
			pointerState state);

	typedef unsigned KeySym;
	typedef unsigned KeyScanCode;

	virtual bool onKeyDown(KeySym sym, KeyScanCode scancode, int repeat);
	virtual bool onKeyUp(KeySym sym, KeyScanCode scancode, int repeat);

	struct PointerArgument {
		pointerId id;
		double x, y;
		pointerState state;
	};

	struct KeyArgument {
		KeySym symbol;
		KeyScanCode scanCode;
		int repeats; //0 if first repeat
	};

	//Signals
	Signal <PointerArgument> clicked;
	Signal <PointerArgument> pointerMoved;
	Signal <PointerArgument> pointerDown;
	Signal <PointerArgument> pointerUp;

	Signal <PointerArgument> pointerEnter;
	Signal <PointerArgument> pointerLeave;

	Signal <KeyArgument> keyDown;
	Signal <KeyArgument> keyUp;

	member_property(double, x);
	member_property(double, y);
	member_property(double, width);
	member_property(double, height);
	member_property(double, weight);
	member_property(bool, owned);
	member_property(int, widthFlags);
	member_property(int, heightFlags);

protected:
	double _x, _y;
	double _width, _height;
	int _widthFlags, _heightFlags;
	double _weight = 0;
	bool _owned = true; //Id the widget should be deleted by its parent
};

} //namespace MatGui