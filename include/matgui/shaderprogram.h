/*
 * shaderProgram.h
 *
 *  Created on: 16 mar 2014
 *      Author: mattias
 */

#pragma once

#include "common-gl.h"

#include <string>

class ShaderProgram {
public:
	ShaderProgram(ShaderProgram &s){
		this->_program = s._program;
	}
	ShaderProgram();
	ShaderProgram(std::string vertexCode, std::string fragmentCode);
	void initProgram(std::string vertexCode, std::string fragmentCode);
	void loadShaderFromFile(std::string vertexFile, std::string fragmentFile);

	void useProgram();

	GLuint getProgram() { return _program; };
	GLint getUniform( char const* name );
	GLint getAttribute( char const* name );
	virtual ~ShaderProgram();

private:
	GLuint _program = 0;
};


class StandardShaderProgram: public ShaderProgram {
public:

	GLuint vertexPointer;
	GLuint colorPointer;
	GLuint mvpMatrixPointer;

	StandardShaderProgram(std::string vertexCode, std::string fragmentCode);
	void disable();
};


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    debug_print("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        debug_print("after %s() glError (0x%x)\n", op, error);
    }
}

