/*
 * fontview.h
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "font.h"

namespace MatGui {

class FontView {
public:
    FontView();
    ~FontView();

    void font(const Font &font) {
        _font = font;
        _needsUpdate = true;
    }

    const Font &font() {
        return _font;
    }

    int width() const {
        return _width;
    }

    int height() const {
        return _height;
    }

    void text(std::string text) {
        if (_text != text) {
            _text = text;
            _needsUpdate = true;
        }
    }

    const std::string &text() const {
        return _text;
    }

    operator bool() const {
        return bool(_font);
    }

    enum Alignment {
        Right = -1,
        Center = 0,
        Left = 1,
    };

    void alignment(Alignment value);
    Alignment alignment();

    void draw(double x, double y);

protected:
    int _width = 0;
    int _height = 0;
    Font _font;
    std::string _text;
    bool _needsUpdate = false;

    class FontViewData;
    std::unique_ptr<FontViewData> _data;
};

} // namespace MatGui
