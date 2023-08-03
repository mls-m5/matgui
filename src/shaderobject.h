#pragma once

#include "matgui/common-gl.h"
#include <string_view>

namespace matgui {

struct ShaderObject {
    ShaderObject(const ShaderObject &) = delete;
    ShaderObject(ShaderObject &&) = delete;
    ShaderObject &operator=(const ShaderObject &) = delete;
    ShaderObject &operator=(ShaderObject &&) = delete;

    ShaderObject(GLenum shaderType, const std::string_view sourceIn);

    ~ShaderObject();

    operator bool() const;

    [[noreturn]] static void printDebugInfo(std::string_view info,
                                            std::string_view code);

    GLuint shader = 0;
};

} // namespace matgui
