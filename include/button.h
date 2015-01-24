/*
 * Button.h
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "view.h"
#include "font.h"
#include <memory>
#include <string>


namespace MatGui {

class Button : public View{
public:
	Button(std::string label = "");
	virtual ~Button();

	void draw() override;


	virtual void onPointerEnter(pointerId id, double x, double y, pointerState state);
	virtual void onPointerLeave(pointerId id, double x, double y, pointerState state);

	protected:

	bool highlight = false;

	std::shared_ptr<Font> font;
	std::string label;
};

}
