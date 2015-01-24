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

	virtual bool onPointerDown(pointerId id, double x, double y) override;
	virtual bool onPointerUp(pointerId id, double x, double y) override;
	virtual bool onPointerMove(pointerId id, double x, double y, pointerState state) override;
	virtual void onPointerEnter(pointerId id, double x, double y, pointerState state) override;
	virtual void onPointerLeave(pointerId id, double x, double y, pointerState state) override;

	virtual void refresh();
	virtual void setLocation(double x, double y, double w, double h, double weight = 0);
	void setOrientation(int orientation);

	virtual void addChild(View *view);
	virtual void addChildAfter(View *view, View* after);
	virtual void removeChild(View *view);
	virtual void deleteChild(View *view);
	virtual void clear(){
		children.clear();
	}
	virtual View *getChild(int index);
	virtual void replaceChild(int index, View *view);
	void setPadding(double p){
		margin = p;
		refresh();
	}


protected:
	void calculateWeights();
	void refreshChildren();

	std::list<View *> children;
	View *pointerFocusedChild = nullptr;

	int orientation;
	double margin;
};


} //namespace MatGui
