/*
 * Button.h
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "view.h"

//void recieveEvent(int) {
//
//}

#include <iostream>

class Button : public View{
public:
	Button();
	virtual ~Button();

	virtual void onPointerMove(pointerId id, double x, double y,
			pointerState state) override;


	void draw() override;

	protected:

	bool highlight = false;
};
