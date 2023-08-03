#include "matgui/standardshaderprogram.h"

namespace matgui {

StandardShaderProgram::StandardShaderProgram(
    const std::string_view vertexCode,
    const std::string_view fragmentCode,
    const std::string_view geometryCode)
    : ShaderProgram(vertexCode, fragmentCode, geometryCode) {

    vertexPointer = attribute("vPosition");
    colorPointer = attribute("vColor");
    mvpMatrixPointer = uniform("mvp_matrix");
}

void StandardShaderProgram::disable() {
    if (vertexPointer != -1) {
        glEnableVertexAttribArray(vertexPointer);
    }
    if (colorPointer != -1) {
        glEnableVertexAttribArray(colorPointer);
    }
}

} // namespace matgui
