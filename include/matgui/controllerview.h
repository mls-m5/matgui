/*
 * controllerview.h
 *
 *  Created on: 23 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "math.h"
#include "view.h"

namespace MatGui {

class ControllerView : public View {
public:
    typedef double _valueType;

    ControllerView()
        : changed(true), _value(0), _min(0), _max(1), _step(0), elementId(-1),
          controllerId(-1) {
        changed.onlySaveLast(true);
    }

    ControllerView(int element, int controller) : ControllerView() {
        elementId = element;
        controllerId = controller;
    }

    // Set the controler with a value from 0 to 1
    void amount(_valueType v) {
        v = std::max(v, 0.);
        v = std::min(v, 1.);

        auto newValue = _min + v * (_max - _min);
        if (_step) {
            newValue = roundDown(newValue, _step);
        }
        value(newValue);
    }

    void value(_valueType v) {
        v = std::max(v, _min);
        v = std::min(v, _max);

        if (v != _value) {
            _value = v;
            //			changed.emit(v); //Is emitted by key handling
            // event, to avoid round calling
        }
    }

    [[deprecated("use value(...) instead")]] void setValue(_valueType v) {
        value(v);
    }

    _valueType value() const {
        return _value;
    }

    [[deprecated("use linear(...) instead")]] //
        void
        setLinear(_valueType min, _valueType max, _valueType step) {
            linear(min, max, step);
        }

    //! Setup min, max and step value for controller
    //! if step is zero the highest available precision is selected
    void linear(_valueType min, _valueType max, _valueType step = 0) {
        this->_min = min;
        this->_max = max;
        this->_step = step;
    }

    void operator=(ControllerView view) {
        value(view.value());
    }

    void operator=(_valueType v) {
        value(v);
    }

    member_property(_valueType, min);
    member_property(_valueType, max);
    member_property(_valueType, step);

    Signal<_valueType> changed;

private:
    _valueType _value;
    _valueType _min, _max, _step;
    int elementId, controllerId;
};

} // namespace MatGui
