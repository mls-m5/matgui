/*
 * Button.h
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "view.h"


namespace MatGui {

class Button : public View{
public:
	Button();
	virtual ~Button();

	void draw() override;


	virtual void onPointerEnter(pointerId id, double x, double y, pointerState state);
	virtual void onPointerLeave(pointerId id, double x, double y, pointerState state);

	protected:

	bool highlight = false;
};

}
