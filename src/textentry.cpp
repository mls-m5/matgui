/*
 * textentry.cpp
 *
 *  Created on: 5 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/textentry.h"
#include "matgui/keys.h"
#include "matgui/keyutils.h"

namespace matgui {

TextEntry::TextEntry() {
    _fontView.alignment(FontView::Left);
    _fontView.font(Font(30));
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

bool TextEntry::onKeyDown(KeySym /*sym*/,
                          KeyScanCode scancode,
                          KeyModifiers modifiers,
                          int /*repeat*/) {
    if (scancode == Keys::Backspace) {
        if (modifiers & (64 + 128)) { // ctrl left or right
            while (_cursorPosition > 0 &&
                   isspace(_text.at(_cursorPosition - 1))) {
                eraseOne();
            }
            while (_cursorPosition > 0 &&
                   !isspace(_text.at(_cursorPosition - 1))) {
                eraseOne();
            }
            updateFontView();
        }
        else if (_cursorPosition > 0) {
            while (_cursorPosition > 1 &&
                   isUtfTail(_text.at(_cursorPosition - 1))) {
                eraseOne();
            }
            eraseOne();
            updateFontView();
        }
    }
    else if (scancode == Keys::Return) {
        submit.emit();
    }

    return true;
}

bool TextEntry::onKeyUp(KeySym /*sym*/,
                        KeyScanCode /*scancode*/,
                        KeyModifiers /*modifiers*/,
                        int /*repeat*/) {
    return true;
}

bool TextEntry::onTextInput(const char *text) {
    _text.append(text);
    _cursorPosition = _text.size();
    updateFontView();
    return true;
}

void TextEntry::onFocus() {
    beginTextEntry();
    View::onFocus();
}

void TextEntry::onUnfocus() {
    endTextEntry();
    View::onUnfocus();
}

void TextEntry::text(const std::string &value) {
    _text = value;
    updateFontView();
    _cursorPosition = _text.size();
}

void TextEntry::updateFontView() {
    _fontView.text(_text);
}

// Erase one character to the left of cursor without updating the fontview
void TextEntry::eraseOne() {
    if (_cursorPosition > 0) {
        _text.erase(_cursorPosition - 1);
        --_cursorPosition;
    }
}

// Delete one character to the right without updating the fontview
void TextEntry::deleteOne() {
    _text.erase(_cursorPosition);
}

} // namespace matgui
