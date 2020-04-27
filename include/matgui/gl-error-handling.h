// Copyright © Mattias Larsson Sköld 2020

#pragma once

#include "common-gl.h"

inline void printGLString(const char *name, GLenum s) {
#ifndef NO_GRAPHICS
    const auto *v = glGetString(s);
    debug_print("GL %s = %s\n", name, v);
#endif
}

inline int checkGlError(const char *op, bool throwError = false) {
#ifndef NO_GRAPHICS
    bool ret = false;
    for (auto error = glGetError(); error; error = glGetError()) {
        const char *c = nullptr;
        switch (error) {
        case 0x0500:
            c = "GL_INVALID_ENUM";
            break;
        case 0x0501:
            c = "GL_INVALID_VALUE";
            break;
        case 0x0502:
            c = "GL_INVALID_OPERATION";
            break;
        case 0x0503:
            c = "GL_STACK_OVERFLOW";
            break;
        case 0x0504:
            c = "GL_STACK_UNDERFLOW";
            break;
        case 0x0505:
            c = "GL_OUT_OF_MEMORY";
            break;
        case 0x0506:
            c = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case 0x0507:
            c = "GL_CONTEXT_LOST";
            break;
        case 0x8031:
            c = "GL_TABLE_TOO_LARGE1";
            break;
        }
        debug_print("after %s()\n glError (0x%x) %s \n\n", op, error, c);
        printGLString(op, error);
        if (throwError) {
            printf("%s\n", c);
            throw c;
        }
    }
    return ret;
#else
    return false;
#endif
}

#ifdef NDEBUG
#define glCall(call) call;
#else
#define glCall(call)                                                           \
    call;                                                                      \
    if (checkGlError(#call, true)) {                                           \
        std::cout << "at" << __FILE__ << ":" << __LINE__ << std::endl;         \
    };
#endif
