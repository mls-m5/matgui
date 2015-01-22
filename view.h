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

struct ViewEvent{
	enum EventType {
		KeyDown,
		KeyUp,
		PointerDown,
		PointerUp,
		PointerMove
	};
//
//	//buttonPressed is only used if type is PointerMove
//	void setPointerEvent(int pointerId, double x, double y, EventType type, bool buttonPressed = 0){
//		pointer.id = pointerId;
//		pointer.x = x;
//		pointer.y = y;
//		this->type = type;
//		switch (type){
//		case PointerDown:
//			pointer.pressed = true;
//			break;
//		case PointerUp:
//			pointer.pressed = false;
//			break;
//		case PointerMove:
//			pointer.pressed = buttonPressed;
//		}
//	}
//
//	void setKeyEvent(int keySym, EventType type){
//		key.sym = keySym;
//		this->type = type;
//	}
//
//	EventType type;
//	union{
//		struct {
//			int sym;
//		} key;
//
//		struct {
//			double x;
//			double y;
//			int id;
//			bool pressed;
//
//		} pointer;
//	};
};

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
	virtual bool handleEvent(const ViewEvent &event) { return false; }
	virtual bool isPointerInside(double x, double y);


	//Callback functions
	virtual bool onPointerDown(pointerId id, double x, double y) {
		pointerDown.emit({id, x, y, 1});
		return true;
	}
	virtual bool onPointerUp(pointerId id, double x, double y) {
		pointerUp.emit({id, x, y, 0});
		if (isPointerInside(x, y)) {
			clicked.emit({id, x, y, 1});
		}
		return true;
	}
	virtual void onPointerMove(pointerId id, double x, double y, pointerState state) {
		pointerMoved.emit({id, x, y, state});
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
