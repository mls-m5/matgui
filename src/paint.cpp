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

void matgui::Paint::drawRect(double x,
                             double y,
                             double width,
                             double height) const {
    matgui::drawRect(x, y, width, height, this);
}

void matgui::Paint::drawRect(const float *location) const {
    matgui::drawRect(location, this);
}

void matgui::Paint::push(Paint &s) {
    if (not s.enabled) {
        return;
    }
    fill += s.fill;
    line += s.line;
}

void matgui::Paint::drawLine(double x, double y, double x2, double y2) const {
    if (line.style()) {
        matgui::drawLine(x, y, x2, y2, this);
    }
}

void matgui::Paint::drawLine(vec v1, vec v2) const {
    if (line.style()) {
        matgui::drawLine(v1, v2, this);
    }
}

void matgui::Paint::drawEllipse(double x,
                                double y,
                                double width,
                                double height) const {
    matgui::drawEllipse(x, y, width, height, this);
}

void matgui::Paint::drawEllipse(const float *location) const {
    matgui::drawEllipse(location, this);
}

void matgui::Paint::drawBasicView(View *view) const {
    drawRect(view->x(), view->y(), view->width(), view->height());
}

void matgui::Paint::drawTriangle(vec v1, vec v2, vec v3) const {
    matgui::drawTriangle(v1, v2, v3, this);
}

//} /* namespace matgui */
