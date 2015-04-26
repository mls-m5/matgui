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

class Button : public View {
public:
	Button(std::string label = "");
	virtual ~Button();

	void draw() override;

	void label(const std::string text) {
		_fontView.text(text);
	}

	const std::string &label() {
		return _fontView.text();
	}

	void font(Font font) {
		_fontView.font(font);
	}

	protected:


	FontView _fontView;
};

}
