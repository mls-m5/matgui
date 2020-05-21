/*
 * View.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: mattias
 */

#include "matgui/view.h"
#include "matgui/layout.h"
#include "matgui/window.h"

namespace MatGui {

View::View() : _weight(1) {
    location(0, 0, VIEW_WEIGHTED, VIEW_WEIGHTED);
    hoverStyle.enabled = false;
    focusStyle.enabled = false;
    updateStyle();
}

View::~View() {
    if (_parent) {
        throw std::runtime_error(
            "View deleted with active parent, remove from layout instead");
    }
}

void View::invalidate() {
    if (auto r = root()) {
        r->invalidate();
    }
}

void View::focus() {
    Window *window = root();
    if (window) {
        window->focus(this);
    }
}

void View::unfocus() {
    Window *window = root();
    if (window) {
        window->unfocus(this);
    }
}

void View::location(double x, double y, double w, double h, double weight) {
    _x = x;
    _y = y;
    _width = w;
    _height = h;
    this->_weight = weight;
    if (_width <= 0) {
        _widthFlags = w;
    }
    else {
        _widthFlags = 1;
    }
    if (_height <= 0) {
        _heightFlags = h;
    }
    else {
        _heightFlags = 1;
    }
}

bool View::isPointerInside(double x, double y) {
    if (x >= _x && y >= _y) {
        if (x <= _x + _width && y <= _y + _height) {
            return true;
        }
    }
    return false;
}

bool View::isPointerInsideLocal(double x, double y) {
    if (x >= 0 && y >= 0) {
        if (x <= _width && y <= _height) {
            return true;
        }
    }
    return false;
}

bool View::onPointerDown(pointerId id, MouseButton button, double x, double y) {
    if (focusable()) {
        focus();
    }
    pointerDown.emit({id, x, y, button});
    return true;
}

bool View::onPointerUp(pointerId id, MouseButton button, double x, double y) {
    pointerUp.emit({id, x, y, button});
    if (isPointerInsideLocal(x, y)) {
        clicked.emit({id, x, y, button});
    }
    return true;
}

bool View::onPointerMove(pointerId id, double x, double y, pointerState state) {
    pointerMoved.emit({id, x, y, state});
    return true;
}

void View::onPointerEnter(pointerId id,
                          double x,
                          double y,
                          pointerState state) {
    highlight(true);
    pointerEnter.emit({id, x, y, state});
}

void View::onPointerLeave() {
    highlight(false);
    pointerLeave.emit();
}

void View::onScroll(pointerId id, double x, double y) {
    scroll.emit({id, x, y});
}

void View::onFocus() {
    focused.emit();
    focusStyle.enabled = true;
    updateStyle();
}

void View::onUnfocus() {
    unfocused.emit();
    focusStyle.enabled = false;
    updateStyle();
}

bool View::onKeyDown(KeySym sym,
                     KeyScanCode scancode,
                     KeyModifiers modifiers,
                     int repeat) {
    // If there is no listener the key go to another view
    if (keyDown) {
        if (_focusable) {
            focus();
        }
        keyDown.emit({sym, scancode, modifiers, repeat});
        return true;
    }
    else {
        return false;
    }
}

bool View::onKeyUp(KeySym sym,
                   KeyScanCode scancode,
                   KeyModifiers modifiers,
                   int repeat) {
    if (keyUp) {
        keyUp.emit({sym, scancode, modifiers, repeat});
        return true;
    }
    else {
        return false;
    }
}

bool View::onTextInput(const char * /*text*/) {
    return false;
}

class Window *View::root() {
    return parent() ? parent()->root() : nullptr;
}

} // namespace MatGui
