/*
 * common.h
 *
 *  Created on: 12 sep 2014
 *      Author: mattias
 */

#pragma once

#ifdef __ANDROID__

#include <android/log.h>
#define debug_print(...) __android_log_print(ANDROID_LOG_INFO, "synth", __VA_ARGS__)
#define debug_write(...) __android_log_write(ANDROID_LOG_INFO, "synth", __VA_ARGS__)


#else
#include <stdio.h> //printf

#define debug_print(...) printf(__VA_ARGS__)
#define debug_write(...) printf(__VA_ARGS__)

#endif //android-check

namespace MatGui {

constexpr double pi = 3.1415926535897932384626433832795028841971693;
constexpr double pi2 = pi * 2;
constexpr float pif = pi;
constexpr float pi2f = pi2;


inline double roundDown(double value, double factor = 1.){
	return factor * (double)((int)(value / factor));
}

inline double roundMiddle(double value, double factor = 1.){
	return factor * (double)((int)(value / factor + .5));
}

} //namespace MatGui

