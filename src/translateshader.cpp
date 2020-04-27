// Copyright © Mattias Larsson Sköld 2020

#include "translateshader.h"
#include <sstream>

using namespace std;

namespace {

bool startsWith(const string &str, const string &substr) {
    return str.substr(0, substr.size()) == substr;
}

// std::string getVariableName(std::string line) {
//    auto semicolonPos = line.find(';');
//    if (semicolonPos == string::npos) {
//        return {};
//    }
//    for (auto i = semicolonPos - 1; i > 1; --i) {
//        if (isspace(line.at(i))) {
//            return line.substr(i + 1, semicolonPos - i - 1);
//        }
//    }
//    return {};
//}

// string replaceWord(string line, string varName, string newVar) {
//    size_t f;
//    if ((f = line.find(varName))) {
//        return line;
//    }

//    if (f == 0) {
//        line.replace(f, varName.size(), newVar);
//    }

//    return line;
//}

string replaceAll(string line, string find, string replaceStr) {
    size_t start = 0u;
    size_t f = 0;
    while ((f = line.find(find, start)) != string::npos) {
        line.replace(f, find.size(), replaceStr);
        start = f + replaceStr.size();
    }
    return line;
}

//! Remove layout from a line
//! Assumes that layout is already found on the line
//! "layout (location = 0) in vec4 vPosition;" is converted to
//! in vec4 vPosition;"
string removeLayout(string line) {
    if (auto f = line.find("in")) {
        line.erase(line.begin(), line.begin() + f);
    }
    return line;
}

} // namespace

//! Used when using for example opengl es 3.0
//! For example for webgl
std::string translateShader(const std::string &fromSource,
                            GLenum /*shaderType*/) {
    string ret = "#version 300 es\n\nprecision mediump float;\n";

    istringstream ss(fromSource);

    string line;
    //    string fragmentOutName; // Because gl_FragColor is deprecated

    while (getline(ss, line)) {
        while (!line.empty() && isspace(line.front())) {
            line.erase(line.begin());
        }
        if (startsWith(line, "layout")) {
            line = removeLayout(line);
        }
        //        if (startsWith(line, "in ")) {
        //            if (shaderType == GL_VERTEX_SHADER) {
        //                line.replace(0, 3, "attribute ");
        //            }
        //            else {
        //                line.replace(0, 3, "varying ");
        //            }
        //        }
        //        else if (startsWith(line, "out ")) {
        //            if (shaderType == GL_VERTEX_SHADER) {
        //                line.replace(0, 4, "varying ");
        //            }
        //            else {
        //                fragmentOutName = getVariableName(line);
        //                line.clear();
        //            }
        //        }
        else if (startsWith(line, "#version")) {
            line.clear();
        }
        //        if (!fragmentOutName.empty()) {
        //            line = replaceWord(move(line), fragmentOutName,
        //            "gl_FragColor");
        //        }

        line = replaceAll(line, "texture2D", "texture");

        ret += line + "\n";
    }

    return ret;
}
