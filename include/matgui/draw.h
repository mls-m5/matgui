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
	double x; double y; double z;
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

//Apply a transform to matrix at "pointer" in current shader program
void modelTransform(unsigned int pointer, vec p, double a, double scaleX, double scaleY);
void resetTransform(unsigned int poniter);
//Draw a rectangle with origin in the top left corner
void drawRect(vec p, double a, double scaleX, double scaleY, DrawStyle_t);
void drawRect(double x, double y, double width, double hegiht, class Paint*);

void drawElipse(vec p, double a, double sx, double sy, DrawStyle_t);
void drawElipse(double x, double y, double width, double height, class Paint*);

//A texture with origo in the top left corner
void drawTextureRect(vec p, double a, double sx, double sy, int textureId, DrawStyle_t = 0);

void drawGraph(double x, double y, double a, double sx, double sy, float *v, int size);

void drawLine(double x1, double y1, double x2, double y2, float width = 2);
void drawLine(double x1, double y1, double x2, double y2, class Paint *paint);


void drawPolygon(double x, double y, double angle, std::vector<vec2> vectorList, class Paint *paint);

//Alternative syntax for functions (probably future syntax)
inline void drawElipse(double x, double y, double a, double sx, double sy, DrawStyle_t drawStyle) {
	drawElipse({x, y}, a, sx, sy, drawStyle);
}
inline void drawRect(double x, double y, double a, double scaleX, double scaleY, DrawStyle_t drawStyle){
	drawRect({x, y}, a, scaleX, scaleY, drawStyle);
}
inline void drawTextureRect(double x, double y, double a, double sx, double sy, int textureId, DrawStyle_t style = 0) {
	drawTextureRect({x, y}, a, sx, sy, textureId, style);
}

void setDimensions(double width, double height);

}

