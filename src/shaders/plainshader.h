/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


namespace PlainShader {

const char *vertexCode = R"V0G0N(
attribute vec4 vPosition;
//uniform vec4 uColor;
uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix
//varying vec4 fColor;

void main() {
	gl_Position = mvp_matrix * vPosition;
//	fColor = uColor / (1. + vPosition.y / 4.); //A little shading
}
)V0G0N";


const char *fragmentCode = R"V0G0N(
//varying vec4 fColor;
uniform vec4 uColor;

void main() {
//	gl_FragColor = fColor;
  gl_FragColor = uColor;
}
)V0G0N";


}

static struct {
	GLint color;
	GLint vertices;
	GLint mvpMatrix;
	unique_ptr<ShaderProgram> program;

	void init() {
		program.reset(new ShaderProgram(PlainShader::vertexCode, PlainShader::fragmentCode));
		debug_check_true(program->getProgram(), "could not create square program");
	    program->useProgram();
	    checkGlError("glUseProgram");

		vertices = program->getAttribute("vPosition");
		color = program->getUniform("uColor");
		mvpMatrix = program->getUniform("mvp_matrix");
	}
} squareProgram;



