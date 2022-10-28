/*
 * layout.h
 *
 *  Created on: 21 aug 2014
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "view.h"

#include <memory>
#include <vector>

namespace matgui {

enum LayoutOrientation { LAYOUT_HORIZONTAL, LAYOUT_VERTICAL };

class Layout : public View {
public:
    Layout();
    virtual ~Layout();

    void draw() override;

    bool onPointerDown(PointerId id,
                       MouseButton button,
                       double x,
                       double y) override;
    bool onPointerUp(PointerId id,
                     MouseButton button,
                     double x,
                     double y) override;
    bool onPointerMove(PointerId id,
                       double x,
                       double y,
                       PointerState state) override;
    void onPointerEnter(PointerId id,
                        double x,
                        double y,
                        PointerState state) override;
    void onPointerLeave() override;

    void refresh() override;
    void location(
        double x, double y, double w, double h, double weight = 0) override;

    void orientation(LayoutOrientation orientation);

    //! Take ownership over a view pointer and adds it to the layout
    //! @returns a raw pointer to the child
    View *addChild(std::unique_ptr<View> view);
    virtual void addChildAfter(std::unique_ptr<View> view, View *after);
    //! Remove child from layout and leaves deletes if user does not use pointer
    virtual std::unique_ptr<View> removeChild(View *view);
    virtual void deleteAll();
    virtual View *getChild(size_t index);
    virtual View *getChild(std::string name);
    virtual void replaceChild(size_t index, View *view);
    void padding(double p) {
        _padding = p;
        refresh();
    }

    //! Create and add a child to to the layout
    template <class T, class... Types>
    constexpr T *createChild(Types &&...args) {
        return dynamic_cast<T *>(addChild(std::make_unique<T>(args...)));
    }

protected:
    void calculateWeights();
    void refreshChildren();

    std::vector<std::unique_ptr<View>> children;
    View *pointerFocusedChild = nullptr;

    LayoutOrientation _orientation;
    double _padding;
};

} // namespace matgui
