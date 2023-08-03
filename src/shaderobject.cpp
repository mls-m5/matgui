#include "shaderobject.h"
#include "translateshader.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace matgui {

//! Print info and indicate the right line
[[noreturn]] void ShaderObject::printDebugInfo(std::string_view info,
                                               std::string_view code) {
    using namespace std;

#ifdef __EMSCRIPTEN__
    cout << info << endl;
    cout << code << endl;

#endif

    if (info.substr(0, 7) == "ERROR: ") {
        info = info.substr(7);
    }

    auto firstColon = info.find(':');
    auto firstParen = info.find('(');
    auto secondParen = info.find(')');

    if (firstColon == string::npos && firstParen == string::npos &&
        secondParen == string::npos) {
        cout << info << endl;
        cout << code << endl;
    }

    cout << info;
    //    auto b = info.begin();
    //    cout << "line: " << string(b + firstColon + 1, b + firstParen) <<
    //    endl; cout << "col: " << string(b + firstParen + 1, b + secondParen)
    //    << endl;

    //    istringstream ss(std::string{code});
    //    size_t lineNum = stoul(string(b + firstColon + 1, b + firstParen));
    //    size_t colNum = stoul(string(b + firstParen + 1, b + secondParen));
    //    string line;
    //    bool isPrinted = false;

    //    for (size_t i = 1; getline(ss, line); ++i) {

    //        cout << line << endl;
    //        if (isPrinted) {
    //            continue;
    //        }

    //        else if (i < lineNum) {
    //            continue;
    //        }

    //        isPrinted = true;
    //        for (size_t c = 0; c < colNum; ++c) {
    //            cout << "-";
    //        }
    //        cout << "^ here\n\n";
    //        break;
    //    }
    //    cout << info << endl;

    throw std::runtime_error(std::string{info});
}

ShaderObject::ShaderObject(GLenum shaderType, const std::string_view sourceIn) {
    shader = glCreateShader(shaderType);
#ifdef USING_GL2
    auto source = translateShader(sourceIn, shaderType);
#else
    auto &source = sourceIn;
#endif

    if (!shader) {
        throw std::runtime_error(
            std::string(__FILE__) + ":" + std::to_string(__LINE__) +
            ": cannot create shader, is the window and context "
            "started properly?");
    }
    (void)translateShader;

    auto *data = source.data();
    const GLint sourceSize = source.size();
    glShaderSource(shader, 1, &data, &sourceSize);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            std::vector<char> buffer(infoLen);
            glGetShaderInfoLog(shader, infoLen, nullptr, buffer.data());

            glDeleteShader(shader);
            shader = 0;

            printDebugInfo(buffer.data(), source);
        }
    }
}

ShaderObject::~ShaderObject() {
    glDeleteShader(shader);
}

ShaderObject::operator bool() const {
    return shader;
}

} // namespace matgui
