/*
 * shaderProgram.h
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#pragma once

#include "common-gl.h"

#include <string>

class ShaderProgram {
public:
    ShaderProgram(ShaderProgram &&s) {
        this->_program = s._program;
        s._program = 0;
    }
    ShaderProgram(const ShaderProgram &s) = delete;
    ShaderProgram();
    ShaderProgram(const std::string &vertexCode,
                  const std::string &fragmentCode,
                  const std::string &geometryCode = "");
    void initProgram(const std::string &vertexCode,
                     const std::string &fragmentCode,
                     const std::string &geometryCode = "");
    void loadShaderFromFile(const std::string &vertexFile,
                            const std::string &fragmentFile);

    void use();
    void unuse();

    [[deprecated]] inline void useProgram() {
        use();
    }

    GLuint getProgram() {
        return _program;
    }
    GLint getUniform(char const *name);
    GLint getAttribute(char const *name);
    virtual ~ShaderProgram();

private:
    GLuint _program = 0;
};

class StandardShaderProgram : public ShaderProgram {
public:
    GLint vertexPointer;
    GLint colorPointer;
    GLint mvpMatrixPointer;

    StandardShaderProgram(const std::string &vertexCode,
                          const std::string &fragmentCode,
                          const std::string &geometryCode = "");
    void disable();
};
