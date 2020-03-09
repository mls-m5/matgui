/*
 * textentry.h
 *
 *  Created on: 5 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "fontview.h"
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

    bool onTextInput(const char *text) override;

    void onFocus() override;
    void onUnfocus() override;

    const std::string &text() const {
        return _text;
    }

    void text(const std::string &value);

    Signal<> submit; // Return is pressed

private:
    void updateFontView();

    void eraseOne();
    void deleteOne();

    std::string _text;
    size_t _cursorPosition = 0;
    FontView _fontView;
};

} // namespace MatGui
