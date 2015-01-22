/*
 * controllerview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */


#pragma once

#include "view.h"

class ControllerView: public View {
public:
	ControllerView():
		elementId(-1),
		controllerId(-1),
		value(0),
		min(0),
		max(1),
		step(0){}


	ControllerView(int element, int controller):
		ControllerView(){
		elementId = element;
		controllerId = controller;
	}

	void setValue(double v){
		value = v;
	}
	void setController(int element, int controller){
		elementId = element;
		controllerId = controller;
	}
	void setLinear(double min, double max, double step){
		this->min = min;
		this->max = max;
		this->step = step;
	}
	void operator=(ControllerView view){
		value = view.value;
	}

protected:
	double value;
	double min, max, step;
	int elementId, controllerId;
};


