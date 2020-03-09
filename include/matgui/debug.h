/*
 * common.h
 *
 *  Created on: 12 sep 2014
 *      Author: mattias
 */

#pragma once

#ifdef __ANDROID__

#include <android/log.h>
#define debug_print(...)                                                       \
    __android_log_print(ANDROID_LOG_INFO, "synth", __VA_ARGS__)
#define debug_write(...)                                                       \
    __android_log_write(ANDROID_LOG_INFO, "synth", __VA_ARGS__)

#else
#include <stdio.h> //printf

#define debug_print(...) printf(__VA_ARGS__)
#define debug_write(...) printf(__VA_ARGS__)

#endif // android-check

#include <ciso646>
