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
    ShaderProgram() = default;
    ShaderProgram(ShaderProgram &&s) = delete;
    ShaderProgram(const ShaderProgram &s) = delete;
    ShaderProgram &operator=(const ShaderProgram &) = delete;
    ShaderProgram &operator=(ShaderProgram &&s) = delete;

    /// Apply a object to the shader
    /// Note that this removes the linked program so that it needs to be linked
    /// again
    void addObject(std::shared_ptr<class ShaderObject>);

    /// Load object from a string
    /// type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
    void addObject(GLint type, std::string_view code);

    /// Load file, specify type
    void loadObject(GLint, std::filesystem::path);

    /// Load file: Guess type by file ending .vert, .frag, .geom
    void loadObject(std::filesystem::path);

    /// Used to call the above functions
    ShaderProgram(std::string_view vertexCode,
                  std::string_view fragmentCode,
                  std::string_view geometryCode = {});

    /// Bind the program
    void use() const;

    /// Set the current bound program to 0
    static void unuse();

    GLint uniform(char const *name, bool soft = false) const;
    GLint attribute(char const *name, bool soft = false) const;

    //! Delete the program from opengl context
    void clear();

    /// Create a new opengl program and link the current objects to it
    void link();

    /// Just remove the program
    void unlink();

    GLuint get() const;

    /// Check if the program is successfully linked
    /// Note if you add objects individually you need to call link or use before
    /// the program is being linked
    operator bool();

    virtual ~ShaderProgram();

private:
    GLuint _program = 0;
    std::vector<std::shared_ptr<class ShaderObject>> _objects;
};

} // namespace matgui
