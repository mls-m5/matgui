/*
 * matgui.h
 *
 *  Created on: 23 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "matguifwd.h"
#include <memory>

namespace matgui {

//! The application handles the startup and shutdown of the environment
//! It also handles the main loop, and distributes events to all windows
class Application {
public:
    // Constructor sets up environment
    // Arguments are used to to use program arguments as --scale
    Application(int argc = 0, char **argv = nullptr);

    // Destructor tears down environment
    // Exception is for emscripten when the destructor does nothing
    virtual ~Application();
    void mainLoop();
    bool handleEvents();

    class Window *getWindow(unsigned int w);

    // Stops the main loop
    static void quit();

    // note: frameUpdate signal is moved to window

    // Scale is a global value that sets the scale for graphics.
    // This is to enable same window size on hd screens
    // Change by sending for example "--scale 2" as arguments on construction
    static float Scale();

    // Multi sample anti aliasing
    // Must be set before window is createde to have effect
    // Values must also be something valid for the current GPU
    // Recommended values is 0 4 16 32 etc
    static void MSAA(int numSamples);
    static int MSAA();

    // Used to set scale
    // If used after crating any window the result is undefined
    static void Scale(float scale);

    // Continuous updates is when the screen updates as fast as possible
    // (in sync with the screen updates
    // When disable window::invalidate triggers a redraw
    // Also any input event triggers redraw of the window by default
    static void ContinuousUpdates(bool state);

    // Sets if windows should be invalidated on input events
    // if false window::invalidate needs to be called to redraw
    static void InvalidateOnEvent(bool state);

    static Application *instance();

    // Store application state in a managed fashion
    // Each type has only one instance
    // Each is also destroyed when the application instance is destroyed
    template <typename T>
    static T &state() {
        auto ptr = stateImpl(typeid(T).hash_code());
        if (!ptr) {
            auto state = std::make_unique<State<T>>();
            ptr = state.get();
            saveState(typeid(T).hash_code(), std::move(state));
        }

        return static_cast<State<T> &>(*ptr).value;
    }

private:
    // Functions used internally to handle windows
    void addWindow(class Window *);
    void removeWindow(class Window *);
    void innerLoop();

    friend class Window;

    static Application *_instance;

    struct StateBase {
        virtual ~StateBase() = default;
    };

    template <typename T>
    struct State : public StateBase {
        ~State() override = default;
        T value;
    };

    static StateBase *stateImpl(size_t);

    static void saveState(size_t, std::unique_ptr<StateBase>);

    struct Impl;

    static Impl *impl();

    std::unique_ptr<Impl> _impl;
};

} // namespace matgui
