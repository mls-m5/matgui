/*
 * memberproperties.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

// standard getters and setters
#define member_set(type, m)                                                    \
    inline void m(type value) {                                                \
        _##m = value;                                                          \
    }
#define member_get(type, m)                                                    \
    inline type m() const {                                                    \
        return _##m;                                                           \
    }
#define member_property(type, m) member_get(type, m) member_set(type, m)
#define member_property_declaration(type, m)                                   \
public:                                                                        \
    member_get(type, m) member_set(type, m) private : type _##m;
#define member_property_declaration_default(type, m, defValue)                 \
public:                                                                        \
    member_get(type, m) member_set(type, m) private : type _##m = defValue;
