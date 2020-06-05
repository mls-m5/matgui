/*
 * paint.cpp
 *
 *  Created on: 24 jun 2015
 *      Author: Mattias Larsson Sköld
 */

//#include "paint.h"
#include "matgui/draw.h"
#include "matgui/memberproperties.h"
#include "matgui/view.h"

void MatGui::Paint::drawRect(double x,
                             double y,
                             double width,
                             double height) const {
    MatGui::drawRect(x, y, width, height, this);
}

void MatGui::Paint::drawRect(const float *location) const {
    MatGui::drawRect(location, this);
}

void MatGui::Paint::push(Paint &s) {
    if (not s.enabled) {
        return;
    }
    fill += s.fill;
    line += s.line;
}

void MatGui::Paint::drawLine(double x, double y, double x2, double y2) const {
    if (line.style()) {
        MatGui::drawLine(x, y, x2, y2, this);
    }
}

void MatGui::Paint::drawLine(vec v1, vec v2) const {
    if (line.style()) {
        MatGui::drawLine(v1, v2, this);
    }
}

void MatGui::Paint::drawEllipse(double x,
                                double y,
                                double width,
                                double height) const {
    MatGui::drawEllipse(x, y, width, height, this);
}

void MatGui::Paint::drawEllipse(const float *location) const {
    MatGui::drawEllipse(location, this);
}

void MatGui::Paint::drawBasicView(View *view) const {
    drawRect(view->x(), view->y(), view->width(), view->height());
}

void MatGui::Paint::drawTriangle(vec v1, vec v2, vec v3) const {
    MatGui::drawTriangle(v1, v2, v3, this);
}

//} /* namespace MatGui */
