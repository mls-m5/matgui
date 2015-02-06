/*
 * layout.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "layout.h"

#include <iostream>
using std::cout; using std::endl;

namespace MatGui {

Layout::Layout():
orientation(LAYOUT_VERTICAL),
margin(4){

}

Layout::~Layout() {
	deleteAll();
}

void Layout::draw() {
	for (auto it: children){
		it->draw();
	}
}

void Layout::deleteChild(View* view) {
	removeChild(view);
	delete view;
}

void Layout::refreshChildren() {
	for (auto it : children) {
		it->refresh();
	}
}

void Layout::addChild(View* view) {
	if (not view) {
		return;
	}
	children.push_back(view);
	view->parent(this);

	refresh();

	refreshChildren();
}


void Layout::addChildAfter(View* view, View* after) {
	view->parent(this);
	for (auto it = children.begin(); it != children.end(); ++it){
		if (*it == after){
			++it;
			children.insert(it, view);
			return;
		}
	}
	children.push_back(view);
}

void Layout::setOrientation(int orientation) {
	if (orientation != this->orientation) {
		this->orientation = orientation;
		refresh();

		refreshChildren();
	}
}

void Layout::calculateWeights() {
	auto restWidth = _width;
	auto restHeight = _height;
	double totalXWeight = 0;
	double totalYWeight = 0;
	for (auto it : children) {
		if (it->widthFlags() == VIEW_WEIGHTED) {
			totalXWeight += it->weight();
		}
		else{
			restWidth -= it->width();
		}
		if (it->heightFlags() == VIEW_WEIGHTED) {
			totalYWeight += it->weight();
		}
		else{
			restHeight -= it->height();
		}
	}
	auto paddingCorrection = margin * (children.size() + 1);
	restHeight -= paddingCorrection;
	restWidth -= paddingCorrection;
	for (auto it : children) {
		if (orientation == LAYOUT_HORIZONTAL){
			if (it->widthFlags() == VIEW_WEIGHTED) {
				it->width(restWidth * it->weight() / totalXWeight);
			}
		}
		else{
			it->width(_width - margin * 2);
		}
		if (orientation == LAYOUT_VERTICAL){
			if (it->heightFlags() == VIEW_WEIGHTED) {
				it->height(restHeight * it->weight() / totalYWeight);
			}
		}
		else{
			it->height(_height - margin * 2);
		}
	}
}

void Layout::refresh() {
	bool weighted = false;
	for (auto it: children){
		switch(it->widthFlags()){
		case VIEW_MATCH_PARENT:
			it->width(_width);
			break;
		case VIEW_WRAP_CONTENT:
			//TODO: fix this
			break;
		case VIEW_WEIGHTED:
			weighted = true;
			break;
		case 1:
			break;
		}
		switch(it->heightFlags()){
		case VIEW_MATCH_PARENT:
			it->height(_height);
			break;
		case VIEW_WRAP_CONTENT:
			//TODO: fix this
			break;
		case VIEW_WEIGHTED:
			weighted = true;
			break;
		case 1:
			break;
		}
	}

	if (weighted){
		calculateWeights();
	}
}

void Layout::setLocation(double x, double y, double w, double h, double weight) {
	View::setLocation(x, y, w, h, weight);

	refresh();

	refreshChildren();
}

void Layout::removeChild(View* view) {
	if (view == nullptr) {
		return;
	}

	if (view == pointerFocusedChild) {
		pointerFocusedChild = nullptr;
	}

	children.remove(view);
	if (view->parent() == this) {
		view->parent(nullptr);
	}

	refresh();

	refreshChildren();
}

void Layout::deleteAll() {
	for (auto it : children) {
		if (it->parent() == this) {
			it->parent(nullptr);
		}
		delete it;
	}

	children.clear();
	pointerFocusedChild = nullptr;

	refresh();
}

View *Layout::getChild(int index) {
	int i = 0;
	if (index >= children.size()){
		return 0;
	}
	for (auto it: children){
		if (i++ == index){
			return it;
		}
	}
	return 0;
}

void Layout::replaceChild(int index, View* view) {
	int i = 0;
	if (index >= children.size()){
		return;
	}
	for (auto &it: children){
		if (i++ == index){
			it = view;
		}
	}
}

bool Layout::onPointerDown(pointerId id, double x, double y) {
	auto wx = x + _x;
	auto wy = y + _y;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it->onPointerDown(id, wx - it->x(), wy - it->y())){
				return true;
			}
		}
	}
	return false;
}

bool Layout::onPointerUp(pointerId id, double x, double y) {
	auto wx = x + _x;
	auto wy = y + _y;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it->onPointerUp(id, wx - it->x(), wy - it->y())){
				return true;
			}
		}
	}
	return false;
}

bool Layout::onPointerMove(pointerId id, double x, double y,
		pointerState state) {
	auto wx = x + _x;
	auto wy = y + _y;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it != pointerFocusedChild) {
				if (pointerFocusedChild) {
					pointerFocusedChild->onPointerLeave();
				}
				pointerFocusedChild = it;
				it->onPointerEnter(id, wx - it->x(), wy - it->y(), state);
			}
			if (it->onPointerMove(id, wx - it->x(), wy - it->y(), state)){
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
	return false;
}

void Layout::onPointerEnter(pointerId id, double x, double y,
		pointerState state) {
	auto wx = x + _x;
	auto wy = y + _y;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it != pointerFocusedChild) {
				if (pointerFocusedChild) {
					pointerFocusedChild->onPointerLeave();
				}
				pointerFocusedChild = it;
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

View* Layout::getChild(std::string name) {
	for (auto it: children) {
		if (not it->name().empty() and it->name() == name) {
			return it;
		}
	}
	//If none is found iterate through all children
	for (auto it: children) {
		if (auto child = dynamic_cast<Layout*>(it)){
			auto res = child->getChild(name);
			if (res) {
				return res;
			}
		}
	}
	return nullptr;
}

} //Namespace MatGui

