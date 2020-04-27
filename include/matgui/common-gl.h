/*
 * common-gl.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "debug.h"

#ifdef __ANDROID__
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#define USING_GL2

#define MATGUI_INTERNAL_TEXTURE_FORMAT GL_BGRA

#else
#if defined(WIN32) || defined(_WIN32) ||                                       \
    defined(__WIN32) && !defined(__CYGWIN__)

#define GLEW_STATIC

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#define USING_GLEW

#else
#ifdef __EMSCRIPTEN__

#define USING_GL2

#else

#define USING_GL3

#endif
#endif
#endif

#ifdef USING_GL3

#define MAT_GL_VERSION 3

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>

#endif

#ifdef USING_GL2

#define MAT_GL_VERSION 2

#define GL_GLEXT_PROTOTYPES 1

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glGenVertexArrays glGenVertexArraysOES
#define glFramebufferTexture glFramebufferTextureOES

#endif

// Texture format

#ifdef __ANDROID__

#define MATGUI_INTERNAL_TEXTURE_FORMAT GL_BGRA

#else

#define MATGUI_INTERNAL_TEXTURE_FORMAT GL_RGBA

#endif
