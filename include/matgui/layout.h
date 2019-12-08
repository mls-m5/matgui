/*
 * layout.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include <list>
#include "view.h"

namespace MatGui {

enum LayoutOrientation {
	LAYOUT_HORIZONTAL,
	LAYOUT_VERTICAL
};

class Layout: public View {
public:
	Layout();
	virtual ~Layout();

	void draw() override;

	bool onPointerDown(pointerId id, MouseButton button, double x, double y) override;
	bool onPointerUp(pointerId id, MouseButton button, double x, double y) override;
	bool onPointerMove(pointerId id, double x, double y, pointerState state) override;
	void onPointerEnter(pointerId id, double x, double y, pointerState state) override;
	void onPointerLeave() override;

	void refresh() override;
	void location(double x, double y, double w, double h, double weight = 0) override;

	[[deprecated ("use orientation(...) instead")]]
	 void setOrientation(LayoutOrientation o) {
		orientation(o);
	}

	void orientation(LayoutOrientation orientation);

	virtual void addChild(View *view);
	virtual void addChildAfter(View *view, View* after);
	virtual void removeChild(View *view);
	virtual void deleteChild(View *view);
	virtual void deleteAll();
	virtual View *getChild(size_t index);
	virtual View *getChild(std::string name);
	virtual void replaceChild(size_t index, View *view);
	void padding(double p){
		_padding = p;
		refresh();
	}


protected:
	void calculateWeights();
	void refreshChildren();

	std::list<View *> children;
	View *pointerFocusedChild = nullptr;

	LayoutOrientation _orientation;
	double _padding;
};

} //namespace MatGui
