#include "draw.h"
#include <math.h>
#include <vector>

#include "shaderprogram.h"

static struct {
	GLint color;
	GLuint vertices;
	GLuint mvpMatrix;
} program1;

static struct {
	GLuint x;
	GLuint y;
	GLuint mvpMatrix;
} program2;


static GLfloat transformMatrix[16];
static double screenWidth, screenHeight;
ShaderProgram *squareShaderProgram = 0;

struct colorDataStruct{
	GLfloat r, g, b, a;
};

static const char gVertexShader[] =
    "attribute vec4 vPosition;\n"
    "uniform vec4 uColor;\n"
    "uniform	 mat4	 mvp_matrix;	 // model-view-projection matrix\n"
    "varying vec4 fColor;"
    "void main() {\n"
    "  gl_Position = mvp_matrix * vPosition;\n"
    "  fColor = uColor;\n"
    "}\n";

static const char gFragmentShader[] =
    "varying vec4 fColor;\n"
    "void main() {\n"
    "  gl_FragColor = fColor; \n"
    "}\n";

inline void identityMatrix(GLfloat *matrix){
	for (int i = 0; i < 16; ++i){
		matrix[i] = 0;
	}
	for (int i = 0; i < 4; ++i){
		matrix[i + i*4] = 1;
	}
}

void modelTransform(unsigned int pointer, vec p, double a, double scaleX, double scaleY){
	identityMatrix(transformMatrix);

	auto s = sin(a), c = cos(a);

	transformMatrix[0] = c * scaleX / screenWidth * 2;
	transformMatrix[1] = s * scaleX / screenWidth * 2;
	transformMatrix[4] = s * scaleY / screenHeight * 2;
	transformMatrix[5] = -c * scaleY / screenHeight * 2;

	transformMatrix[12] = p.x / screenWidth * 2 - 1.;
	transformMatrix[13] = -p.y / screenHeight * 2 + 1.;
	transformMatrix[14] = p.z;

    glUniformMatrix4fv(pointer, 1, GL_FALSE, transformMatrix);
}

void resetTransform(unsigned int pointer){
	identityMatrix(transformMatrix);
    glUniformMatrix4fv(pointer, 1, GL_FALSE, transformMatrix);
}

//Square
static const GLfloat gSquareVertices[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f };
static const GLfloat gSquareColors[] = {
		.8, .8, 1., 1,
};

void drawSquare(vec p, double a, double sx, double sy, DrawStyle drawStyle){
	squareShaderProgram->useProgram();

	modelTransform(program1.mvpMatrix, p, a / 180., sx, sy);
    glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices);
    glEnableVertexAttribArray(program1.vertices);

    glUniform4fv(program1.color, 1, gSquareColors);

    glDrawArrays((drawStyle == DRAW_STYLE_LINES)? GL_LINE_LOOP: GL_TRIANGLE_FAN, 0, 4);

    glDisableVertexAttribArray(program1.vertices);
}



static std::vector <GLfloat> elipseVertices;

void drawElipse(vec p, double a, double sx, double sy, DrawStyle drawStyle){
	squareShaderProgram->useProgram();

	modelTransform(program1.mvpMatrix, p, a / 180., sx, sy);
    glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, &elipseVertices[0]);
    glEnableVertexAttribArray(program1.vertices);

    glUniform4fv(program1.color, 1, gSquareColors);

    glDrawArrays((drawStyle == DRAW_STYLE_LINES)? GL_LINE_LOOP: GL_TRIANGLE_FAN, 0, elipseVertices.size() / 2);

    glDisableVertexAttribArray(program1.vertices);
}




void setDimensions(double width, double height){
	screenWidth = width;
	screenHeight = height;
}

bool initDrawModule(double width, double height) {
	if (squareShaderProgram){
		delete squareShaderProgram;
	}
    squareShaderProgram = new ShaderProgram(gVertexShader, gFragmentShader);

    if (!squareShaderProgram->getProgram()) {
        debug_print("Could not create program.");
        return false;
    }

    squareShaderProgram->useProgram();
    checkGlError("glUseProgram");

	program1.vertices = squareShaderProgram->getAttribute("vPosition");
	program1.color = squareShaderProgram->getUniform("uColor");
	program1.mvpMatrix = squareShaderProgram->getUniform("mvp_matrix");

	int count = 20;
	elipseVertices.resize(count * 2);
	for (int i = 0; i < count; ++i){
		auto a = (double) i / count * pi2;
		elipseVertices[i * 2] = .5 + sin(a) / 2;
		elipseVertices[i * 2 + 1] = .5 + cos(a) / 2;
	}


	setDimensions(width, height);
	return false;
}

void QuitDrawModule() {
	delete squareShaderProgram;
}
