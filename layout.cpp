/*
 * layout.cpp
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#include "layout.h"

#include <iostream>
using std::cout; using std::endl;

Layout::Layout():
orientation(LAYOUT_VERTICAL),
margin(4){

}

Layout::~Layout() {
}

void Layout::draw() {
	for (auto it: children){
		it->draw();
	}
}

void Layout::addChildAfter(View* view, View* after) {
	for (auto it = children.begin(); it != children.end(); ++it){
		if (*it == after){
			++it;
			children.insert(it, view);
			return;
		}
	}
	children.push_back(view);
}

void Layout::refreshChildren() {
	for (auto it : children) {
		it->refresh();
	}
}

void Layout::addChild(View* view) {
	children.push_back(view);

	refresh();

	refreshChildren();
}

void Layout::setOrientation(int orientation) {
	if (orientation != this->orientation) {
		this->orientation = orientation;
		refresh();

		refreshChildren();
	}
}

void Layout::calculateWeights() {
	auto restWidth = width;
	auto restHeight = height;
	double totalXWeight = 0;
	double totalYWeight = 0;
	for (auto it : children) {
		if (it->widthFlags == VIEW_WEIGHTED) {
			totalXWeight += it->weight;
		}
		else{
			restWidth -= it->width;
		}
		if (it->heightFlags == VIEW_WEIGHTED) {
			totalYWeight += it->weight;
		}
		else{
			restHeight -= it->height;
		}
	}
	auto paddingCorrection = margin * (children.size() + 1);
	restHeight -= paddingCorrection;
	restWidth -= paddingCorrection;
	for (auto it : children) {
		if (orientation == LAYOUT_HORIZONTAL){
			if (it->widthFlags == VIEW_WEIGHTED) {
				it->width = restWidth * it->weight / totalXWeight;
			}
		}
		else{
			it->width = width - margin * 2;
		}
		if (orientation == LAYOUT_VERTICAL){
			if (it->heightFlags == VIEW_WEIGHTED) {
				it->height = restHeight * it->weight / totalYWeight;
			}
		}
		else{
			it->height = height - margin * 2;
		}
	}
}

void Layout::refresh() {
	bool weighted = false;
	for (auto it: children){
		switch(it->widthFlags){
		case VIEW_MATCH_PARENT:
			it->width = width;
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
		switch(it->heightFlags){
		case VIEW_MATCH_PARENT:
			it->height = height;
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

//bool Layout::handleEvent(const ViewEvent& event) {
//	switch (event.type){
//	case ViewEvent::PointerDown:
//	case ViewEvent::PointerUp:
//	case ViewEvent::PointerMove:
//	{
//		auto x = event.pointer.x + xPos;
//		auto y = event.pointer.y + yPos;
//		ViewEvent e = event;
//		for (auto it: children){
//			if (it->isPointerInside(x, y)){
//				e.pointer.x = x - it->xPos;
//				e.pointer.y = y - it->yPos;
//
//				if (it->handleEvent(e)){
//					return true;
//				}
//			}
//		}
//		break;
//	}
//	} //switch end
//
//	return true;
//}

void Layout::removeChild(View* view) {
	children.remove(view);

	refresh();

	refreshChildren();
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
	auto wx = x + xPos;
	auto wy = y + yPos;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it->onPointerDown(id, x - it->xPos, y - it->yPos)){
				return true;
			}
		}
	}
	return false;
}

bool Layout::onPointerUp(pointerId id, double x, double y) {
	auto wx = x + xPos;
	auto wy = y + yPos;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it->onPointerUp(id, x - it->xPos, y - it->yPos)){
				return true;
			}
		}
	}
	return false;
}

bool Layout::onPointerMove(pointerId id, double x, double y,
		pointerState state) {
	auto wx = x + xPos;
	auto wy = y + yPos;

	for (auto it: children){
		if (it->isPointerInside(wx, wy)){
			if (it != pointerFocusedChild) {
				if (pointerFocusedChild) {
					pointerFocusedChild->onPointerLeave(id,
							x - pointerFocusedChild->xPos, y-pointerFocusedChild->yPos, state);
				}
				pointerFocusedChild = it;
				it->onPointerEnter(id, x - it->xPos, y - it->yPos, state);
			}
			if (it->onPointerMove(id, x - it->xPos, y - it->yPos, state)){
				return true;
			}
		}
	}
	if (pointerFocusedChild) {
		pointerFocusedChild->onPointerLeave(id,
				x - pointerFocusedChild->xPos, y - pointerFocusedChild->yPos, state);
		pointerFocusedChild = nullptr;
	}
	return false;
}
