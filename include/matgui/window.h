/*
 * window.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "linearlayout.h"
#include <memory>
#include <string>

namespace MatGui {

class Window : public LinearLayout {
public:
    Window(std::string title,
           int width = 512,
           int height = 512,
           bool resizable = false);

    virtual ~Window();

    void draw() override;

    Window *root() override {
        return this;
    }

    bool onKeyDown(KeySym sym,
                   KeyScanCode scancode,
                   KeyModifiers modifiers,
                   int repeat) override;

    bool onKeyUp(KeySym sym,
                 KeyScanCode scancode,
                 KeyModifiers modifiers,
                 int repeat) override;

    bool onTextInput(const char *text) override;

    // Functions to be called before and after draw() for window
    void clear();
    void swap();

    virtual bool onRequestClose();

    virtual bool onResize(int width, int height);

    void show();
    void hide();

    // Set fullscreen if state is true or disable with state = false
    // if changeVideoMode is set, the display changes resolution to the windows
    // size return false on success and true on error
    bool fullscreen(bool state = true, bool changeVideoMode = false);

    // Set title of the window
    void title(std::string title);

    // set if the window should have borders or not
    void bordered(bool state);

    // invalidate triggers a redraw of the window
    void invalidate() override {
        invalid(true);
    }

    // check if the window needs redrawing
    bool invalid();
    void invalid(bool state);

    // @see LinearLayout.
    // This is to hide warnings
    void focus() override {
        LinearLayout::focus();
    }

    // @see LinearLayout
    // Explicit to hide warnings
    void unfocus() override {
        LinearLayout::unfocus();
    }

    // Make a object focused, or clear focus by sending in nullptr
    void focus(View *view);
    View *focused();
    // Remove focus only if the focused view is the specified view
    void unfocus(const View *view);

    // Set if the cursor should be visible over the window
    void cursorVisibility(bool value);

    void cursorPosition(int x, int y);
    std::pair<int, int> cursorPosition();

    // A direct call signal
    // if a function returns true closing of the window is aborted
    // if true is returned, the window is closed as usual
    Signal<void, bool> closeSignal;

    // Signal called when the main loop take a step
    // the argument is the time since the last time the signal was called
    // Note: the functions is called directly so it is okay to connect
    // render code to this signal
    MatSig::Signal<double> frameUpdate;

private:
    struct WindowData;
    std::unique_ptr<WindowData> _windowData;

    friend class Application;
};

} /* namespace MatGui */
