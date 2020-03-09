/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

namespace TextureShader {

const char *vertexCode = R"_(
attribute vec4 vPosition;
attribute vec4 vtex;
uniform vec4 uColor;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
varying vec4 fTex;

void main() {
	gl_Position = mvp_matrix * vPosition;
	//fColor = uColor;
	fTex = vec4(vtex.xy, 0,0);//vec4(vPosition.x, vPosition.y, 0, 0);
}
)_";

const char *fragmentCode = R"_(
varying vec4 fTex;
uniform sampler2D texture1;

void main() {
	float s = texture2D(texture1, fTex.xy).a;
	gl_FragColor = texture2D(texture1, fTex.xy) * s;
}
)_";

} // namespace TextureShader

static struct {
    GLint color;
    GLint vertices;
    GLint texcoords;
    GLint mvpMatrix;
    GLint texture;
    std::unique_ptr<ShaderProgram> program;

    enum { OrdinaryArray, CenderedArray, arrayEnd };
    GLuint vertexArrays[arrayEnd];
    GLuint vertexArrayCentered;

    const GLfloat colors[4] = {
        1.,
        1.,
        1.,
        1.,
    };

    void init() {
        program.reset(new ShaderProgram(TextureShader::vertexCode,
                                        TextureShader::fragmentCode));
        debug_check_true(program->getProgram(),
                         "could not create texture program");

        vertices = program->getAttribute("vPosition");
        texcoords = program->getAttribute("vtex");
        color = program->getUniform("uColor");
        mvpMatrix = program->getUniform("mvp_matrix");
        texture = program->getUniform("texture1");

        glGenVertexArrays(
            2, vertexArrays); // Generating both vertexArrays at thes ame time

        auto &textureProgram = *this;

        for (int i = 0; i < 2; ++i) {
            glBindVertexArray(vertexArrays[i]);

            textureProgram.program->use();
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindBuffer(GL_ARRAY_BUFFER, squareProgram.vertexBuffer);
            if (i) {
                glVertexAttribPointer(textureProgram.vertices,
                                      2,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      0,
                                      (void *)sizeof(squareProgram.vertices));
            }
            else {
                glVertexAttribPointer(
                    textureProgram.vertices, 2, GL_FLOAT, GL_FALSE, 0, 0);
            }
            glEnableVertexAttribArray(textureProgram.vertices);

            glVertexAttribPointer(textureProgram.texcoords,
                                  2,
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0,
                                  0); // Set the texture coordinates
            glEnableVertexAttribArray(textureProgram.texcoords);

            glUniform4fv(textureProgram.color, 1, textureProgram.colors);
        }
        glBindVertexArray(0);
    }
} textureProgram;
