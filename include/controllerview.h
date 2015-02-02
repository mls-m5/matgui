/*
 * controllerview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */


#pragma once

#include "view.h"

namespace MatGui {

class ControllerView: public View {
public:
	typedef double _valueType;

	ControllerView():
		elementId(-1),
		controllerId(-1),
		_value(0),
		_min(0),
		_max(1),
		_step(0),
		changed(true){
		changed.onlySaveLast(true);
	}


	ControllerView(int element, int controller):
		ControllerView(){
		elementId = element;
		controllerId = controller;
	}

	//Set the controler with a value from 0 to 1
	void amount(_valueType v) {
		auto newValue = _min + v * (_max - _min);
		if (_step) {
			newValue = roundDown(newValue, _step);
		}
		value(newValue);
	}

	void value(_valueType v){
		if (v != _value) {
			_value = v;
//			changed.emit(v); //Is emitted by key handling event, to avoid round calling
		}
	}

	void setValue(_valueType v) {
		value(v);
	}

	_valueType value() {
		return _value;
	}

	void setLinear(_valueType min, _valueType max, _valueType step){
		this->_min = min;
		this->_max = max;
		this->_step = step;
	}

	void operator=(ControllerView view){
		value(view.value());
	}

	void operator=(_valueType v) {
		value(v);
	}

	Signal<_valueType> changed;

protected:
	_valueType _value;
	_valueType _min, _max, _step;
	int elementId, controllerId;
};


} //namespace Matgui
