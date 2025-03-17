/*
 * signal-test.cpp
 *
 *  Created on: 1 feb 2015
 *      Author: Mattias Larsson Sköld
 */

// #define DO_NOT_CATCH_ERRORS

#include "matgui/matsig.h"
#include "mls-unit-test/unittest.h"

using namespace MatSig;

class VoidCallbackClass {
public:
    void voidCallback() {
        isCalled = true;
    }

    bool isCalled = false;

    void reset() {
        isCalled = false;
    }
};

template <class _valueType>
class CallbackClass {
public:
    void callBack(_valueType arg) {
        isCalled = true;
        argument = arg;
    }

    void reset() {
        isCalled = false;
    }

    _valueType argument = 0;
    bool isCalled = false;
};

static bool functionIsCalled;

static void globalFunction() {
    functionIsCalled = true;
}

static void globalArgumentFunction(int i) {
    functionIsCalled = true;
}

TEST_SUIT_BEGIN(MatguiSignal)

TEST_CASE("void signal callback") {
    Signal<void> signal;
    VoidCallbackClass testClass;

    signal.connect(&testClass, &VoidCallbackClass::voidCallback);
    signal.emit();

    ASSERT(testClass.isCalled == 0, "callback called to early");

    flushSignals();

    ASSERT(testClass.isCalled, "callback was never called");

    testClass.reset();

    signal.disconnect(&testClass);

    signal.emit();
    flushSignals();

    ASSERT(testClass.isCalled == 0,
           "callback was called but should be disconnected");
}

TEST_CASE("signal argument") {
    Signal<int> signal;
    CallbackClass<int> testClass;

    signal.connect(&testClass, &CallbackClass<int>::callBack);

    signal.emit(23);

    flushSignals();

    ASSERT(testClass.isCalled, "callback never called");
    ASSERT_EQ(testClass.argument, 23);

    testClass.reset();

    signal.disconnect(&testClass);

    signal.emit(23);
    flushSignals();

    ASSERT(testClass.isCalled == 0,
           "callback was called but should be disconnected");
}

TEST_CASE("lambda argument function") {
    Signal<int> signal;

    bool isCalled = false;
    int y = 0;

    signal.connect([&isCalled, &y](int x) {
        isCalled = true;
        y = x;
    });

    signal.emit(3);
    flushSignals();

    ASSERT(isCalled, "funktionen function not called");
    ASSERT_EQ(y, 3);
}

TEST_CASE("lambda void argument function") {
    Signal<void> signal;
    bool isCalled = false;
    int y = 0;

    auto function = [&isCalled, &y]() {
        isCalled = true;
        y = 5;
    };
    signal.connect(function);

    signal.emit();
    flushSignals();

    ASSERT(isCalled, "the function not called");

    ASSERT_EQ(y, 5);

    isCalled = false;
    signal.disconnectAll();

    signal.emit();
    signal.flush();

    ASSERT(not isCalled, "function is called but should be disconnected");
}

TEST_CASE("lambda direct call") {
    Signal<int> signal;

    int x = 0;

    signal.connect([&x](int) { x = 1; });
    signal.directCall(1);

    ASSERT_EQ(x, 1);
}

TEST_CASE("std::bind") {
    Signal<void> signal;
    VoidCallbackClass testClass;

    std::function<void()> function =
        std::bind(&VoidCallbackClass::voidCallback, &testClass);
    signal.connect(function);
    signal.emit();

    ASSERT(testClass.isCalled == 0, "callback called to early");

    flushSignals();

    ASSERT(testClass.isCalled, "callback was never called");

    testClass.reset();

    signal.clearQueue();
    signal.disconnectAll();

    signal.emit();
    flushSignals();

    ASSERT(not testClass.isCalled,
           "callback was called but should be disconnected");
}

TEST_CASE("standard function") {
    Signal<void> signal;
    functionIsCalled = false;

    signal.connect(globalFunction);

    signal.emit();
    flushSignals();

    ASSERT(functionIsCalled, "global function was not called");

    functionIsCalled = false;

    signal.disconnect(globalFunction);

    signal.emit();
    flushSignals();
    ASSERT(not functionIsCalled,
           "function is called but should be disconnected");
}

TEST_CASE("standard function with arguments") {
    Signal<int> signal;
    functionIsCalled = false;

    signal.connect(globalArgumentFunction, (void *)globalArgumentFunction);

    signal.emit(1);
    flushSignals();

    ASSERT(functionIsCalled, "global function was not called");

    functionIsCalled = false;

    signal.disconnect(globalArgumentFunction);

    signal.emit(1);
    flushSignals();
    ASSERT(not functionIsCalled,
           "function is called but should be disconnected");
}

TEST_CASE("void direct call") {
    {
        Signal<void> signal;
        bool isCalled = false;

        signal.connect([&isCalled]() { isCalled = true; });

        signal.directCall();

        ASSERT(isCalled, "function never called");
    }

    {
        Signal<int> signal;
        int x = 0;

        signal.connect([&x](int) { x = 20; });

        signal.directCall(10);

        ASSERT(x == 20, "function never called");
    }
}

TEST_CASE("return value from direct call") {
    {
        Signal<void, int> signal;

        signal.connect([]() { return 24; });

        int ret = signal.directCall();
        ASSERT_EQ(ret, 24);
    }
    {
        Signal<int, int> signal;

        signal.connect([](int x) { return x; });

        ASSERT_EQ(signal.directCall(25), 25);
    }
}

TEST_CASE("disconnect inside flush") {
    Signal<int> signal;

    void *ref = (void *)1;
    int count = 0;

    signal.connect(
        [&](int x) {
            signal.disconnect(ref);
            ++count;
        },
        ref);

    signal.emit(1);
    flushSignals();

    ASSERT_EQ(count, 1);

    signal.emit(1);
    flushSignals();

    ASSERT_EQ(count, 1);
}

TEST_SUIT_END
