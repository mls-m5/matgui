#pragma once

#include "shaderprogram.h"
#include <string_view>

namespace matgui {

#warning "remove this file"

class StandardShaderProgram : public ShaderProgram {
public:
    GLint vertexPointer;
    GLint colorPointer;
    GLint mvpMatrixPointer;

    StandardShaderProgram(const std::string_view vertexCode,
                          const std::string_view fragmentCode,
                          const std::string_view geometryCode = {});
    void disable();
};

} // namespace matgui
