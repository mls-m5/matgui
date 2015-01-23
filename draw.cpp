#include "draw.h"
#include <math.h>
#include <vector>

#include "shaderprogram.h"

namespace MatGui {

static struct {
	GLint color;
	GLint vertices;
	GLint mvpMatrix;
} program1;


static struct {
	GLint color;
	GLint vertices;
	GLint mvpMatrix;
	GLint texture;
} textureProgram;


static GLfloat transformMatrix[16];
static double screenWidth, screenHeight;
ShaderProgram *squareShaderProgram = 0;

ShaderProgram *textureShaderProgram = 0;

struct colorDataStruct{
	GLfloat r, g, b, a;
};

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

	double s = 0, c = 1;
	if (a) {
		s = sin(a);
		c = cos(a);
	}

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
		.8, .8, 1., .5,
//		0,.5,.5,0
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

//static const GLfloat texturecoordinates[] = {0,0, 0,1, 1,0, 1,1};
void drawTexture(vec p, double a, double sx, double sy, int textureId) {
	textureShaderProgram->useProgram();

	modelTransform(textureProgram.mvpMatrix, p, a / 180., sx, sy);
    glVertexAttribPointer(textureProgram.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices);
    glEnableVertexAttribArray(textureProgram.vertices);

    glUniform4fv(textureProgram.color, 1, gSquareColors);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureProgram.texture, 0); //GL_TEXTURE0 equals 0

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


    glDisableVertexAttribArray(textureProgram.vertices);
}



static class ElipseVertices: public std::vector <GLfloat> {
public:
	ElipseVertices() {

		int count = 20;
		resize(count * 2);
		for (int i = 0; i < count; ++i){
			auto a = (double) i / count * pi2;
			at(i * 2) = .5 + sin(a) / 2;
			at(i * 2 + 1) = .5 + cos(a) / 2;
		}

	}
} elipseVertices;

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
//    squareShaderProgram = new ShaderProgram(gVertexShader, gFragmentShader);
	squareShaderProgram = new ShaderProgram;
	squareShaderProgram->loadShaderFromFile("shaders/shader.vsl", "shaders/shader.fsl");

    if (!squareShaderProgram->getProgram()) {
        debug_print("Could not create program.");
        return false;
    }

    squareShaderProgram->useProgram();
    checkGlError("glUseProgram");

	program1.vertices = squareShaderProgram->getAttribute("vPosition");
	program1.color = squareShaderProgram->getUniform("uColor");
	program1.mvpMatrix = squareShaderProgram->getUniform("mvp_matrix");



	textureShaderProgram = new ShaderProgram;
	textureShaderProgram->loadShaderFromFile("shaders/fontshader.vsl", "shaders/fontshader.fsl");

	textureProgram.vertices = textureShaderProgram->getAttribute("vPosition");
	textureProgram.color = textureShaderProgram->getUniform("uColor");
	textureProgram.mvpMatrix = textureShaderProgram->getUniform("mvp_matrix");
	textureProgram.texture = textureShaderProgram->getUniform("texture1");

	setDimensions(width, height);
	return false;
}

void QuitDrawModule() {
	delete squareShaderProgram;
}

} //Namespace Matgui
