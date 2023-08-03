/*
 * shaderProgram.cpp
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#include "matgui/shaderprogram.h"
#include "matgui/gl-error-handling.h"
#include "shaderobject.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

namespace matgui {

GLuint ShaderProgram::createProgram(std::string_view pVertexSource,
                                    std::string_view pFragmentSource,
                                    std::string_view geometryCode) {
    _objects.push_back(
        std::make_unique<ShaderObject>(GL_VERTEX_SHADER, pVertexSource));
    _objects.push_back(
        std::make_unique<ShaderObject>(GL_FRAGMENT_SHADER, pFragmentSource));
#ifndef USING_GL2
    if (!geometryCode.empty()) {
        _objects.push_back(
            std::make_unique<ShaderObject>(GL_GEOMETRY_SHADER, geometryCode));
    }
#endif

    GLuint program = glCreateProgram();
    if (!program) {
        debug_print("glCreateProgram() failed");
    }

    for (auto &o : _objects) {
        glCall(glAttachShader(program, o->shader));
    }

    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint bufLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
        if (bufLength) {
            std::vector<char> buffer(static_cast<size_t>(bufLength));
            glGetProgramInfoLog(program, bufLength, nullptr, &buffer[0]);
            printDebugInfo(buffer.data(), "");
        }
        else {
            debug_print("Shader program linking failed, but with no error log");
        }
        glDeleteProgram(program);
        program = 0;
    }
    checkGlError("before return");

    return program;
}

ShaderProgram::ShaderProgram() {
}

void ShaderProgram::initProgram(const std::string &vertexCode,
                                const std::string &fragmentCode,
                                const std::string &geometryCode) {
    if (_program) {
        glDeleteProgram(_program);
    }
    _program = createProgram(vertexCode, fragmentCode, geometryCode);
}

GLint ShaderProgram::getUniform(char const *name) const {
    GLint ret;
    ret = glGetUniformLocation(_program, name);

    if (ret < 0) {
        debug_print("could not find uniform %s\n", name);
    }
    return ret;
}

GLint ShaderProgram::getAttribute(char const *name) const {
    GLint ret;
    ret = glGetAttribLocation(_program, name);

    if (ret < 0) {
        debug_print("could not find attribute %s\n", name);
    }

    return ret;
}

ShaderProgram::ShaderProgram(const std::string &vertexCode,
                             const std::string &fragmentCode,
                             const std::string &geometryCode) {
    initProgram(vertexCode, fragmentCode, geometryCode);
}

ShaderProgram::~ShaderProgram() {
    clear();
}

StandardShaderProgram::StandardShaderProgram(const std::string &vertexCode,
                                             const std::string &fragmentCode,
                                             const std::string &geometryCode)
    : ShaderProgram(vertexCode, fragmentCode, geometryCode) {

    vertexPointer = getAttribute("vPosition");
    colorPointer = getAttribute("vColor");
    mvpMatrixPointer = getUniform("mvp_matrix");
}

void StandardShaderProgram::disable() {
    if (vertexPointer != -1) {
        glEnableVertexAttribArray(vertexPointer);
    }
    if (colorPointer != -1) {
        glEnableVertexAttribArray(colorPointer);
    }
}

void ShaderProgram::use() const {
    if (_program) {
        glUseProgram(_program);
    }
}

void ShaderProgram::unuse() const {
    glUseProgram(0);
}

void ShaderProgram::clear() {
    if (_program) {
        glDeleteProgram(_program);
    }
}

void ShaderProgram::loadShaderFromFile(const std::string &vertexFile,
                                       const std::string &fragmentFile) {
    std::ifstream vfile(vertexFile);
    if (!vfile) {
        cout << "could not open vertex shader file " << vertexFile << endl;
    }
    std::string code((std::istreambuf_iterator<char>(vfile)),
                     std::istreambuf_iterator<char>());
    std::ifstream ffile(fragmentFile);
    if (!ffile) {
        cout << "could not open fragment shader file " << fragmentFile << endl;
    }
    std::string fcode((std::istreambuf_iterator<char>(ffile)),
                      std::istreambuf_iterator<char>());
    initProgram(code, fcode);
}

} // namespace matgui
