#include "matgui/scrollview.h"
#include "matgui/draw.h"

matgui::ScrollView::ScrollView() {
    int i = 0;
    double currentY = 0;

    double standardHeight = 50;

    for (View *child; (child = _layout.getChild(i)); ++i) {
        auto height = child->height();
        height = height ? height : standardHeight;

        child->location(0, currentY, width(), height);
        currentY += child->height();
    }
}

void matgui::ScrollView::draw() {
    matgui::pushViewport(x(), y(), width(), height());

    _layout.draw();

    matgui::popViewport();
}

void matgui::ScrollView::refresh() {
    View::refresh();
    _scrollWidth = std::max(width(), _scrollWidth);
    _scrollHeight = std::max(height(), _scrollHeight);

    _layout.location(0, 0, _scrollWidth, _scrollHeight);
}

void matgui::ScrollView::scrollY(double value) {
    // TODO: This does not work
    _layout.y(_scrollY);
    _layout.refresh();
}
