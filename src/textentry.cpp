/*
 * textentry.cpp
 *
 *  Created on: 5 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/textentry.h"
#include "matgui/keys.h"

namespace MatGui {

TextEntry::TextEntry() {
    _fontView.alignment(FontView::Left);
    _fontView.font(Font(30));
    style.line.color(1, 1, 1, .3);
    updateStyle();
}

TextEntry::~TextEntry() {
}

void TextEntry::draw() {
    currentStyle.drawBasicView(this);

    if (_fontView) {
        _fontView.draw(x(), y() + height() / 2.);
    }
}

bool TextEntry::onKeyDown(KeySym sym,
                          KeyScanCode scancode,
                          KeyModifiers modifiers,
                          int repeat) {
    if (scancode == Keys::Backspace) {
        _text.pop_back();
        updateFontView();
    }
    if (isalnum(sym)) {
        _text.push_back(static_cast<char>(sym));
        updateFontView();
    }
    else {
        return false; // Not handled
    }

    return true;
}

bool TextEntry::onKeyUp(KeySym sym,
                        KeyScanCode scancode,
                        KeyModifiers modifiers,
                        int repeat) {
    return true;
}

void TextEntry::text(const std::string &value) {
    _text = value;
    updateFontView();
    _cursorPosition = _text.size();
}

void TextEntry::updateFontView() {
    _fontView.text(_text);
}

} // namespace MatGui
