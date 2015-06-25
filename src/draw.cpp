#include "draw.h"
#include "paint.h"
#include <math.h>
#include <vector>
#include <memory>

#include "shaderprogram.h"
#include "shaders/textureshader.h"
#include "shaders/plainshader.h"
#include "shaders/graphshader.h"
#include "shaders/lineshader.h"

using std::unique_ptr;

namespace MatGui {

static struct {
	GLint color;
	GLint vertices;
	GLint mvpMatrix;
} program1;


static struct {
	GLint color;
	GLint vertices;
	GLint texcoords;
	GLint mvpMatrix;
	GLint texture;
} textureProgram;

static struct {
	GLuint x;
	GLuint y;
	GLuint mvpMatrix;
} graphProgram;

static struct {
	GLuint v;
	GLuint mvpMatrix;
	GLuint color;
	unique_ptr<ShaderProgram> program = 0;
} lineProgram;


static GLfloat transformMatrix[16];
static double screenWidth, screenHeight;
static unique_ptr<ShaderProgram> squareShaderProgram = 0;

static unique_ptr<ShaderProgram> textureShaderProgram = 0;

static unique_ptr<ShaderProgram> graphShaderProgram = 0;

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
static const GLfloat gSquareVerticesCenteredAtOrigo[] = { -.5f, -.5f, .5f, -.5f, .5f, .5f, -.5f, .5f };
static const GLfloat gSquareColors[] = {
		.8, .8, 1., .5,
};

//Deprecated function
void drawRect(vec p, double a, double sx, double sy, DrawStyle_t drawStyle){
	squareShaderProgram->useProgram();

	modelTransform(program1.mvpMatrix, p, a, sx, sy);

    if (drawStyle & DrawStyle::CenterOrigo) {
    	glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVerticesCenteredAtOrigo);
    }
    else {
    	glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices);
    }
    glEnableVertexAttribArray(program1.vertices);

    glUniform4fv(program1.color, 1, gSquareColors);

    if (drawStyle & DrawStyle::Filled) {
    	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    else {
    	glDrawArrays(GL_LINE_LOOP, 0, 4);
    }

    glDisableVertexAttribArray(program1.vertices);
}


void drawRect(double x, double y, double width, double hegiht, class Paint* paint) {
	squareShaderProgram->useProgram();

	modelTransform(program1.mvpMatrix, {x, y}, 0, width, hegiht);

	glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices);
    glEnableVertexAttribArray(program1.vertices);

    if (paint->fill) {
    	glUniform4fv(program1.color, 1, &paint->fill.r);
    	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    if (paint->line) {
    	glLineWidth(paint->line.width());
    	glUniform4fv(program1.color, 1, &paint->line.r);
    	glDrawArrays(GL_LINE_LOOP, 0, 4);
    	glLineWidth(1);
    }

    glDisableVertexAttribArray(program1.vertices);
}

//static const GLfloat texturecoordinates[] = {0,0, 0,1, 1,0, 1,1};
void drawTextureRect(vec p, double a, double sx, double sy, int textureId, DrawStyle_t style) {
	textureShaderProgram->useProgram();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	modelTransform(textureProgram.mvpMatrix, p, a, sx, sy);
    glVertexAttribPointer(textureProgram.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices);
    if (style & DrawStyle::CenterOrigo) {
    	glVertexAttribPointer(textureProgram.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVerticesCenteredAtOrigo);
    }
    else {
    	glVertexAttribPointer(textureProgram.vertices, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices);
    }
    glEnableVertexAttribArray(textureProgram.vertices);


    glVertexAttribPointer(textureProgram.texcoords, 2, GL_FLOAT, GL_FALSE, 0, gSquareVertices); //Set the texture coordinates
    glEnableVertexAttribArray(textureProgram.texcoords);

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

//Deprecated function
void drawElipse(vec p, double a, double sx, double sy, DrawStyle_t drawStyle){
	squareShaderProgram->useProgram();

	modelTransform(program1.mvpMatrix, p, a / 180., sx, sy);
    glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, &elipseVertices[0]);
    glEnableVertexAttribArray(program1.vertices);

    glUniform4fv(program1.color, 1, gSquareColors);

    glDrawArrays((drawStyle & DrawStyle::Lines)? GL_LINE_LOOP: GL_TRIANGLE_FAN, 0, elipseVertices.size() / 2);

    glDisableVertexAttribArray(program1.vertices);
}

void drawElipse(double x, double y, double width, double height, class Paint* paint) {
	squareShaderProgram->useProgram();

	modelTransform(program1.mvpMatrix, {x, y}, 0, width, height);
    glVertexAttribPointer(program1.vertices, 2, GL_FLOAT, GL_FALSE, 0, &elipseVertices[0]);
    glEnableVertexAttribArray(program1.vertices);

    if (paint->fill) {
    	glUniform4fv(program1.color, 1, &paint->fill.r);
    	glDrawArrays(GL_TRIANGLE_FAN, 0, elipseVertices.size() / 2);
    }
    if (paint->line) {
    	glLineWidth(paint->line.width());
    	glUniform4fv(program1.color, 1, &paint->line.r);
    	glDrawArrays(GL_LINE_LOOP, 0, elipseVertices.size() / 2);
    	glLineWidth(1);
    }

    glDisableVertexAttribArray(program1.vertices);
}



static std::vector <float> tmpFloat;

void drawGraph(double x, double y, double a, double sx, double sy, float *v, int size){
	glLineWidth(2);
	graphShaderProgram->useProgram();

	if (tmpFloat.size() < size){
		tmpFloat.resize(size);
		for (int i = 0; i < size; ++i){
			tmpFloat[i] = i;
		}
	}

	modelTransform(graphProgram.mvpMatrix, {x, y}, a, sx, sy);

	glEnableVertexAttribArray(graphProgram.x);
    glVertexAttribPointer(graphProgram.x, 1, GL_FLOAT, GL_FALSE, 0, &tmpFloat[0]);
	glEnableVertexAttribArray(graphProgram.y);
    glVertexAttribPointer(graphProgram.y, 1, GL_FLOAT, GL_FALSE, 0, v);

    glDrawArrays(GL_LINE_STRIP, 0, size);

    glDisableVertexAttribArray(graphProgram.x);
    glDisableVertexAttribArray(graphProgram.y);

    glLineWidth(1);
}


void drawLine(double x1, double y1, double x2, double y2, float width) {
	glLineWidth(width);
	graphShaderProgram->useProgram();

#ifdef __ANDROID__
	typedef float type;
	constexpr int typeName = GL_FLOAT;
#else
	typedef double type;
	constexpr int typeName = GL_DOUBLE;
#endif

	type tmpX[] = {x1, x2};
	type tmpY[] = {y1, y2};

	modelTransform(graphProgram.mvpMatrix, {0, 0}, 0, 1, 1);

	glEnableVertexAttribArray(graphProgram.x);
    glVertexAttribPointer(graphProgram.x, 1, typeName, GL_FALSE, 0, tmpX);
	glEnableVertexAttribArray(graphProgram.y);
    glVertexAttribPointer(graphProgram.y, 1, typeName, GL_FALSE, 0, tmpY);

    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glDisableVertexAttribArray(graphProgram.x);
    glDisableVertexAttribArray(graphProgram.y);

    glLineWidth(1);
}


void drawLine(double x1, double y1, double x2, double y2, class Paint* paint) {
	glLineWidth(paint->line.width());
	lineProgram.program->useProgram();

#ifdef __ANDROID__
	typedef float type;
	constexpr int typeName = GL_FLOAT;
#else
	typedef double type;
	constexpr int typeName = GL_DOUBLE;
#endif

	type tmpV[] = {x1, y1, x2, y2};

	modelTransform(graphProgram.mvpMatrix, {0, 0}, 0, 1, 1);

	glEnableVertexAttribArray(lineProgram.v);
    glVertexAttribPointer(lineProgram.v, 2, typeName, GL_FALSE, 0, tmpV);

    glUniform4fv(lineProgram.color, 1, &paint->line.r);

    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glDisableVertexAttribArray(lineProgram.v);

    glLineWidth(1);
}


void setDimensions(double width, double height){
	screenWidth = width;
	screenHeight = height;
}

bool initDrawModule(double width, double height) {
	squareShaderProgram.reset(new ShaderProgram(PlainShader::vertexCode, PlainShader::fragmentCode));

    if (!squareShaderProgram->getProgram()) {
        debug_print("Could not create program.");
        return false;
    }

    squareShaderProgram->useProgram();
    checkGlError("glUseProgram");

	program1.vertices = squareShaderProgram->getAttribute("vPosition");
	program1.color = squareShaderProgram->getUniform("uColor");
	program1.mvpMatrix = squareShaderProgram->getUniform("mvp_matrix");



	textureShaderProgram.reset(new ShaderProgram(TextureShader::vertexCode, TextureShader::fragmentCode));

	textureProgram.vertices = textureShaderProgram->getAttribute("vPosition");
	textureProgram.texcoords = textureShaderProgram->getAttribute("vtex");
	textureProgram.color = textureShaderProgram->getUniform("uColor");
	textureProgram.mvpMatrix = textureShaderProgram->getUniform("mvp_matrix");
	textureProgram.texture = textureShaderProgram->getUniform("texture1");


	graphShaderProgram.reset(new ShaderProgram(GraphShader::vertexCode, GraphShader::fragmentCode));
	graphProgram.x = graphShaderProgram->getAttribute("vX");
	graphProgram.y = graphShaderProgram->getAttribute("vY");
	graphProgram.mvpMatrix = graphShaderProgram->getUniform("mvp_matrix");


	{
		auto program = new ShaderProgram(LineShader::vertexCode, LineShader::fragmentCode);
		lineProgram.program.reset(program);
		lineProgram.v = program->getAttribute("v");
		lineProgram.color = program->getUniform("uColor");
		lineProgram.mvpMatrix = program->getUniform("mvp_matrix");
	}

	setDimensions(width, height);

	return true;
}

void QuitDrawModule() {
	squareShaderProgram.reset();
	graphShaderProgram.reset();
	textureShaderProgram.reset();
	lineProgram.program.reset();
}

} //Namespace Matgui

