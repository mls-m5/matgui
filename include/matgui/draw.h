/*
 * draw.h
 *
 *  Created on: 19 aug 2014
 *      Author: mattias
 */

#pragma once

namespace matgui {

struct vec {
    vec(double x, double y, double z) : x(x), y(y), z(z) {
    }
    vec(double x, double y) : x(x), y(y), z(0) {
    }

    template <typename T>
    vec(T p) : x{p.x}, y{p.y}, z{p.z} {
    }

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
    vec2() {
    }
    template <typename T>
    vec2(T v) : x(v.x), y(v.y) {
    }
    vec2(DrawReal x, DrawReal y) : x(x), y(y) {
    }
    DrawReal x = 0;
    DrawReal y = 0;
};

enum DrawStyle : int {
    None = 0,
    Filled = 1,
    Lines = 2,

    OrigoTopLeft = 0,
    CenterOrigo = 4,

    Inherit = 1024,
};

// This is only to be able to do binary operators
typedef int DrawStyle_t;

// Setup stuff
bool initDrawModule(double width, double height);
void QuitDrawModule();

// Graphics settings
void setDepthEnabled(bool enabled);

// Apply a transform to matrix at "pointer" in current shader program
// void modelTransform(unsigned int pointer, vec p, double a, double scaleX,
// double scaleY);

// clang-format off
void resetTransform(unsigned int poniter);

void drawRect(double x, double y, double width, double height, const class Paint*);
void drawRect(const float *locationMatrix, const class Paint*);

void drawTriangle(vec v1, vec v2, vec v3, const Paint *paint);
void drawTriangle(const float *location, const class Paint* paint);

void drawEllipse(double x, double y, double width, double height, const class Paint*);
void drawEllipse(const float *locationMatrix, const class Paint*);

//A texture with origo in the top left corner
void drawTextureRect(vec p, double a, double sx, double sy, int textureId, DrawStyle_t);
void drawTextureRect(const float *locationMatrix, int textureId, DrawStyle_t = 0);

void drawGraph(double x, double y, double a, double sx, double sy, const float *v, int size);

void drawLine(double x1, double y1, double x2, double y2, const class Paint *paint);
void drawLine(vec v1, vec v2, const class Paint *paint);

inline void drawTextureRect(double x, double y, double a, double sx, double sy, int textureId, DrawStyle_t style) {
	drawTextureRect({x, y}, a, sx, sy, textureId, style);
}

// Set root dimensions for window
void setDimensions(int width, int height);

// For drawing sub windows
// y is from the top of the window (not as opengl)
void pushViewport(int x, int y, int width, int height);
void popViewport();

// Fill the current render buffer (often the screen) with a single color
void clear(float r, float g, float b, float a, bool depth = true);

// clang-format on

} // namespace matgui
