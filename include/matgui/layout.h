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

namespace MatGui {

enum LayoutOrientation { LAYOUT_HORIZONTAL, LAYOUT_VERTICAL };

class Layout : public View {
public:
    Layout();
    virtual ~Layout();

    void draw() override;

    bool onPointerDown(pointerId id,
                       MouseButton button,
                       double x,
                       double y) override;
    bool onPointerUp(pointerId id,
                     MouseButton button,
                     double x,
                     double y) override;
    bool onPointerMove(pointerId id,
                       double x,
                       double y,
                       pointerState state) override;
    void onPointerEnter(pointerId id,
                        double x,
                        double y,
                        pointerState state) override;
    void onPointerLeave() override;

    void refresh() override;
    void location(
        double x, double y, double w, double h, double weight = 0) override;

    [[deprecated("use orientation(...) instead")]] //
        void
        setOrientation(LayoutOrientation o) { orientation(o); }

    void orientation(LayoutOrientation orientation);

    //! Take ownership over a view pointer and adds it to the layout
    //! @returns a raw pointer to the child
    View *addChild(View *view);
    View *addChild(std::unique_ptr<View> view);
    virtual void addChildAfter(View *view, View *after);
    //! Remove child from layout and deletes it
    virtual void removeChild(View *view);
    //! Remove child from layout but release pointer
    virtual std::unique_ptr<View> releaseChild(View *view);
    virtual void deleteAll();
    virtual View *getChild(size_t index);
    virtual View *getChild(std::string name);
    virtual void replaceChild(size_t index, View *view);
    void padding(double p) {
        _padding = p;
        refresh();
    }

#if __cplusplus >= 201402L
    //! Create and add a child to to the layout
    template <class T, class... Types>
    T *createChild(Types... args) {
        return dynamic_cast<T *>(
            addChild(std::move(std::make_unique<T>(args...))));
    }
#endif

protected:
    void calculateWeights();
    void refreshChildren();

    std::vector<std::unique_ptr<View>> children;
    View *pointerFocusedChild = nullptr;

    LayoutOrientation _orientation;
    double _padding;
};

} // namespace MatGui
