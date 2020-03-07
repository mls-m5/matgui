/*
 * layout.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "matgui/layout.h"

#include <algorithm>
#include <iostream>
using std::cout;
using std::endl;

namespace MatGui {

Layout::Layout() : _orientation(LAYOUT_VERTICAL), _padding(4) {
}

Layout::~Layout() {
    deleteAll();
}

void Layout::draw() {
    for (auto &it : children) {
        it->draw();
    }
}

void Layout::deleteChild(View *view) {
    removeChild(view);
    delete view;
}

void Layout::refreshChildren() {
    for (auto &it : children) {
        it->refresh();
    }
}

void Layout::addChild(std::unique_ptr<View> &&view) {
    if (not view) {
        return;
    }
    view->parent(this);
    children.push_back(std::move(view));

    refresh();

    refreshChildren();
}

void Layout::addChild(View *view) {
    addChild(std::unique_ptr<View>(view));
}

void Layout::addChildAfter(View *view, View *after) {
    view->parent(this);
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (it->get() == after) {
            ++it;
            children.insert(it, std::unique_ptr<View>(view));
            return;
        }
    }
    children.push_back(std::unique_ptr<View>(view));
}

void Layout::orientation(LayoutOrientation orientation) {
    if (orientation != this->_orientation) {
        this->_orientation = orientation;
        refresh();

        refreshChildren();
    }
}

void Layout::calculateWeights() {
    auto restWidth = width();
    auto restHeight = height();
    double totalXWeight = 0;
    double totalYWeight = 0;
    for (auto &it : children) {
        if (it->widthFlags() == VIEW_WEIGHTED) {
            totalXWeight += it->weight();
        }
        else {
            restWidth -= it->width();
        }
        if (it->heightFlags() == VIEW_WEIGHTED) {
            totalYWeight += it->weight();
        }
        else {
            restHeight -= it->height();
        }
    }
    auto paddingCorrection = _padding * (children.size() + 1);
    restHeight -= paddingCorrection;
    restWidth -= paddingCorrection;
    for (auto &it : children) {
        if (_orientation == LAYOUT_HORIZONTAL) {
            if (it->widthFlags() == VIEW_WEIGHTED) {
                it->width(restWidth * it->weight() / totalXWeight);
            }
        }
        else {
            it->width(width() - _padding * 2);
        }
        if (_orientation == LAYOUT_VERTICAL) {
            if (it->heightFlags() == VIEW_WEIGHTED) {
                it->height(restHeight * it->weight() / totalYWeight);
            }
        }
        else {
            it->height(height() - _padding * 2);
        }
    }
}

void Layout::refresh() {
    bool weighted = false;
    for (auto &it : children) {
        switch (it->widthFlags()) {
        case VIEW_MATCH_PARENT:
            it->width(width());
            break;
        case VIEW_WRAP_CONTENT:
            // TODO: fix this
            break;
        case VIEW_WEIGHTED:
            weighted = true;
            break;
        case 1:
            break;
        }
        switch (it->heightFlags()) {
        case VIEW_MATCH_PARENT:
            it->height(height());
            break;
        case VIEW_WRAP_CONTENT:
            // TODO: fix this
            break;
        case VIEW_WEIGHTED:
            weighted = true;
            break;
        case 1:
            break;
        }
    }

    if (weighted) {
        calculateWeights();
    }
}

void Layout::location(double x, double y, double w, double h, double weight) {
    View::location(x, y, w, h, weight);

    refresh();

    refreshChildren();
}

void Layout::removeChild(View *view) {
    if (view == nullptr) {
        return;
    }

    if (view == pointerFocusedChild) {
        pointerFocusedChild = nullptr;
    }

    for (auto it = children.begin(); it != children.end(); ++it) {
        if (it->get() == view) {
            if (view->parent() == this) {
                view->parent(nullptr);
            }
            children.erase(it);

            refresh();

            refreshChildren();

            return;
        }
    }
}

void Layout::deleteAll() {
    for (auto &it : children) {
        if (it->parent() == this) {
            it->parent(nullptr);
        }
    }

    children.clear();
    pointerFocusedChild = nullptr;

    refresh();
}

View *Layout::getChild(size_t index) {
    size_t i = 0;
    if (index >= children.size()) {
        return 0;
    }
    for (auto &it : children) {
        if (i++ == index) {
            return it.get();
        }
    }
    return 0;
}

void Layout::replaceChild(size_t index, View *view) {
    size_t i = 0;
    if (index >= children.size()) {
        return;
    }
    for (auto &it : children) {
        if (i++ == index) {
            it.reset(view);
        }
    }
}

bool Layout::onPointerDown(pointerId id,
                           MouseButton button,
                           double x,
                           double y) {
    auto wx = x + this->x();
    auto wy = y + this->y();

    for (auto &it : children) {
        if (it->isPointerInside(wx, wy)) {
            if (it->onPointerDown(id, button, wx - it->x(), wy - it->y())) {
                return true;
            }
        }
    }
    return View::onPointerDown(id, button, x, y);
}

bool Layout::onPointerUp(pointerId id, MouseButton button, double x, double y) {
    auto wx = x + this->x();
    auto wy = y + this->y();

    if (pointerFocusedChild) {
        if (not pointerFocusedChild->isPointerInside(wx, wy)) {
            pointerFocusedChild->onPointerLeave();
            pointerFocusedChild = nullptr;
            return true;
        }
    }

    for (auto &it : children) {
        if (it->isPointerInside(wx, wy)) {
            if (it->onPointerUp(id, button, wx - it->x(), wy - it->y())) {
                return true;
            }
        }
    }
    return View::onPointerUp(id, button, x, y);
}

bool Layout::onPointerMove(pointerId id,
                           double x,
                           double y,
                           pointerState state) {
    auto wx = x + this->x();
    auto wy = y + this->y();

    if (state) {
        if (pointerFocusedChild) {
            return pointerFocusedChild->onPointerMove(
                id,
                wx - pointerFocusedChild->x(),
                wy - pointerFocusedChild->y(),
                state);
        }
    }

    for (auto &it : children) {
        if (it->isPointerInside(wx, wy)) {
            if (it.get() != pointerFocusedChild) {
                if (pointerFocusedChild) {
                    pointerFocusedChild->onPointerLeave();
                }
                pointerFocusedChild = it.get();
                it->onPointerEnter(id, wx - it->x(), wy - it->y(), state);
            }
            if (it->onPointerMove(id, wx - it->x(), wy - it->y(), state)) {
                return true;
            }
        }
    }
    if (pointerFocusedChild) {
        if (not pointerFocusedChild->isPointerInside(wx, wy)) {
            pointerFocusedChild->onPointerLeave();
            pointerFocusedChild = nullptr;
        }
    }
    return View::onPointerMove(id, x, y, state);
}

void Layout::onPointerEnter(pointerId id,
                            double x,
                            double y,
                            pointerState state) {
    auto wx = x + this->x();
    auto wy = y + this->y();

    for (auto &it : children) {
        if (it->isPointerInside(wx, wy)) {
            if (it.get() != pointerFocusedChild) {
                if (pointerFocusedChild) {
                    pointerFocusedChild->onPointerLeave();
                }
                pointerFocusedChild = it.get();
                it->onPointerEnter(id, wx - it->x(), wy - it->y(), state);
                return;
            }
        }
    }
    View::onPointerEnter(id, x, y, state);
}

void Layout::onPointerLeave() {
    if (pointerFocusedChild) {
        pointerFocusedChild->onPointerLeave();
        pointerFocusedChild = nullptr;
    }
    View::onPointerLeave();
}

View *Layout::getChild(std::string name) {
    for (auto &it : children) {
        if (not it->name().empty() and it->name() == name) {
            return it.get();
        }
    }
    // If none is found iterate through all children
    for (auto &it : children) {
        if (auto child = dynamic_cast<Layout *>(it.get())) {
            auto res = child->getChild(name);
            if (res) {
                return res;
            }
        }
    }
    return nullptr;
}

} // Namespace MatGui
