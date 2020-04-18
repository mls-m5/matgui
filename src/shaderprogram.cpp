/*
 * shaderProgram.cpp
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#include "matgui/shaderprogram.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;

namespace {

//// Used when using for example opengl es 3.0
// std::string translate(const std::string &fromSource,
//                      GLenum shaderType,
//                      int fromVersion,
//                      int toVersion) {
//    std::string ret;

//    ret = "precision mediump float;\n";
//    ret += fromSource;
//    return ret;
//}

//! Print info and indicate the right line
void printDebugInfo(std::string info, const std::string &code) {
    using namespace std;

    auto firstColon = info.find(':');
    auto firstParen = info.find('(');
    auto secondParen = info.find(')');

    if (firstColon == string::npos && firstParen == string::npos &&
        secondParen == string::npos) {
        cout << info << endl;
        cout << code << endl;
    }

    auto b = info.begin();
    cout << "line: " << string(b + firstColon + 1, b + firstParen) << endl;
    cout << "col: " << string(b + firstParen + 1, b + secondParen) << endl;

    istringstream ss(code);
    size_t lineNum = stoul(string(b + firstColon + 1, b + firstParen));
    size_t colNum = stoul(string(b + firstParen + 1, b + secondParen));
    string line;
    bool printed = false;

    for (size_t i = 1; getline(ss, line); ++i) {

        cout << line << endl;
        if (printed) {
            continue;
        }

        else if (i < lineNum) {
            continue;
        }

        printed = true;
        for (size_t c = 0; c < colNum; ++c) {
            cout << "-";
        }
        cout << "^ here\n\n";
        break;
    }
    cout << info << endl;

    throw std::runtime_error(info);
}

} // namespace

static GLuint loadShader(GLenum shaderType, const std::string &pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
#ifdef USING_GL2
        if (shaderType == GL_FRAGMENT_SHADER) {
            auto translated = translate(pSource, shaderType, 330, 300);
            const auto ptr = translated.c_str();
            glShaderSource(shader, 1, &ptr, 0);
        }
        else {
            const auto ptr = pSource.c_str();
            glShaderSource(shader, 1, &ptr, 0);
        }
#else
        const auto ptr = pSource.c_str();
        glShaderSource(shader, 1, &ptr, 0);
#endif
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                std::vector<char> buffer(infoLen);
                glGetShaderInfoLog(shader, infoLen, 0, &buffer[0]);
                //                debug_print("Could not compile shader
                //                %d:\n%s\n",
                //                            shaderType,
                //                            &buffer[0]);

                //                debug_print("Shader code: \n%s\n",
                //                pSource.c_str());
                printDebugInfo(buffer.data(), pSource);
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(std::string pVertexSource,
                     std::string pFragmentSource,
                     const std::string &geometryCode) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        debug_print("Shader program: failed creating vertex shader");
        return 0;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!fragmentShader) {
        debug_print("Shader program: failed creating fragment shader");
        return 0;
    }

#ifndef USING_GL2
    GLuint geometryShader = 0;
    if (!geometryCode.empty()) {
        geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryCode.c_str());
    }
#endif

    GLuint program = glCreateProgram();
    if (program) {
        glCall(glAttachShader(program, vertexShader));
        glCall(glAttachShader(program, fragmentShader));
        if (!geometryCode.empty()) {
#ifndef USING_GL2
            glCall(glAttachShader(program, geometryShader));
#endif
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
                //                debug_print("Could not link program:\n%s\n",
                //                &buffer[0]);
            }
            else {
                debug_print(
                    "Shader program linking failed, but with no error log");
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    else {
        debug_print("glCreateProgram() failed");
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

GLint ShaderProgram::getUniform(char const *name) {
    GLint ret;
    ret = glGetUniformLocation(_program, name);

    if (ret < 0) {
        debug_print("could not find uniform %s\n", name);
    }
    //    checkGlError(name);
    return ret;
}

GLint ShaderProgram::getAttribute(char const *name) {
    GLint ret;
    ret = glGetAttribLocation(_program, name);

    if (ret < 0) {
        debug_print("could not find attribute %s\n", name);
    }

    //	checkGlError(name);

    return ret;
}

ShaderProgram::ShaderProgram(const std::string &vertexCode,
                             const std::string &fragmentCode,
                             const std::string &geometryCode) {
    initProgram(vertexCode, fragmentCode, geometryCode);
}

ShaderProgram::~ShaderProgram() {
    if (_program) {
        glDeleteProgram(_program);
    }
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

void ShaderProgram::use() {
    if (_program) {
        glUseProgram(_program);
    }
}

void ShaderProgram::unuse() {
    glUseProgram(0);
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
