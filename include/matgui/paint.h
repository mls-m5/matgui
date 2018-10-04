/*
 * paint.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "draw.h"
#include "memberproperties.h"


namespace MatGui {

class View;

class ColorStyle {
public:
	float r = 0, g = 0, b = 0, a = 1;

	//remember to set this to override previous value
	bool inherit = true;

	//set color and disable inherit flag
	void color(float red, float green, float blue) {
		r = red;
		g = green;
		b = blue;
		a = 1;

		if (_style == DrawStyle::None) {
			_style = DrawStyle::Filled;
		}

		inherit = false;
	}

	void color(float red, float green, float blue, float alpha) {
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

	operator bool() const {
		return _style != DrawStyle::None;
	}

	//shorthand notation
	member_get(DrawStyle, style);

	void operator += (const ColorStyle &s) {
		if (not s.inherit) {
			*this = s;
		}
	}

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

class ShadowStyle: public ColorStyle {
public:

	void size(vec2 s) {
		_size = s;
		inherit = false;
	}

	void offset(vec2 o) {
		_offset = o;
		inherit = false;
	}

	member_get(vec2, size);

protected:
	vec2 _offset = vec2(0,0);
	vec2 _size = vec2(0,0);
};

class Paint {
public:
	Paint();
	virtual ~Paint();

	ColorStyle fill;
	LineStyle line;
	ShadowStyle shadow;

	bool enabled = true;

	void push(Paint &s);

	void operator+=(Paint &s) {
		push(s);
	}

	void drawLine(double x, double y, double x2, double y2) const;
	void drawRect(double x, double y, double width, double height) const;
	void drawRect(const float *location) const;
	void drawEllipse(double x, double y, double width, double height) const;
	void drawEllipse(const float *location) const;

	void drawBasicView(View*) const;
};

} /* namespace MatGui */

