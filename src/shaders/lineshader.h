/*
 * lineshader.h
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

namespace LineShader {

const char *vertexCode = R"V0G0N(
#version 330 core
in vec2 v;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
void main() {
  gl_Position = mvp_matrix * vec4(v.x, v.y, 1, 1);
}
)V0G0N";

const char *fragmentCode = R"_(
#version 330 core
uniform  vec4 uColor;
out vec4 fragColor;

void main() {
  fragColor = uColor;
}
)_";

} // namespace LineShader

static struct {
    GLuint v;
    GLuint mvpMatrix;
    GLuint color;
    std::unique_ptr<ShaderProgram> program;

    void init() {
        auto p =
            new ShaderProgram(LineShader::vertexCode, LineShader::fragmentCode);
        debug_check_true(p->getProgram(), "could not create graph program");
        program.reset(p);
        v = program->getAttribute("v");
        color = program->getUniform("uColor");
        mvpMatrix = program->getUniform("mvp_matrix");
    }
} lineProgram;
