/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <memory>

namespace GraphShader {

const char *vertexCode = R"_(
attribute float vX, vY;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
void main() {
  gl_Position = mvp_matrix * vec4(vX, vY, 1, 1);
}
)_";

const char *fragmentCode = R"_(
void main() {
  gl_FragColor = vec4(1,1,1,1);
}
)_";

} // namespace GraphShader

static struct {
    GLuint x;
    GLuint y;
    GLuint mvpMatrix;
    std::unique_ptr<ShaderProgram> program;
    std::vector<float> tmpFloat;

    void init() {
        program.reset(new ShaderProgram(GraphShader::vertexCode,
                                        GraphShader::fragmentCode));
        x = program->getAttribute("vX");
        y = program->getAttribute("vY");
        mvpMatrix = program->getUniform("mvp_matrix");

        debug_check_true(program->getProgram(),
                         "could not create graph program");
    }
} graphProgram;
