/*
 * label.h
 *
 *  Created on: 19 dec 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "font.h"
#include "view.h"

#include <memory>
#include <string>

namespace MatGui {

// A simple label
class Label : public View {
public:
    Label(std::string label = "");
    virtual ~Label();

    void draw() override;

    void label(const std::string text) {
        _fontView.text(text);
    }

    const std::string &label() {
        return _fontView.text();
    }

    void font(Font font) {
        _fontView.font(font);
    }

private:
    FontView _fontView;
};

} // namespace MatGui
