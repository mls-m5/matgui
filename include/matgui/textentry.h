/*
 * textentry.h
 *
 *  Created on: 5 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "font.h"
#include "view.h"

namespace MatGui {

class TextEntry : public View {
public:
    TextEntry();

    ~TextEntry();

    void draw() override;

    bool onKeyDown(KeySym sym,
                   KeyScanCode scancode,
                   KeyModifiers modifiers,
                   int repeat) override;
    bool onKeyUp(KeySym sym,
                 KeyScanCode scancode,
                 KeyModifiers modifiers,
                 int repeat) override;

    const std::string &text() const {
        return _text;
    }

    void text(const std::string &value);

private:

    void updateFontView();

    std::string _text;
    size_t _cursorPosition = 0;
    FontView _fontView;
};

} // namespace MatGui
