/*
 * Font.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <memory>
#include <string>

#ifndef USE_TTF_FONT
#define USE_BITMAP_FONT
#endif

namespace matgui {

class Font {
public:
    Font();
    Font(const std::string, int size);
    Font(int size);
    Font(const Font &font);
    virtual ~Font();

    void draw(double x, double y, const std::string &text, bool centered);

    Font &operator=(const Font &);

    static void DefaultFontPath(std::string path);

    operator bool() const;

protected:
    std::unique_ptr<class FontData> _data;

    friend class FontView;
    friend class FontViewData;
};

} // namespace matgui
