/*
 * View.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "matgui-common.h"
#include "matsig.h"
#include "style.h"
#include "memberproperties.h"

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

	virtual void draw() {
		currentStyle.drawBasicView(this);
	}

	virtual void refresh() {};
	virtual void updateStyle() {
		currentStyle = style;
		currentStyle += hoverStyle;

	}
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
	virtual void onPointerLeave();

	typedef unsigned KeySym;
	typedef unsigned KeyScanCode;
	typedef unsigned KeyModifiers;

	virtual bool onKeyDown(KeySym sym, KeyScanCode scancode, KeyModifiers modifiers, int repeat);
	virtual bool onKeyUp(KeySym sym, KeyScanCode scancode, KeyModifiers modifiers, int repeat);

	struct PointerArgument {
		pointerId id;
		double x, y;
		pointerState state;
	};

	struct KeyArgument {
		KeySym symbol;
		KeyScanCode scanCode;
		KeyModifiers modifier;
		int repeats; //0 if first repeat
	};

	//Signals
	Signal <PointerArgument> clicked;
	Signal <PointerArgument> pointerMoved;
	Signal <PointerArgument> pointerDown;
	Signal <PointerArgument> pointerUp;

	Signal <PointerArgument> pointerEnter;
	Signal <> pointerLeave;

	Signal <KeyArgument> keyDown;
	Signal <KeyArgument> keyUp;

	member_property(double, x);
	member_property(double, y);
	member_property(double, width);
	member_property(double, height);
	member_property(double, weight);
	member_property(int, widthFlags);
	member_property(int, heightFlags);
	member_property(std::string, name);
	member_property(class Layout *,parent);

	void highlight(bool value) {
		if (_highlight == value) {
			return;
		}
		_highlight = value;
		hoverStyle.enabled = value;
		updateStyle();
	}

	bool highlight() {
		return _highlight;
	}

	//Helper functions
	double right() {
		return x() + width();
	}

	double bottom() {
		return y() + height();
	}

	//This is the style that is used to paint the view
	Paint currentStyle;

	//This is styles that is enabled or disabled depending on the views state
	Paint hoverStyle;

	//This is the style of the view with no changes to it
	Paint style;

protected:
	double _x, _y;
	double _width, _height;
	int _widthFlags, _heightFlags;
	double _weight = 0;
	bool _highlight = false;
	std::string _name;
	class Layout *_parent = nullptr;
};

} //namespace MatGui
