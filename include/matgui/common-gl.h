/*
 * common-gl.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


#include "matgui-common.h"

#ifdef __ANDROID__
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
#define USING_GL2

//#define GL_BGRA GL_BGRA_EXT

#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define GLEW_STATIC

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#define USING_GLEW

#else
#ifdef __EMSCRIPTEN__

#define USING_GL2

#else

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>

#endif
#endif
#endif



#ifdef USING_GL2

#define GL_GLEXT_PROTOTYPES 1

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define glBindVertexArray glBindVertexArrayOES
#define glDeleteVertexArrays glDeleteVertexArraysOES
#define glGenVertexArrays glGenVertexArraysOES

#endif



