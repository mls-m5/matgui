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

	void label(const std::string value) {
		_label = value;
	}

	const std::string &label() {
		return _label;
	}

	void font(std::shared_ptr<Font> font) {
		_font = font;
	}

	protected:


	std::shared_ptr<Font> _font;
	std::string _label;
};

}
