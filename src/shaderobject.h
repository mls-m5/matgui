#pragma once

#include "matgui/common-gl.h"
#include <filesystem>
#include <string_view>

namespace matgui {

struct ShaderObject {
    ShaderObject(const ShaderObject &) = delete;
    ShaderObject(ShaderObject &&) = delete;
    ShaderObject &operator=(const ShaderObject &) = delete;
    ShaderObject &operator=(ShaderObject &&) = delete;

    ShaderObject(GLenum shaderType,
                 const std::string_view sourceIn,
                 std::filesystem::path sourcePath);

    ~ShaderObject();

    operator bool() const;

    [[noreturn]] static void printDebugInfo(std::string_view info,
                                            std::string_view code,
                                            std::filesystem::path sourcePath);

    GLuint shader = 0;
    std::filesystem::path source;
};

} // namespace matgui
