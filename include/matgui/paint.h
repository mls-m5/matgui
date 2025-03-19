/*
 * paint.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "draw.h"
#include "memberproperties.h"

namespace matgui {

class View;

class ColorStyle {
public:
    float r = 0, g = 0, b = 0, a = 1;

    // remember to set this to override previous value
    bool inherit = true;

    constexpr ColorStyle() = default;
    constexpr ColorStyle(const ColorStyle &) = default;
    constexpr ColorStyle(ColorStyle &&) = default;
    constexpr ColorStyle &operator=(const ColorStyle &) = default;
    constexpr ColorStyle &operator=(ColorStyle &&) = default;

    constexpr ColorStyle(float red,
                         float green,
                         float blue,
                         float alpha = 1.f) {
        color(red, green, blue, alpha);
    }

    // set color and disable inherit flag
    constexpr ColorStyle &color(float red,
                                float green,
                                float blue,
                                float alpha = 1.f) {
        r = red;
        g = green;
        b = blue;
        a = alpha;

        if (_style == DrawStyle::None) {
            _style = DrawStyle::Filled;
        }

        inherit = false;
        return *this;
    }

    constexpr ColorStyle &color(long longColor, float alpha = 1) {
        r = (float)((longColor & 0xFF000000) >> 24) / 255.;
        g = (float)((longColor & 0x00FF0000) >> 16) / 255.;
        b = (float)((longColor & 0x0000FF00) >> 8) / 255.;
        a = alpha;

        if (_style == DrawStyle::None) {
            _style = DrawStyle::Filled;
        }

        inherit = false;
        return *this;
    }

    // set style and disable inherit flag
    constexpr ColorStyle &style(DrawStyle value) {
        _style = value;
        inherit = false;
        return *this;
    }

    constexpr operator bool() const {
        return _style != DrawStyle::None;
    }

    // shorthand notation
    member_get(DrawStyle, style);

    void operator+=(const ColorStyle &s) {
        if (not s.inherit) {
            *this = s;
        }
    }

protected:
    DrawStyle _style = DrawStyle::Filled;

    friend class Paint;
};

class LineStyle : public ColorStyle {
public:
    using ColorStyle::ColorStyle;

    void width(float w) {
        _width = w;
        inherit = false;
    }

    member_get(float, width);

protected:
    float _width = 1;
};

class ShadowStyle : public ColorStyle {
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
    vec2 _offset = vec2(0, 0);
    vec2 _size = vec2(0, 0);
};

class Paint {
public:
    ColorStyle fill = ColorStyle().style(DrawStyle::None);
    LineStyle line;
    ShadowStyle shadow;

    bool enabled = true;

    void push(Paint &s);

    void operator+=(Paint &s) {
        push(s);
    }

    void drawLine(double x, double y, double x2, double y2) const;
    void drawLine(vec v1, vec v2) const;
    void drawTriangle(vec v1, vec v2, vec v3) const;
    void drawRect(double x, double y, double width, double height) const;
    void drawRect(const float *location) const;
    void drawEllipse(double x, double y, double width, double height) const;
    void drawEllipse(const float *location) const;

    void drawBasicView(View *) const;

    void clear() {
        *this = {};
    }
};

} /* namespace matgui */
