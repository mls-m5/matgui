/*
 * draw.h
 *
 *  Created on: 19 aug 2014
 *      Author: mattias
 */


#pragma once

namespace MatGui {

struct vec {
	vec(double x, double y, double z): x(x), y(y), z(z) {}
	vec(double x, double y): x(x), y(y), z(0) {}
	double x; double y; double z;
};

enum DrawStyle : int {
	Filled = 0,
	Lines = 1,

	OrigoTopLeft = 0,
	CenterOrigo = 2,
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
void drawElipse(vec p, double a, double sx, double sy, DrawStyle_t);

//A texture with origo in the top left corner
void drawTextureRect(vec p, double a, double sx, double sy, int textureId, DrawStyle_t = 0);

void drawGraph(double x, double y, double a, double sx, double sy, float *v, int size);
void drawLine(double x1, double y1, double x2, double y2);

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

