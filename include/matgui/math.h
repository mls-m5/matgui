/*
 * math.h
 *
 *  Created on: 9 mars 2020
 *      Author: Mattias Larsson Sköld
 */

#pragma once

namespace MatGui {

inline double roundDown(double value, double factor = 1.) {
    return factor * (double)((int)(value / factor));
}

inline double roundMiddle(double value, double factor = 1.) {
    return factor * (double)((int)(value / factor + .5));
}

} // namespace MatGui
