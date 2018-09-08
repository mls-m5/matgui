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
	//pointers to shader variables
	GLint pColor;
	GLint pPertices;
	GLint pMvpMatrix;
	unique_ptr<ShaderProgram> program;

	//Square vertices
	GLfloat vertices[8] = { 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f };
	GLfloat centeredVertices[8] = { -.5f, -.5f, .5f, -.5f, .5f, .5f, -.5f, .5f };
	GLfloat colors[4] = {.8, .8, 1., .5,};

	std::vector <GLfloat> ellipseVertices;

	GLuint vertexArray = 0;
	GLuint vertexBuffer = 0;//, vertexBufferCentered = 0;
	GLuint ellipseVertexArray = 0;
	GLuint ellipseBuffer = 0;

	void init() {

		int count = 20;
		ellipseVertices.resize(count * 2);
		for (int i = 0; i < count; ++i){
			auto a = (double) i / count * pi2;
			ellipseVertices.at(i * 2) = .5 + sin(a) / 2;
			ellipseVertices.at(i * 2 + 1) = .5 + cos(a) / 2;
		}

		program.reset(new ShaderProgram(PlainShader::vertexCode, PlainShader::fragmentCode));
		debug_check_true(program->getProgram(), "could not create square program");
	    program->use();
	    checkGlError("glUseProgram");


		pPertices = program->getAttribute("vPosition");
		pColor = program->getUniform("uColor");
		pMvpMatrix = program->getUniform("mvp_matrix");

	    { //Initialize square program

			glGenVertexArrays(1, &vertexArray);
			glBindVertexArray(vertexArray);
	//		glGenBuffers(2, &vertexBuffer); //Note that i generate 2 buffers
			glGenBuffers(1, &vertexBuffer); //Note that i generate 2 buffers
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData( //One buffer for both's content
					GL_ARRAY_BUFFER,
					sizeof(vertices) * 2, //Change this if start using std::vector
					vertices,
					GL_STATIC_DRAW);


			glVertexAttribPointer(pPertices, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(pPertices);

	    }


	    { //Initialize square program

			glGenVertexArrays(1, &ellipseVertexArray);
			glBindVertexArray(ellipseVertexArray);

			glGenBuffers(1, &ellipseBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, ellipseBuffer);
	    }

		glBindVertexArray(0);
	}
} squareProgram;



