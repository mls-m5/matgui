/*
 * paint.cpp
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

//#include "paint.h"
#include "view.h"
#include "draw.h"
#include "memberproperties.h"
#include <ciso646>

//
//enum DrawStyle : int { //Testing this is defined in draw.h
//	None = 0,
//	Filled = 1,
//	Lines = 2,
//
//	OrigoTopLeft = 0,
//	CenterOrigo = 4,
//
//	Inherit = 1024,
//};

//namespace MatGui {
//
//	class View;
//
//	class ColorStyle {
//	public:
//		float r = 0, g = 0, b = 0, a = 1;
//		bool inherit = true;
//
//		//set color and disable inherit flag
//		void setColor(float red, float green, float blue) {
//			r = red;
//			g = green;
//			b = blue;
//			a = 1;
//
//			if (_style == DrawStyle::None) {
//				_style = DrawStyle::Filled;
//			}
//
//			inherit = false;
//		}
//
//		void setColor(float red, float green, float blue, float alpha) {
//			r = red;
//			g = green;
//			b = blue;
//			a = alpha;
//
//			if (_style == DrawStyle::None) {
//				_style = DrawStyle::Filled;
//			}
//
//			inherit = false;
//		}
//
//		//set style and disable inherit flag
//		void style(DrawStyle value) {
//			_style = value;
//			inherit = false;
//		}
//
//		operator bool() {
//			return _style != DrawStyle::None;
//		}
//
//		//shorthand notation
//		//member_get(DrawStyle, style);
//		inline DrawStyle style() { return _style; }
//
//		void operator += (ColorStyle &s) {
//			if (not s.inherit) {
//				*this = s;
//			}
//		}
//
//		//remember to set this to override previous value
//
//	protected:
//		DrawStyle _style = DrawStyle::Filled;
//
//		friend class Paint;
//	};
//
//	class LineStyle : public ColorStyle {
//	public:
//
//		void width(float w) {
//			_width = w;
//			inherit = false;
//		}
//
//		member_get(float, width);
//
//	protected:
//		float _width = 1;
//	};
//
//	class Paint {
//	public:
//		Paint();
//		virtual ~Paint();
//
//		ColorStyle fill;
//		LineStyle line;
//
//		bool enabled = true;
//
//		void push(Paint &s);
//
//		void operator+=(Paint &s) {
//			push(s);
//		}
//
//		void drawLine(double x, double y, double x2, double y2);
//		void drawRect(double x, double y, double width, double height);
//		void drawElipse(double x, double y, double width, double height);
//
//		void drawBasicView(View*);
//	};
//
//	//} /* namespace MatGui */
//	//
//	//
//	//
//	//namespace MatGui {
//
//}

using namespace MatGui;

MatGui::Paint::Paint() {
	fill._style = DrawStyle::None;
}

MatGui::Paint::~Paint() {
}

void MatGui::Paint::drawRect(double x, double y, double width, double height) const {
	MatGui::drawRect(x, y, width, height, this);
}

void MatGui::Paint::drawRect(const float *location) const {
	MatGui::drawRect(location, this);
}

void MatGui::Paint::push(Paint& s) {
	if (not s.enabled) {
		return;
	}
	fill += s.fill;
	line += s.line;
}

void MatGui::Paint::drawLine(double x, double y, double x2, double y2) const {
	if (line.style()) {
		MatGui::drawLine(x, y, x2, y2, this);
	}
}

void MatGui::Paint::drawLine(vec v1, vec v2) const {
	if (line.style()) {
		MatGui::drawLine(v1, v2, this);
	}
}

void MatGui::Paint::drawEllipse(double x, double y, double width, double height) const {
	MatGui::drawEllipse(x, y, width, height, this);
}

void MatGui::Paint::drawEllipse(const float *location) const {
	MatGui::drawEllipse(location, this);
}

void MatGui::Paint::drawBasicView(View* view) const {
	drawRect(view->x(), view->y(), view->width(), view->height());
}

void MatGui::Paint::drawTriangle(vec v1, vec v2, vec v3) const {
	MatGui::drawTriangle(v1, v2, v3, this);
}


//} /* namespace MatGui */
