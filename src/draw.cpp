#include "draw.h"
#include "paint.h"
#include <math.h>
#include <vector>
#include <memory>

#include "shaderprogram.h"

using std::unique_ptr;

namespace MatGui {

#define debug_check_true( cond , text) if (not (cond)) debug_print("%s: %s", #cond, text);

#include "shaders/plainshader.h"
#include "shaders/textureshader.h"
#include "shaders/graphshader.h"
#include "shaders/lineshader.h"


static GLfloat transformMatrix[16];
static double screenWidth, screenHeight;
static GLuint vertexArray = 0;


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



void drawRect(double x, double y, double width, double hegiht, class Paint* paint) {
	glBindVertexArray(squareProgram.vertexArray);
	squareProgram.program->use();

	modelTransform(squareProgram.pMvpMatrix, {x, y}, 0, width, hegiht);

    if (paint->fill) {
    	glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
    	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    if (paint->line) {
    	glLineWidth(paint->line.width());
    	glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
    	glDrawArrays(GL_LINE_LOOP, 0, 4);
    	glLineWidth(1);
    }

	glBindVertexArray(0);
}

void drawRect(float *location, class Paint* paint) {
	glBindVertexArray(squareProgram.vertexArray);
	squareProgram.program->use();

    glUniformMatrix4fv(squareProgram.pMvpMatrix, 1, GL_FALSE, location);

    if (paint->fill) {
    	glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
    	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    if (paint->line) {
    	glLineWidth(paint->line.width());
    	glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
    	glDrawArrays(GL_LINE_LOOP, 0, 4);
    	glLineWidth(1);
    }

	glBindVertexArray(0);
}

void drawTextureRect(vec p, double a, double sx, double sy, int textureId, DrawStyle_t style) {
	int arrayIndex = (style & DrawStyle::CenterOrigo)? 1: 0;
	glBindVertexArray(textureProgram.vertexArrays[arrayIndex]);

	textureProgram.program->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureProgram.texture, 0); //GL_TEXTURE0 equals 0
	modelTransform(textureProgram.mvpMatrix, p, a, sx, sy);


    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}




void drawEllipse(double x, double y, double width, double height, class Paint* paint) {
	glBindVertexArray(squareProgram.ellipseVertexArray);

	squareProgram.program->use();

	modelTransform(squareProgram.pMvpMatrix, {x, y}, 0, width, height);


    if (paint->fill) {
    	glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
    	glDrawArrays(GL_TRIANGLE_FAN, 0, squareProgram.ellipseVertices.size() / 2);
    }
    if (paint->line) {
    	glLineWidth(paint->line.width());
    	glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
    	glDrawArrays(GL_LINE_LOOP, 0, squareProgram.ellipseVertices.size() / 2);
    }

    glBindVertexArray(0);
}


void drawEllipse(float *location, class Paint* paint) {
	glBindVertexArray(squareProgram.ellipseVertexArray);

	squareProgram.program->use();

    glUniformMatrix4fv(squareProgram.pMvpMatrix, 1, GL_FALSE, location);

    if (paint->fill) {
    	glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
    	glDrawArrays(GL_TRIANGLE_FAN, 0, squareProgram.ellipseVertices.size() / 2);
    }
    if (paint->line) {
    	glLineWidth(paint->line.width());
    	glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
    	glDrawArrays(GL_LINE_LOOP, 0, squareProgram.ellipseVertices.size() / 2);
    }

    glBindVertexArray(0);
}



void drawGraph(double x, double y, double a, double sx, double sy, float *v, int size){
	glLineWidth(2);
	graphProgram.program->use();

	if (graphProgram.tmpFloat.size() < size){
		graphProgram.tmpFloat.resize(size);
		for (int i = 0; i < size; ++i){
			graphProgram.tmpFloat[i] = i;
		}
	}

	modelTransform(graphProgram.mvpMatrix, {x, y}, a, sx, sy);

	glEnableVertexAttribArray(graphProgram.x);
    glVertexAttribPointer(graphProgram.x, 1, GL_FLOAT, GL_FALSE, 0, &graphProgram.tmpFloat[0]);
	glEnableVertexAttribArray(graphProgram.y);
    glVertexAttribPointer(graphProgram.y, 1, GL_FLOAT, GL_FALSE, 0, v);

    glDrawArrays(GL_LINE_STRIP, 0, size);

    glDisableVertexAttribArray(graphProgram.x);
    glDisableVertexAttribArray(graphProgram.y);

    glLineWidth(1);
}


void drawLine(double x1, double y1, double x2, double y2, float width) {
	glLineWidth(width);
	graphProgram.program->use();

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
	lineProgram.program->use();

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



void drawPolygon(double x, double y, double angle, std::vector<vec2> vectorList, class Paint *paint) {
	glLineWidth(paint->line.width());
	lineProgram.program->use();

	modelTransform(lineProgram.mvpMatrix, { x, y }, angle, 1, 1);

	glEnableVertexAttribArray(lineProgram.v);
	glVertexAttribPointer(lineProgram.v, 2, drawTypeName, GL_FALSE, 0, &vectorList.front().x);


	if (paint->line) {
		glUniform4fv(lineProgram.color, 1, &paint->line.r);
		glDrawArrays(GL_LINE_LOOP, 0, vectorList.size());
	}
	if (paint->fill) {
		glUniform4fv(lineProgram.color, 1, &paint->fill.r);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vectorList.size());
	}

	glDisableVertexAttribArray(lineProgram.v);

	glLineWidth(1);
}


void setDimensions(double width, double height){
	screenWidth = width;
	screenHeight = height;
}

bool initDrawModule(double width, double height) {
    squareProgram.init();
    textureProgram.init();
    graphProgram.init();
    lineProgram.init();

    glGenVertexArrays(1, &vertexArray);

	setDimensions(width, height);

	return true;
}

void setDepthEnabled(bool enabled) {
	if (enabled) {
		glEnable(GL_DEPTH_TEST);

	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void QuitDrawModule() {
	squareProgram.program.reset();
	graphProgram.program.reset();
	textureProgram.program.reset();
	lineProgram.program.reset();
}

} //Namespace Matgui

