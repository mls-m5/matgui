/*
 * plainshader.h
 *
 *  Created on: 24 jan 2015
 *      Author: Mattias Larsson Sköld
 */


#pragma once


namespace TextureShader {

const char *vertexCode = R"V0G0N(
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
)V0G0N";


const char *fragmentCode = R"V0G0N(
varying vec4 fTex;
uniform sampler2D texture1;

void main() {
	float s = texture2D(texture1, fTex.xy).a;
	gl_FragColor = texture2D(texture1, fTex.xy) * s;
}
)V0G0N";


}

static struct {
	GLint color;
	GLint vertices;
	GLint texcoords;
	GLint mvpMatrix;
	GLint texture;
	unique_ptr<ShaderProgram> program;

	void init() {
		program.reset(new ShaderProgram(TextureShader::vertexCode, TextureShader::fragmentCode));
		debug_check_true(program->getProgram(), "could not create texture program");

		vertices = program->getAttribute("vPosition");
		texcoords = program->getAttribute("vtex");
		color = program->getUniform("uColor");
		mvpMatrix = program->getUniform("mvp_matrix");
		texture = program->getUniform("texture1");
	}
} textureProgram;

