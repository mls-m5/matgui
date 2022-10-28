#pragma once

#include "linearlayout.h"
#include "view.h"

namespace matgui {

class ScrollView : public View {
public:
    ScrollView();
    ~ScrollView() override = default;

    void draw() override;

    auto &layout() {
        return _layout;
    }

    void refresh() override;

    //! Create and add a child to to the layout
    template <class T, class... Types>
    constexpr T *createChild(Types &&...args) {
        return dynamic_cast<T *>(addChild(std::make_unique<T>(args...)));
    }

    View *addChild(std::unique_ptr<View> view) {
        return _layout.addChild(std::move(view));
    }

    void location(
        double x, double y, double w, double h, double weight = 1) override {
        _layout.location(0, 0, w, h);
    }

    void scrollY(double value);

    void orientation(LayoutOrientation orientation) {
        _layout.orientation(orientation);
    }

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

    member_property_declaration_default(double, scrollWidth, 0);
    member_property_declaration_default(double, scrollHeight, 0);
    member_get(double, scrollY);

private:
    LinearLayout _layout;

    double _scrollX = 0;
    double _scrollY = 0;

public:
    void invalidate() override {
        _layout.invalidate();
    }
};

} // namespace matgui
