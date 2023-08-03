/*
 * shaderProgram.h
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#pragma once

#include "common-gl.h"
#include <memory>
#include <string>
#include <vector>

namespace matgui {

class ShaderProgram {
public:
    ShaderProgram();
    ShaderProgram(ShaderProgram &&s) = delete;
    ShaderProgram(const ShaderProgram &s) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram &operator=(ShaderProgram &&s) = delete;
    ShaderProgram(const std::string &vertexCode,
                  const std::string &fragmentCode,
                  const std::string &geometryCode = "");

    void initProgram(const std::string &vertexCode,
                     const std::string &fragmentCode,
                     const std::string &geometryCode = "");

    void loadShaderFromFile(const std::string &vertexFile,
                            const std::string &fragmentFile);
    void use() const;
    void unuse() const;

    GLint getUniform(char const *name) const;
    GLint getAttribute(char const *name) const;

    //! Delete the program from opengl context
    void clear();

    GLuint get() const {
        return _program;
    }

    operator bool() {
        return _program;
    }

    [[deprecated("use get() instead")]] GLuint getProgram() const {
        return get();
    }

    virtual ~ShaderProgram();

private:
    GLuint createProgram(std::string_view pVertexSource,
                         std::string_view pFragmentSource,
                         std::string_view geometryCode);

    GLuint _program = 0;
    std::vector<std::unique_ptr<class ShaderObject>> _objects;
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

} // namespace matgui
