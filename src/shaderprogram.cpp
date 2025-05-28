/*
 * shaderProgram.cpp
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#include "matgui/shaderprogram.h"
#include "glpp/gl-backend.hpp"
#include "matgui/files.h"
#include "shaderobject.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace matgui {

GLint ShaderProgram::uniform(const char *name, bool soft) const {
    GLint ret = 0;
    ret = glGetUniformLocation(_program, name);

    if (ret < 0 && !soft) {
        debug_print("could not find uniform %s\n", name);
        throw std::runtime_error{"could not find " + std::string{name} +
                                 " shader " + name};
    }
    return ret;
}

GLint ShaderProgram::attribute(char const *name, bool soft) const {
    GLint ret;
    ret = glGetAttribLocation(_program, name);

    if (ret < 0 && !soft) {
        debug_print("could not find attribute %s\n", name);
    }

    return ret;
}

ShaderProgram::ShaderProgram(std::string_view vertexCode,
                             std::string_view fragmentCode,
                             std::string_view geometryCode) {
    addObject(GL_VERTEX_SHADER, vertexCode);
    addObject(GL_FRAGMENT_SHADER, fragmentCode);
    if (!geometryCode.empty()) {
        addObject(GL_GEOMETRY_SHADER, geometryCode);
    }
    link();
}

void ShaderProgram::addObject(std::shared_ptr<ShaderObject> object) {
    if (!object) {
        throw std::runtime_error{"trying to add null shader"};
    }
    _objects.push_back(std::move(object));
    unlink();
}

void ShaderProgram::addObject(GLint type, std::string_view code) {
    if (code.empty()) {
        throw std::runtime_error{"trying to add shader object with no code"};
    }
    addObject(std::make_shared<ShaderObject>(type, code));
}

ShaderProgram::~ShaderProgram() {
    clear();
}

void ShaderProgram::use() const {
    if (_program) {
        glUseProgram(_program);
    }
}

void ShaderProgram::unuse() {
    glUseProgram(0);
}

void ShaderProgram::clear() {
    unlink();

    _objects.clear();
}

void ShaderProgram::link() {
    if (_program) {
        return;
    }

    _program = glCreateProgram();
    if (!_program) {
        debug_print("glCreateProgram() failed");
    }

    for (auto &o : _objects) {
        glCall(glAttachShader(_program, o->shader));
    }

    glLinkProgram(_program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(_program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint bufLength = 0;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &bufLength);
        if (bufLength) {
            std::vector<char> buffer(static_cast<size_t>(bufLength));
            glGetProgramInfoLog(_program, bufLength, nullptr, &buffer[0]);
            ShaderObject::printDebugInfo(buffer.data(), "");
        }
        else {
            debug_print("Shader program linking failed, but with no error log");
        }
        glDeleteProgram(_program);
        _program = 0;
    }

    glCall((void)"after created program");
}

void ShaderProgram::unlink() {
    if (_program) {
        glDeleteProgram(_program);
    }
}

GLuint ShaderProgram::get() const {
    return _program;
}

ShaderProgram::operator bool() {
    return _program;
}

void ShaderProgram::loadObject(GLint type, std::filesystem::path path) {
    const static std::map<int, std::string_view> typeMap = {
        {GL_VERTEX_SHADER, "vertex"},
        {GL_FRAGMENT_SHADER, "fragment"},
        {GL_GEOMETRY_SHADER, "geometry"},
    };

    if (path.empty()) {
        throw std::runtime_error{"trying to load shader with no path"};
    }

    auto file = openFile(path);
    //    std::ifstream file(path);
    if (!file || !*file) {
        std::cout << "could not open " << typeMap.at(type) << " shader file "
                  << path << std::endl;
        throw std::runtime_error{"could not open shader file " + path.string()};
        return;
    }
    std::string code((std::istreambuf_iterator<char>(*file)),
                     std::istreambuf_iterator<char>());
    addObject(type, code);
}

void ShaderProgram::loadObject(std::filesystem::path path) {
    auto ext = path.extension();
    const static std::map<std::string_view, int> typeMap = {
        {".vert", GL_VERTEX_SHADER},
        {".vs", GL_VERTEX_SHADER},
        {".frag", GL_FRAGMENT_SHADER},
        {".fs", GL_FRAGMENT_SHADER},
        {".geom", GL_GEOMETRY_SHADER},
        {".gs", GL_GEOMETRY_SHADER},
    };
    loadObject(typeMap.at(ext.string()), path);
}

} // namespace matgui
