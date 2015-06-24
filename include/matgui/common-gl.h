/*
 * common-gl.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


#include "matgui-common.h"

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define GL_BGRA GL_BGRA_EXT

#else

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1

#include <GL/gl.h>
#include <GL/glext.h>
#endif


