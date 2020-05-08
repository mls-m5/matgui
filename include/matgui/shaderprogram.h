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
    ShaderProgram(ShaderProgram &&s);
    ShaderProgram(const ShaderProgram &s) = delete;
    ShaderProgram();
    ShaderProgram(const std::string &vertexCode,
                  const std::string &fragmentCode,
                  const std::string &geometryCode = "");

    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram &operator=(ShaderProgram &&s);

    void initProgram(const std::string &vertexCode,
                     const std::string &fragmentCode,
                     const std::string &geometryCode = "");
    void loadShaderFromFile(const std::string &vertexFile,
                            const std::string &fragmentFile);
    void use() const;
    void unuse() const;

    //! Delete the program from opengl context
    void clear();

    [[deprecated]] inline void useProgram() {
        use();
    }

    GLuint getProgram() const {
        return _program;
    }
    GLint getUniform(char const *name) const;
    GLint getAttribute(char const *name) const;

    operator bool() {
        return _program;
    }

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
