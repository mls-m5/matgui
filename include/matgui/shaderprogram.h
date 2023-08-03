/*
 * shaderProgram.h
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#pragma once

#include "common-gl.h"
#include <filesystem>
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

    ShaderProgram(std::string_view vertexCode,
                  std::string_view fragmentCode,
                  std::string_view geometryCode = "");

    void initProgram(std::string_view vertexCode,
                     std::string_view fragmentCode,
                     std::string_view geometryCode = "");

    void loadShaderFromFile(std::filesystem::path vertexFile,
                            std::filesystem::path fragmentFile,
                            std::filesystem::path geometryFile);
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

    virtual ~ShaderProgram();

private:
    GLuint createProgram(std::string_view pVertexSource,
                         std::string_view pFragmentSource,
                         std::string_view geometryCode);

    GLuint _program = 0;
    std::vector<std::unique_ptr<class ShaderObject>> _objects;
};

} // namespace matgui
