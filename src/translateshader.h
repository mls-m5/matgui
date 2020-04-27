// Copyright © Mattias Larsson Sköld 2020

#pragma once

#include "matgui/common-gl.h"
#include <string>

// Used when using for example opengl es 3.0
std::string translateShader(const std::string &fromSource, GLenum shaderType);
