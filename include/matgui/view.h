/*
 * View.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "matsig.h"
#include "memberproperties.h"
#include "style.h"

namespace matgui {

enum MouseButton {
    MOUSE_LEFT = 1,
    MOUSE_MIDDLE = 1 << 1,
    MOUSE_RIGHT = 1 << 2,
    MOUSE_X1 = 1 << 3,
    MOUSE_X2 = 1 << 4,
};

using MatSig::Signal;
class View {
public:
    enum widthFlags {
        VIEW_MATCH_PARENT = -1,
        VIEW_WRAP_CONTENT = -2,
        VIEW_WEIGHTED = 0,
        VIEW_FIXED = 1
    };

    typedef int pointerId;
    typedef int pointerState;

    View();
    View(const View &) = delete;
    View(View &&) = delete;
    View &operator=(const View &) = delete;
    View &operator=(View &&) = delete;
    virtual ~View();

    virtual void draw() {
        currentStyle.drawBasicView(this);
    }

    //! Trigger a redraw
    virtual void invalidate();
    virtual void refresh(){};
    virtual void updateStyle() {
        currentStyle = style;
        currentStyle += hoverStyle;
        currentStyle += focusStyle;
    }
    virtual void focus();
    virtual void unfocus();

    [[deprecated]] virtual void setLocation(
        double x, double y, double w, double h, double weight = 1) {
        location(x, y, w, h, weight);
    }

    virtual void location(
        double x, double y, double w, double h, double weight = 1);
    virtual bool isPointerInside(double x, double y);
    virtual bool isPointerInsideLocal(double localx, double localy);

    // Callback functions return true to capture the mouse focus
    virtual bool onPointerDown(pointerId id,
                               MouseButton button,
                               double x,
                               double y);
    virtual bool onPointerUp(pointerId id,
                             MouseButton button,
                             double x,
                             double y);
    virtual bool onPointerMove(pointerId id,
                               double x,
                               double y,
                               pointerState state);
    virtual void onPointerEnter(pointerId id,
                                double x,
                                double y,
                                pointerState state);
    virtual void onPointerLeave();
    virtual void onScroll(pointerId id, double x, double y);

    virtual void onFocus();
    virtual void onUnfocus();

    typedef unsigned KeySym;
    typedef unsigned KeyScanCode;

    // Example
    // left shift = 1
    // right shift = 2
    // left ctrl = 64
    // right ctrl = 128
    typedef unsigned KeyModifiers;

    //! Handle key events
    //! Returns true if handled and false otherwise
    virtual bool onKeyDown(KeySym sym,
                           KeyScanCode scancode,
                           KeyModifiers modifiers,
                           int repeat);
    virtual bool onKeyUp(KeySym sym,
                         KeyScanCode scancode,
                         KeyModifiers modifiers,
                         int repeat);

    //! Text input after beginTextInput (from keys.h) is called
    //! returns true if handled
    virtual bool onTextInput(const char *text);

    //! Return the root window for the layout
    virtual class Window *root();

    struct PointerArgument {
        pointerId id; // Always 0 for mouse
        double x, y;
        pointerState state; // A bit representing which buttons pressed 1 for
                            // left, 2 for middle 4 for right etc.
        operator std::tuple<double &, double &>() {
            return {x, y};
        }
    };

    struct ScrollArgument {
        pointerId id;
        double x, y;
        operator std::tuple<double &, double &>() {
            return {x, y};
        }
    };

    struct KeyArgument {
        KeySym symbol;
        KeyScanCode scanCode;
        KeyModifiers modifier;
        int repeats; // 0 if first repeat
    };

    // Signals
    Signal<PointerArgument> clicked;
    Signal<PointerArgument> pointerMoved;
    Signal<PointerArgument> pointerDown;
    Signal<PointerArgument> pointerUp;

    Signal<PointerArgument> pointerEnter;
    Signal<> focused;
    Signal<> unfocused;
    Signal<> pointerLeave;
    Signal<ScrollArgument> scroll;

    Signal<KeyArgument> keyDown;
    Signal<KeyArgument> keyUp;
    Signal<std::string> textInput;

    member_property(double, x);
    member_property(double, y);
    member_property(double, width);
    member_property(double, height);
    member_property(double, weight);
    member_property(int, widthFlags);
    member_property(int, heightFlags);
    member_property(std::string, name);
    member_property(class Layout *, parent);
    member_property(bool, focusable);

    void highlight(bool value) {
        if (_highlight == value) {
            return;
        }
        _highlight = value;
        hoverStyle.enabled = value;
        updateStyle();
    }

    bool highlight() {
        return _highlight;
    }

    // Helper functions
    double right() {
        return x() + width();
    }

    double bottom() {
        return y() + height();
    }

    // This is the style that is used to paint the view
    Paint currentStyle;

    // Style enabled when mouse is hovering
    Paint hoverStyle;

    // Style enabled when view got focus
    Paint focusStyle;

    // This is the style of the view with no changes to it
    Paint style;

private:
    double _x = 0, _y = 0;
    double _width = 0, _height = 0;
    int _widthFlags = 0, _heightFlags = 0;
    double _weight = 0;
    bool _highlight = false;
    std::string _name;
    class Layout *_parent = nullptr;
    bool _focusable = true;
};

} // namespace matgui
