/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "matgui/shaderprogram.h"
#include <memory>
#include <vector>

namespace GraphShader {

namespace {

const char *vertexCode = R"_(
#version 330 core
in float vX, vY;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
void main() {
  gl_Position = mvp_matrix * vec4(vX, vY, 1, 1);
}
)_";

const char *fragmentCode = R"_(
#version 330 core
out vec4 fragColor;

void main() {
  fragColor = vec4(1,1,1,1);
}
)_";

} // namespace

} // namespace GraphShader

static struct {
    GLuint x;
    GLuint y;
    GLuint mvpMatrix;
    std::unique_ptr<ShaderProgram> program;
    std::vector<float> tmpFloat;

    void init() {
        program = std::make_unique<ShaderProgram>(GraphShader::vertexCode,
                                                  GraphShader::fragmentCode);
        x = program->getAttribute("vX");
        y = program->getAttribute("vY");
        mvpMatrix = program->getUniform("mvp_matrix");

        debug_check_true(program->get(), "could not create graph program");
    }
} graphProgram;
