/*
 * paint.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "draw.h"
#include "memberproperties.h"
#include <ciso646> //or, and etc


namespace MatGui {

class View;

class ColorStyle {
public:
	float r = 0, g = 0, b = 0, a = 1;
	bool inherit = true;

	//set color and disable inherit flag
	void setColor(float red, float green, float blue) {
		r = red;
		g = green;
		b = blue;
		a = 1;

		if (_style == DrawStyle::None) {
			_style = DrawStyle::Filled;
		}

		inherit = false;
	}

	void setColor(float red, float green, float blue, float alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;

		if (_style == DrawStyle::None) {
			_style = DrawStyle::Filled;
		}

		inherit = false;
	}

	//set style and disable inherit flag
	void style(DrawStyle value) {
		_style = value;
		inherit = false;
	}

	operator bool() {
		return _style != DrawStyle::None;
	}

	//shorthand notation
	member_get(DrawStyle, style);

	void operator += (ColorStyle &s) {
		if (not s.inherit) {
			*this = s;
		}
	}

	//remember to set this to override previous value

protected:
	DrawStyle _style = DrawStyle::Filled;

	friend class Paint;
};

class LineStyle: public ColorStyle {
public:

	void width(float w) {
		_width = w;
		inherit = false;
	}

	member_get(float, width);

protected:
	float _width = 1;
};

class Paint {
public:
	Paint();
	virtual ~Paint();

	ColorStyle fill;
	LineStyle line;

	bool enabled = true;

	void push(Paint &s);

	void operator+=(Paint &s) {
		push(s);
	}

	void drawLine(double x, double y, double x2, double y2);
	void drawRect(double x, double y, double width, double height);
	void drawElipse(double x, double y, double width, double height);
	void drawPolygon(double x, double y, double angle, const std::vector<vec2> &vecList);

	void drawBasicView(View*);
};

} /* namespace MatGui */

