/*
 * signal-test.cpp
 *
 *  Created on: 1 feb 2015
 *      Author: Mattias Larsson Sköld
 */


#include "unittest.h"
#include "signal.h"
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

	_valueType argument = 0;
	bool isCalled = false;
};

TEST_SUIT_BEGIN

TEST_CASE("void signal callback") {
	Signal<> signal;
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

	ASSERT(testClass.isCalled == 0, "callback was called but should be disconnected");
}

TEST_CASE("signal argument") {
	Signal<int> signal;
	CallbackClass<int> testClass;

	signal.connect(&testClass, &CallbackClass<int>::callBack);

	signal.emit(23);

	flushSignals();

	ASSERT(testClass.isCalled, "callback never called");
	ASSERT_EQ(testClass.argument, 23);
}

TEST_SUIT_END


