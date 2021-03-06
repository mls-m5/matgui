/*
 * Signal.cpp
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/matsig.h"
#include <ciso646>
#include <iostream>
#include <thread>

namespace MatSig {

static std::list<SignalBase *> signals;
static std::list<SignalBase *> deletedSignals;
typedef std::lock_guard<std::mutex> LockGuard;
static std::thread::id mainThreadId;

static inline void clearDeletedSignals() {
    if (not deletedSignals.empty()) {
        for (auto it : deletedSignals) {
            signals.remove(it);
        }
        deletedSignals.clear();
    }
}

void SignalBase::flushAll() {
    setMainThread();
    clearDeletedSignals();
    for (auto it = signals.begin(); it != signals.end();) {
        (*it)->flush();
        // Check if signal has been deleted since last call
        bool hasBeenDeleted = false;
        if (not deletedSignals.empty()) {
            for (auto jt = deletedSignals.begin(); jt != deletedSignals.end();
                 ++jt) {
                if (*jt == *it) {
                    it = signals.erase(jt);
                    jt = deletedSignals.erase(jt); // clear from removed list
                    hasBeenDeleted = true;
                }
            }
            clearDeletedSignals(); // clear deleted signals that is not the
                                   // current signal
        }
        if (not hasBeenDeleted) {
            ++it; // iterate as usual
        }
    }
    clearDeletedSignals();
}

void setMainThread() {
    mainThreadId = std::this_thread::get_id();
}

void assertMainThread() {
#ifndef __ANDROID__
    if (std::this_thread::get_id() != mainThreadId) {
        std::cerr
            << "Error! Assert This function must be called from main thread"
            << std::endl;
        throw std::runtime_error(
            "Assert This function must be called from main thread");
    }
#endif
}

// Todo: implement delete later method
SignalBase::~SignalBase() noexcept(false) {
#ifndef __ANDROID__
    if (std::this_thread::get_id() != mainThreadId) {
        std::cerr << "Error! Signals (and views) must be created and removed "
                     "from the same thread"
                  << std::endl;
        throw std::runtime_error("Signals (and views) must be created and "
                                 "deleted from the same thread");
    }
#endif
    deletedSignals.push_back(this);
}

SignalBase::SignalBase() {
    // Check so that a new signal not has been allocated in the place of a
    // deleted signal
    for (auto it = deletedSignals.begin(); it != deletedSignals.end();) {
        if (*it == this) {
            it = deletedSignals.erase(it);
            return; // Never mind adding it if it already is in the list
        }
        else {
            ++it;
        }
    }

    signals.push_back(this);
}

} // namespace MatSig
