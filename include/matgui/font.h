/*
 * Font.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <memory>
#include <string>

namespace MatGui {

class Font {
public:
    Font();
    Font(const std::string, int size);
    Font(int size);
    Font(const Font &font);
    virtual ~Font();

    void draw(double x, double y, const std::string &text, bool centered);

    void operator=(const Font &);

    [[deprecated("use DefaultFontPath instead")]] static void
    setDefaultFontPath(std::string path) {
        DefaultFontPath(path);
    }

    static void DefaultFontPath(std::string path);

    operator bool() const;

protected:
    std::unique_ptr<class FontData> _data;

    friend class FontView;
    friend class FontViewData;
};

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

    std::unique_ptr<class FontViewData> _data;

    friend class FontViewData;
};

} /* namespace MatGui */
