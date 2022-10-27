/*
 * draw.cpp
 *
 *  Created on: 19 aug 2014
 *      Author: mattias
 */

#include "matgui/draw.h"

#include "matgui/paint.h"
#include <cmath>
#include <memory>
#include <vector>

#include "matgui/gl-error-handling.h"
#include "matgui/shaderprogram.h"

#define debug_check_true(cond, text)                                           \
    if (not(cond))                                                             \
        debug_print("%s: %s", #cond, text);

#include "shaders/graphshader.h"
#include "shaders/lineshader.h"
#include "shaders/plainshader.h"
#include "shaders/textureshader.h"

using std::unique_ptr;

namespace matgui {

namespace {

GLfloat transformMatrix[16];
GLuint vertexArray = 0;

struct ViewportData {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

std::vector<ViewportData> viewportStack = {{0, 0, 0, 0}};

double screenWidth() {
    return viewportStack.back().width;
}

double screenHeight() {
    return viewportStack.back().height;
}

struct colorDataStruct {
    GLfloat r, g, b, a;
};

void identityMatrix(GLfloat *matrix) {
    for (int i = 0; i < 16; ++i) {
        matrix[i] = 0;
    }
    for (int i = 0; i < 4; ++i) {
        matrix[i + i * 4] = 1;
    }
}

void modelTransform(
    unsigned int pointer, vec p, double a, double scaleX, double scaleY) {
    identityMatrix(transformMatrix);

    double s = 0, c = 1;
    if (a) {
        s = sin(a);
        c = cos(a);
    }

    auto screenWidth = matgui::screenWidth();
    auto screenHeight = matgui::screenHeight();

    transformMatrix[0] = c * scaleX / screenWidth * 2;
    transformMatrix[1] = s * scaleX / screenWidth * 2;
    transformMatrix[4] = s * scaleY / screenHeight * 2;
    transformMatrix[5] = -c * scaleY / screenHeight * 2;

    transformMatrix[12] = p.x / screenWidth * 2 - 1.;
    transformMatrix[13] = -p.y / screenHeight * 2 + 1.;
    transformMatrix[14] = p.z;

    glUniformMatrix4fv(pointer, 1, GL_FALSE, transformMatrix);
}

} // namespace

void drawRect(
    double x, double y, double width, double hegiht, const class Paint *paint) {
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

void drawRect(const float *location, const class Paint *paint) {
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

void drawTriangle(vec v1, vec v2, vec v3, const Paint *paint) {
    typedef float T;
    // clang-format off
    const T location[] = {
        (T)((v2.x - v1.x) / screenWidth() * 2.),
        (T)(-(v2.y - v1.y) / screenHeight() * 2.),
        (T)(v2.z - v1.z), 0,

        (T)((v3.x - v1.x) / screenWidth() * 2), (T)(-(v3.y - v1.y) / screenHeight() * 2), 0, 0,
        0, 0, 1, 0,
        (T)(v1.x / screenWidth() * 2. - 1.),
        (T)(-v1.y / screenHeight() * 2. + 1.), (T)v1.z, 1,
    };
    // clang-format on

    drawTriangle(location, paint);
}

void drawTriangle(const float *location, const class Paint *paint) {
    glBindVertexArray(squareProgram.vertexArray);
    squareProgram.program->use();

    glUniformMatrix4fv(squareProgram.pMvpMatrix, 1, GL_FALSE, location);

    if (paint->fill) {
        glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    }
    if (paint->line) {
        glLineWidth(paint->line.width());
        glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
        glDrawArrays(GL_LINE_LOOP, 0, 3);
        glLineWidth(1);
    }

    glBindVertexArray(0);
}

void drawTextureRect(const float *locationMatrix,
                     int textureId,
                     DrawStyle_t style) {
    int arrayIndex = (style & DrawStyle::CenterOrigo) ? 1 : 0;
    glBindVertexArray(textureProgram.vertexArrays[arrayIndex]);

    textureProgram.program->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureProgram.texture, 0); // GL_TEXTURE0 equals 0

    glUniformMatrix4fv(textureProgram.mvpMatrix, 1, GL_FALSE, locationMatrix);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}

void drawTextureRect(
    vec p, double a, double sx, double sy, int textureId, DrawStyle_t style) {
    int arrayIndex = (style & DrawStyle::CenterOrigo) ? 1 : 0;
    glBindVertexArray(textureProgram.vertexArrays[arrayIndex]);

    textureProgram.program->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(textureProgram.texture, 0); // GL_TEXTURE0 equals 0
    modelTransform(textureProgram.mvpMatrix, p, a, sx, sy);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}

void drawEllipse(
    double x, double y, double width, double height, const class Paint *paint) {
    glBindVertexArray(squareProgram.ellipseVertexArray);

    squareProgram.program->use();

    modelTransform(squareProgram.pMvpMatrix, {x, y}, 0, width, height);

    if (paint->fill) {
        glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
        glDrawArrays(
            GL_TRIANGLE_FAN, 0, squareProgram.ellipseVertices.size() / 2);
    }
    if (paint->line) {
        glLineWidth(paint->line.width());
        glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
        glDrawArrays(GL_LINE_LOOP, 0, squareProgram.ellipseVertices.size() / 2);
    }

    glBindVertexArray(0);
}

void drawEllipse(const float *location, const class Paint *paint) {
    glBindVertexArray(squareProgram.ellipseVertexArray);

    squareProgram.program->use();

    glUniformMatrix4fv(squareProgram.pMvpMatrix, 1, GL_FALSE, location);

    if (paint->fill) {
        glUniform4fv(squareProgram.pColor, 1, &paint->fill.r);
        glDrawArrays(
            GL_TRIANGLE_FAN, 0, squareProgram.ellipseVertices.size() / 2);
    }
    if (paint->line) {
        glLineWidth(paint->line.width());
        glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
        glDrawArrays(GL_LINE_LOOP, 0, squareProgram.ellipseVertices.size() / 2);
    }

    glBindVertexArray(0);
}

void drawGraph(double /*x*/,
               double /*y*/,
               double /*a*/,
               double /*sx*/,
               double /*sy*/,
               const float * /*v*/,
               int /*size*/) {
    //    glLineWidth(2);
    //    graphProgram.program->use();
    //
    //    if (graphProgram.tmpFloat.size() < size){
    //        graphProgram.tmpFloat.resize(size);
    //        for (int i = 0; i < size; ++i){
    //            graphProgram.tmpFloat[i] = i;
    //        }
    //    }
    //
    //    modelTransform(graphProgram.mvpMatrix, {x, y}, a, sx, sy);
    //
    //    glEnableVertexAttribArray(graphProgram.x);
    //    glVertexAttribPointer(graphProgram.x, 1, GL_FLOAT, GL_FALSE, 0,
    //    &graphProgram.tmpFloat[0]); glEnableVertexAttribArray(graphProgram.y);
    //    glVertexAttribPointer(graphProgram.y, 1, GL_FLOAT, GL_FALSE, 0, v);
    //
    //    glDrawArrays(GL_LINE_STRIP, 0, size);
    //
    //    glDisableVertexAttribArray(graphProgram.x);
    //    glDisableVertexAttribArray(graphProgram.y);
    //
    //    glLineWidth(1);
}

// template <typename, typename> struct is_same { static const bool value =
// false;}; template <typename T> struct is_same<T,T> { static const bool value
// = true;};

template <class T>
void drawLine(T *location, const Paint *paint) {
    glBindVertexArray(squareProgram.vertexArray);
    squareProgram.program->use();

    glUniformMatrix4fv(
        squareProgram.pMvpMatrix, 1, GL_FALSE, (float *)location);

    if (paint->line) {
        glLineWidth(paint->line.width());
        glUniform4fv(squareProgram.pColor, 1, &paint->line.r);
        glDrawArrays(GL_LINE_LOOP, 0, 2);
        glLineWidth(1);
    }

    glBindVertexArray(0);
}

void drawLine(
    double x1, double y1, double x2, double y2, const class Paint *paint) {
    typedef float T;
    // clang-format off
    const T location[] = {
        (T)((x2 - x1) / screenWidth() * 2.), (T)(-(y2 - y1) / screenHeight() * 2.), 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        (T)(x1 / screenWidth() * 2. - 1.), (T)(-y1 / screenHeight() * 2. + 1.), 0, 1,
    };
    // clang-format on

    drawLine(location, paint);
}

void drawLine(vec v1, vec v2, const class Paint *paint) {
    typedef float T;
    // clang-format off
    const T location[] = {
        (T)((v2.x - v1.x) / screenWidth() * 2.),
        (T)(-(v2.y - v1.y) / screenHeight() * 2.),
        (T)(v2.z - v1.z), 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        (T)(v1.x / screenWidth() * 2. - 1.),
        (T)(-v1.y / screenHeight() * 2. + 1.), (T)v1.z, 1,
    };
    // clang-format on

    drawLine(location, paint);
}

void setDimensions(int width, int height) {
    viewportStack.front() = {0, 0, width, height};
    glViewport(0, 0, width, height);
}

void pushViewport(int x, int y, int width, int height) {
    viewportStack.push_back(
        {x, -y + (viewportStack.front().height - height), width, height});
    glViewport(x, viewportStack.back().y, width, height);
}

void popViewport() {
    if (viewportStack.size() > 1) {
        viewportStack.pop_back();
    }
    auto v = viewportStack.back();
    glViewport(v.x, v.y, v.width, v.height);
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

} // namespace matgui
