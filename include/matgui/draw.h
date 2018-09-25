/*
 * draw.h
 *
 *  Created on: 19 aug 2014
 *      Author: mattias
 */


#pragma once

#include <vector>

namespace MatGui {

struct vec {
	vec(double x, double y, double z): x(x), y(y), z(z) {}
	vec(double x, double y): x(x), y(y), z(0) {}
	double x, y, z;
};

#ifdef __ANDROID__
typedef float DrawReal;
#define drawTypeName GL_FLOAT
#else
typedef double DrawReal;
#define drawTypeName GL_DOUBLE
#endif

struct vec2 {
	vec2() {}
	template <typename T>
	vec2(T v) : x(v.x), y(v.y) {}
	vec2(DrawReal x, DrawReal y) : x(x), y(y) {}
	DrawReal x; DrawReal y;
};

enum DrawStyle : int {
	None = 0,
	Filled = 1,
	Lines = 2,

	OrigoTopLeft = 0,
	CenterOrigo = 4,

	Inherit = 1024,
};

//This is only to be able to do binary operators
typedef int DrawStyle_t;

//Setup stuff
bool initDrawModule(double width, double height);
void QuitDrawModule();

//Graphics settings
void setDepthEnabled(bool enabled);

//Apply a transform to matrix at "pointer" in current shader program
void modelTransform(unsigned int pointer, vec p, double a, double scaleX, double scaleY);
void resetTransform(unsigned int poniter);

void drawRect(double x, double y, double width, double height, const class Paint*);
void drawRect(const float *locationMatrix, const class Paint*);

void drawEllipse(double x, double y, double width, double height, const class Paint*);
void drawEllipse(const float *locationMatrix, const class Paint*);

//A texture with origo in the top left corner
void drawTextureRect(vec p, double a, double sx, double sy, int textureId, DrawStyle_t = 0);
void drawTextureRect(float *locationMatrix, int textureId, DrawStyle_t = 0);

void drawGraph(double x, double y, double a, double sx, double sy, float *v, int size);

void drawLine(double x1, double y1, double x2, double y2, float width = 2);
void drawLine(double x1, double y1, double x2, double y2, const class Paint *paint);


void drawPolygon(double x, double y, double angle, std::vector<vec2> vectorList, const class Paint *paint);

inline void drawTextureRect(double x, double y, double a, double sx, double sy, int textureId, DrawStyle_t style = 0) {
	drawTextureRect({x, y}, a, sx, sy, textureId, style);
}

void setDimensions(double width, double height);

}

